#include "../common.h"

volatile int shouldexit = 0;
int sockfd;
char username[17];

pthread_mutex_t exitlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outplock = PTHREAD_MUTEX_INITIALIZER;

void *receiver(void *);
void *sender(void *);
void safe_printf(const char *);
void set_shouldexit(int);
int read_shouldexit(void);
void cleanup_exit(int);
void sig_handler(int);

int main(int argc, char *argv[]) {
    char buf[BUFLEN];
    int portno;
    struct sockaddr_in servaddr;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("opening client socket\n");
    
    server = gethostbyname(argv[1]);
    if (!server)
        error("no such host\n");
    
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portno);
    bcopy(
        (char *)server->h_addr_list[0],
        (char *)&servaddr.sin_addr.s_addr,
        server->h_length
    );

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        error("connecting\n");  

    signal(SIGINT, sig_handler);

    printf("-------------------------- Welcome to ZChat ---------------------------\n");
    printf("\033[0;32m[SUCCESS]: Connected to server %s on port %s\033[0m\n", argv[1], argv[2]);
    printf("-----------------------------------------------------------------------\n");

    while (!read_shouldexit()) {
        bzero(buf, BUFLEN);
        printf("$ Enter your username: ");

        fgets(buf, BUFLEN-1, stdin);
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) <= 0) {
            printf("\033[0;31m[ERROR]: Username cannot be empty\033[0m\n");
            continue;
        } 

        strcpy(username, buf);
        write_fd(sockfd, buf);
        bzero(buf, BUFLEN);
        read(sockfd, buf, BUFLEN-1);

        if (strcmp(buf, "error") == 0) {
            printf("\033[0;31m[ERROR]: Invalid or duplicate username. Try again\033[0m\n");
            fflush(stdout);
        } else if (strcmp(buf, "full") == 0) {
            printf("\033[0;31m[ERROR]: Server is full. Try later\033[0m\n");
            close(sockfd);
            exit(1);
        } else {
            printf("\033[0;33m[SERVER]: %s\033[0m\n", buf);
            break;
        }
    }

    pthread_t recv_tid, send_tid;

    if (pthread_create(&recv_tid, NULL, receiver, NULL) != 0)
        error("creating receiver");
    
    if (pthread_create(&send_tid, NULL, sender, NULL) != 0)
        error("creating sender");

    pthread_join(recv_tid, NULL);
    pthread_join(send_tid, NULL);

    if (sockfd >= 0) {
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    }

    pthread_mutex_destroy(&exitlock);
    pthread_mutex_destroy(&outplock);

    return 0;
}

void *receiver(void *arg) {
    char buf[BUFLEN];
    while (!read_shouldexit()) {
        bzero(buf, BUFLEN);
        int n = read(sockfd, buf, BUFLEN-1);
        if (n <= 0) {   
            if (!read_shouldexit()) 
                safe_printf("\r\033[0;34m[INFO]: Disconnected from server\033[0m\n");
            set_shouldexit(1);
            break;
        }
        buf[n] = '\0';

        if (strncmp(buf, "shutdown", 8) == 0) {
            safe_printf("\r\033[0;31m[ERROR]: Server is down...\033[0m\n");
            set_shouldexit(1);
            close(STDIN_FILENO);
            break;
        }

        if (strstr(buf, "broadcasted") != NULL || strstr(buf, "whispered") != NULL || strstr(buf, "File received") != NULL || strstr(buf, "File sent") != NULL) {
            strcat(buf, "\n[nick: ");
            strcat(buf, username);
            strcat(buf, "] > ");
        }

        safe_printf(buf);
    }

    if (read_shouldexit())
        safe_printf("\r\033[0;31m[ERROR]: Terminating... (Press enter to proceed)\033[0m");
    
    return NULL;
}

void *sender(void *arg) {
    char buf[BUFLEN];
    while (!read_shouldexit()) {
        pthread_mutex_lock(&outplock);
        printf("[nick: %s] > ", username);
        fflush(stdout);
        pthread_mutex_unlock(&outplock);

        bzero(buf, BUFLEN);
        if (!fgets(buf, BUFLEN - 1, stdin))
            break;

        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) <= 0) strcpy(buf, "-");
        write_fd(sockfd, buf);
        sleep(1);
    }
    return NULL;
}

void safe_printf(const char *msg) {
    pthread_mutex_lock(&outplock);
    printf("%s", msg);
    fflush(stdout);
    pthread_mutex_unlock(&outplock);
}

void set_shouldexit(int val) {
    pthread_mutex_lock(&exitlock);
    shouldexit = val;
    pthread_mutex_unlock(&exitlock);
}

int read_shouldexit(void) {
    pthread_mutex_lock(&exitlock);
    int val = shouldexit;
    pthread_mutex_unlock(&exitlock);
    return val;
}

void cleanup_exit(int code) {
    set_shouldexit(1);

    if (sockfd >= 0) {
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    }

    pthread_mutex_destroy(&exitlock);
    pthread_mutex_destroy(&outplock);

    exit(code);
}

void sig_handler(int sig) {
    set_shouldexit(1);
    close(STDIN_FILENO);
    safe_printf("\r\033[0;34m[INFO]: Received SIGINT. Terminating...\033[0m\n");
}