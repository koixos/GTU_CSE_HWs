#include "common.h"
#include "file_operations/file_operations.h"
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERR_MSG "Try again or type \"fileManager\" to view the man page.\n"

const int show_help(void);
const int select_command(char **, const int, const char *[]);
const int count_char(const char *, const char);
const int split_line(const char *, const char, int, char **);
char **create_char_arr_ptr(const int);
char **resize_char_arr_ptr(char **, const int, const int);
void free_char_arr_ptr(char **, const int);
void signal_handler(const int);

int main(void) {
    signal(SIGCHLD, signal_handler);

    const char *commands[] = {
        "fileManager",
        "createDir",
        "createFile",
        "deleteDir",
        "deleteFile",
        "listDir",
        "listByExt",
        "readFile",
        "appendFile",
        "showLogs",
        "quit"
    };

    write(STDOUT_FILENO, "--------------------------------\n", strlen("--------------------------------\n"));
    write(STDOUT_FILENO, PROGRAM_NAME, strlen(PROGRAM_NAME));
    write(STDOUT_FILENO, PROGRAM_VERSION, strlen(PROGRAM_VERSION));
    write(STDOUT_FILENO, PROGRAMMER, strlen(PROGRAMMER));
    write(STDOUT_FILENO, PROGRAMMER_ID, strlen(PROGRAMMER_ID));
    write(STDOUT_FILENO, "--------------------------------", strlen("--------------------------------"));

    while (1) {
        char buffer[MAX_LENGTH], *command;

        write(STDOUT_FILENO, "\n-> ", strlen("\n-> "));
        ssize_t readBytes = read(STDIN_FILENO, buffer, MAX_LENGTH);

        if (0 > readBytes) {
            write(STDERR_FILENO, "ERROR: reading input\n", strlen("ERROR: reading input\n"));
            return EXIT_FAIL;
        } else if (0 == readBytes) {
            write(STDOUT_FILENO, "WARNING: EOF\n", strlen("WARNING: EOF\n"));
            break;
        } else if (MAX_LENGTH <= readBytes) {
            write(STDERR_FILENO, "ERROR: buffer overflow\n", strlen("ERROR: buffer overflow\n"));
            return EXIT_FAIL;
        } else {
            buffer[readBytes] = '\0';
            command = buffer;
        }
        
        int argNum = 1 + count_char(command, ' ');
        char **argsPtr = create_char_arr_ptr(argNum);
        if (NULL == argsPtr) {
            write(STDERR_FILENO, "ERROR: malloc\n", strlen("ERROR: malloc\n"));
            return EXIT_FAIL;
        }

        int tempArgNum = split_line(command, ' ', argNum, argsPtr);
        if (EXIT_FAIL == tempArgNum) {
            write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
            argsPtr[0] = command;
        } else {
            char **tempPtr = resize_char_arr_ptr(argsPtr, argNum, tempArgNum);
            if (NULL == tempPtr) {
                write(STDERR_FILENO, "ERROR: realloc\n", strlen("ERROR: realloc\n"));
                free_char_arr_ptr(argsPtr, argNum);
                return EXIT_FAIL;
            }

            argNum = tempArgNum;
            argsPtr = tempPtr;

            int result = select_command(argsPtr, argNum, commands);

            if (EXIT_QUIT == result) {
                write(STDOUT_FILENO, "Terminating...\n", strlen("Terminating...\n"));
                free_char_arr_ptr(argsPtr, argNum);
                return EXIT_SUCCESSFUL;
            }

            if (EXIT_FAIL == result)
                write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
        }
        free_char_arr_ptr(argsPtr, argNum);
    }
    return EXIT_SUCCESSFUL;
}

const int select_command(char **args, const int argNum, const char *commands[]) {
    if (OP_SUCCESS == strcmp(args[0], commands[0]) && argNum < 5) {
        if (1 == argNum) // man page
            return show_help();
        if (2 == argNum) {
            if (OP_SUCCESS == strcmp(args[1], commands[9])) { // showLogs
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    // child process
                    exit(show_logs());  
                } else {
                    // parent process
                    int status;
                    wait(&status); // wait for child process
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            } if (OP_SUCCESS == strcmp(args[1], commands[10])) // quit
                return EXIT_QUIT;
            return EXIT_FAIL;
        }
        if (3 == argNum) {
            if (OP_SUCCESS == strcmp(args[1], commands[1])) { // createDir
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(create_directory(args[2]));  
                } else {
                    int status;
                    wait(&status);
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            }
            if (OP_SUCCESS == strcmp(args[1], commands[2])) { // createFile
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(create_file(args[2]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            } 
            if (OP_SUCCESS == strcmp(args[1], commands[3])) { // deleteDir
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(delete_directory(args[2]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            } 
            if (OP_SUCCESS == strcmp(args[1], commands[4])) { // deleteFile
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(delete_file(args[2]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            } 
            if (OP_SUCCESS == strcmp(args[1], commands[5])) { // list
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(list(args[2]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            } 
            if (OP_SUCCESS == strcmp(args[1], commands[7])) { // readFile
                pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(read_file(args[2]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
            }
            return EXIT_FAIL;
        }
        if (OP_SUCCESS == strcmp(args[1], commands[6])) { // listByExt
            pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(list_by_extension(args[2], args[3]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
        } 
        if (OP_SUCCESS == strcmp(args[1], commands[8])) { // appendFile
            pid_t pid = fork();
                if (0 > pid) {
                    write(STDERR_FILENO, "ERROR: fork\n", strlen("ERROR: fork\n"));
                    return EXIT_FAIL;
                } else if (0 == pid) {
                    exit(append_file(args[2], args[3]));  
                } else {
                    int status;
                    wait(&status); 
                    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAIL;
                }
        }
        return EXIT_FAIL;
    }
    return EXIT_FAIL;
}

const int show_help(void) {
    const char *msg[] = {
        " $ Usage: fileManager <command> [args]\n\n",
        " # Commands:\n\n",
        " * createDir 'folderName'\t\t - create a new directory\n",
        " * createFile 'fileName'\t\t - create a new file\n",
        " * deleteDir 'folderName'\t\t - delete an empty directory\n",
        " * deleteFile 'fileName'\t\t - delete a file\n",
        " * listDir 'folderName'\t\t\t - list all files in a directory\n",
        " * listByExt 'folderName' <extension>\t - list files with specific extensions\n",
        " * readFile 'fileName'\t\t\t - read a file's content\n",
        " * appendFile 'fileName' 'content'\t - append content to a file\n",
        " * showLogs\t\t\t\t - display operation logs\n",
        " * quit\t\t\t\t\t - terminate the program\n"
    }; 

    int msgNum = 12;
    for (int i = 0; i < msgNum; i++) {
        write(STDOUT_FILENO, msg[i], strlen(msg[i]));
    }

    return EXIT_SUCCESSFUL;
}

const int count_char(const char *str, const char c) {
    int num = 0;
    for (int i = 0; i < MAX_LENGTH, '\0' != str[i]; i++)
        if (c == str[i])
            ++num;
    return num;
}

const int split_line(const char *str, const char delim, int argNum, char **args) {
    for (int i = 0; i < argNum; i++)
        for (int j = 0; j < MAX_LENGTH; j++)
            args[i][j] = '\0';

    if (0 != count_char(str, '"') % 2)
        return EXIT_FAIL;

    int lettermark = 0;
    for (int i = 0; i < argNum; i++) {
        for (int j = 0; j+lettermark < MAX_LENGTH; j++) {
            if ('\0' == str[j+lettermark] || '\n' == str[j+lettermark]) {
                args[i][j] = '\0';
                i = MAX_ARGS;
                break;
            } else if (delim == str[j+lettermark]) {
                args[i][j] = '\0';
                lettermark += j+1;
                break;
            } else if ('"' == str[j+lettermark]) {
                int k = j+lettermark+1;
                for (; k < MAX_LENGTH, '"' != str[k]; k++) {
                    if ('\0' == str[k+1] || '\n' == str[k+1] || MAX_LENGTH-1 == k && '"' != str[k])
                        return EXIT_FAIL;
                    if (delim == str[k])
                        --argNum;
                    args[i][k-lettermark-1] = str[k];
                }
                args[i][k-lettermark] = '\0';
                if (delim == str[k+1])
                    k += 2;
                lettermark = k;
                break;
            } else {
                args[i][j] = str[j+lettermark];
            }
        }
    }
    return argNum;
}

char **create_char_arr_ptr(const int size) {
    char **ptr = (char **)malloc(size * sizeof(char *));
    if (NULL == ptr)
        return NULL;

    for (int i = 0; i < size; i++) {
        ptr[i] = (char *)malloc(MAX_LENGTH * sizeof(char));
        if (NULL == ptr[i]) {
            free_char_arr_ptr(ptr, i);
            return NULL;
        }
    }
    
    return ptr;
}

char **resize_char_arr_ptr(char **ptr, const int prevSize, const int newSize) {
    if (NULL == ptr)
        return NULL;

    char **temp = (char **)realloc(ptr, newSize * sizeof(char *));
    if (NULL == temp)
        return ptr;

    if (newSize < prevSize) {
        for (int i = newSize; i < prevSize; i++) {
            free(temp[i]);
            temp[i] = NULL;
        }
    }

    return temp;
}

void free_char_arr_ptr(char **ptr, const int size) {
    if(NULL == ptr)
        return;

    for (int i = 0; i < size; i++) {
        if (NULL != ptr[i]) {
            free(ptr[i]);
            ptr[i] = NULL;
        }
    }

    free(ptr);
}

void signal_handler(const int signo) {
    if (SIGCHLD == signo)
        wait(NULL);
}
