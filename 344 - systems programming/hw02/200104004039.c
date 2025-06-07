#define _POSIX_SOURCE
#define SA_RESTART 0

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <assert.h>
#include <errno.h>
#include <syslog.h>
#include <time.h>
#include <sys/select.h>

#define FIFO_1 "MY_FIFO_1"
#define FIFO_2 "MY_FIFO_2"

#define FILE_MODE   0666
#define SLEEP       10

#define MAX_ARGS    3    
#define MAX_CHILD   2
#define OPERATION   "find_largest"
#define LOG_FILE    "log.txt"
 
volatile sig_atomic_t child_counter = 0;
int log_fd;

void sigchld_handler(int);
void sig_handler(int);
void daemonize();
void close_fifos();

int read_with_timeout(int fd, void *buf, size_t count, int timeout_secs) {
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    tv.tv_sec = timeout_secs;
    tv.tv_usec = 0;

    int ready = select(fd + 1, &readfds, NULL, NULL, &tv);
    if (ready == -1) {
        return -1;
    } else if (ready == 0) {
        errno = ETIMEDOUT;
        return -1;
    }
    return read(fd, buf, count);
}

void _log(const int fd, const char* msg) {
    time_t now;
    time(&now);

    char time_str[100], log_buf[512];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    snprintf(log_buf, sizeof(log_buf), "[%s] %s", time_str, msg);
    write(fd, log_buf, strlen(log_buf));
}

int main(int argc, char *argv[]) {
    int result = 0, fd;
    int len_cmd = strlen(OPERATION);
    char buf[256];

    log_fd = open(LOG_FILE, O_WRONLY | O_CREAT, 0644);
    if (log_fd < 0) {
        fprintf(stderr, "Cannot open log file %s: %s\n", LOG_FILE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (argc != MAX_ARGS) {
        fprintf(stderr, "# Invalid use of command\nUsage: %s <int1> <int2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);    

    signal(SIGUSR1, sig_handler);
    signal(SIGHUP, sig_handler);
    signal(SIGTERM, sig_handler);    

    pid_t pid1 = fork();
    if (pid1 == -1) {
        _log(STDERR_FILENO, "# Fork 1 failed\n");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        sleep(SLEEP);

        _log(STDOUT_FILENO, "- Greetings from the 1st child!\n");
        _log(STDOUT_FILENO, "- Hold on a second...\n\n");

        fd = open(FIFO_1, O_RDONLY);
        if (fd == -1) {
            _log(STDERR_FILENO, "# Failed to open FIFO 1\n");
            exit(EXIT_FAILURE);
        }

        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
        
        int read_nums[MAX_ARGS-1];
        for (int i = 0; i < MAX_ARGS-1; i++) {
            if (read(fd, &read_nums[i], sizeof(int)) != sizeof(int)) {
                _log(STDERR_FILENO, "# Failed to read the numbers from FIFO 1\n");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }
        close(fd);

        int largest = read_nums[0];
        _log(STDOUT_FILENO, "- Read numbers: ");
        for (int i = 0; i < MAX_ARGS-1; i++) {
            snprintf(buf, sizeof(buf), "%d ", read_nums[i]);
            _log(STDOUT_FILENO, buf);
            if (read_nums[i] > largest)
                largest = read_nums[i];
        }
        _log(STDOUT_FILENO, "\n- The largest number is found.\n");

        fd = open(FIFO_2, O_WRONLY | O_APPEND);
        if (fd == -1) {
            _log(STDERR_FILENO, "# Failed to open FIFO 2\n");
            exit(EXIT_FAILURE);
        }

        if (write(fd, &largest, sizeof(int)) != sizeof(int)) {
            _log(STDERR_FILENO, "# Failed to write result to FIFO 2\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
        _log(STDOUT_FILENO, "- The largest number is written to FIFO 2.\n");
        _log(STDOUT_FILENO, "- First child is successfully completed!\n\n");
        exit(EXIT_SUCCESS);
    } else {
        pid_t pid2 = fork();
        if (pid2 == -1) {
            _log(STDERR_FILENO, "# Fork 2 failed\n");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            sleep(SLEEP);

            _log(STDOUT_FILENO, "- Greetings from the 2nd child!\n");
            _log(STDOUT_FILENO, "- Hold on a second...\n\n");

            fd = open(FIFO_2, O_RDONLY);
            if (fd == -1) {
                _log(STDERR_FILENO, "# Failed to open FIFO 2\n");
                exit(EXIT_FAILURE);
            }
            _log(STDOUT_FILENO, "- Reading FIFO 2...\n");

            fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);

            int timeout = 5;
            char read_cmd[len_cmd+1];
            if (read_with_timeout(fd, read_cmd, len_cmd, timeout) != len_cmd) {
                _log(STDERR_FILENO, "# Failed to read the command from FIFO 2 (timeout)\n");
                close(fd);
                exit(EXIT_FAILURE);
            }
            read_cmd[len_cmd] = '\0';

            sleep(2);

            if (read_with_timeout(fd, &result, sizeof(int), timeout) != sizeof(int)) {
                _log(STDERR_FILENO, "# Failed to read the result from FIFO 2 (timeout)\n");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);

            snprintf(buf, sizeof(buf), "- Command and the result, respectively: %s - %d\n", read_cmd, result);
            _log(STDOUT_FILENO, buf);
            _log(STDOUT_FILENO, "- 2nd child is successfully completed!\n\n");
            exit(EXIT_SUCCESS);
        } else {
            _log(STDOUT_FILENO, "- Hi, this is parent talking!\n\n");

            if (mkfifo(FIFO_1, FILE_MODE) == -1 && errno != EEXIST) {
                _log(STDERR_FILENO, "# Failed to create FIFO 1\n");
                exit(EXIT_FAILURE);
            }
            _log(STDOUT_FILENO, "- FIFO 1 is created.\n");
            if (mkfifo(FIFO_2, FILE_MODE) == -1 && errno != EEXIST) {
                _log(STDERR_FILENO, "# Failed to create FIFO 2\n");
                exit(EXIT_FAILURE);
            }
            _log (STDOUT_FILENO, "- FIFO 2 is created.\n");

            sleep(1);

            fd = open(FIFO_2, O_WRONLY);
            if (fd == -1) {
                _log(STDERR_FILENO, "# Failed to open FIFO 2\n");
                exit(EXIT_FAILURE);
            }

            if (write(fd, OPERATION, len_cmd) != len_cmd) {
                _log(STDERR_FILENO, "# Failed to write the command to FIFO 2\n");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);
            _log(STDOUT_FILENO, "- Command is written to FIFO 2.\n");
            
            fd = open(FIFO_1, O_WRONLY);
            if (fd == -1) {
                _log(STDERR_FILENO, "# Failed to open FIFO 1\n");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < MAX_ARGS-1; i++) {
                int num = atoi(argv[i+1]);
                if (write(fd, &num, sizeof(int)) != sizeof(int)) {
                    _log(STDERR_FILENO, "# Failed to write the numbers to FIFO 1\n");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
            close(fd);
            _log(STDOUT_FILENO, "- Numbers are written to FIFO 1.\n");

            _log(STDOUT_FILENO, "- Parent is daemonizing...\n");
            daemonize();

            time_t start_time = time(NULL);
            int timeout = 30;
            
            while (1) {
                _log(STDOUT_FILENO, "\nProceeding...\n\n");

                if (time(NULL) - start_time > timeout) {
                    _log(STDOUT_FILENO, "- Timeout reached, terminating children...\n");
                    kill(0, SIGTERM);
                    break;
                }

                sleep(2);
                if (child_counter >= MAX_CHILD) {
                    _log(STDOUT_FILENO, "- All child processes have done. Terminating...\n");
                    break;
                }
            }

            _log(STDOUT_FILENO, "- Deleting FIFOs...\n");
            close(log_fd);
            close_fifos();
        }
    }

    exit(EXIT_SUCCESS);
}

void sigchld_handler(int signo) {
    int stat;
    char buf[256];
    pid_t child_pid;

    while ((child_pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        if (WIFEXITED(stat)) {           
            snprintf(buf, sizeof(buf), "--> Child %d successfully completed with exit status %d.\n\n", child_pid, WEXITSTATUS(stat));
            _log(STDOUT_FILENO, buf);
            if (WEXITSTATUS(stat) == EXIT_SUCCESS)
                ++child_counter;
        } else if (WIFSIGNALED(stat)) {
            snprintf(buf, sizeof(buf), "--> Child %d terminated by signal %d",  child_pid, WTERMSIG(stat));
            _log(STDOUT_FILENO, buf);
        }
    }
}

void sig_handler(int signo) {
    if (signo == SIGUSR1) {
        _log(STDOUT_FILENO, "--> Received SIGUSR1\n");
    } else if (signo == SIGHUP) {
        _log(STDOUT_FILENO, "--> Received SIGHUP\n");
    } else if (signo == SIGTERM) {
        _log(STDOUT_FILENO, "--> Received SIGTERM. Exiting...\n");
        close(log_fd);
        close_fifos();
        exit(0);
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid == -1) {
        _log(STDERR_FILENO, "# 1st fork to daemonize failed\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        _log(STDERR_FILENO, "# Creating a new session to daemonize failed\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        _log(STDERR_FILENO, "# 2nd fork to daemonize failed\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    int original_log_fd = log_fd;

    for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++)
        if (i != original_log_fd)
            close(i);

    log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        syslog(LOG_ERR, "Cannot open _log file %s", LOG_FILE);
        exit(EXIT_FAILURE);
    }

    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);

    int null_fd = open("/dev/null", O_RDWR);
    if (null_fd != -1) {
        dup2(null_fd, STDIN_FILENO);
        if (null_fd > 2)
            close(null_fd);
    }

    _log(STDOUT_FILENO, "--> Daemon process started\n");
}

void close_fifos() {
    unlink(FIFO_1);
    unlink(FIFO_2);
    _log(STDOUT_FILENO, "--> FIFOs have been unlinked\n");
}
