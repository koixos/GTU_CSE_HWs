#ifndef COMMON_H
#define COMMON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>

#define BUFLEN 512
#define USRSIZ 16
#define ROMSIZ 32
#define FILSIZ 64
#define MAX_CLIENTS 30
#define MAX_ROOMS    15
#define MAX_UPLOADS 3
#define MAX_ROOM_CAP 15
#define MAX_QUEUE_CAP 2

typedef struct {
    int sockfd;
    char usrname[USRSIZ+1];
    char room[ROMSIZ+1];
} client_t;

typedef struct {
    int capacity;
    char name[ROMSIZ+1];
} room_t;

typedef struct {
    char receiver[USRSIZ + 1];
    char sender[USRSIZ + 1];
    char path[BUFSIZ+1];
    char name[FILSIZ+1];
    char ext[4];
    int sendersock;
    int receiversock;
    int status; // 0 = waiting, 1 = in progress, 2 = completed
    time_t enqueue_time;
} file_t;

typedef struct {
    file_t que[MAX_QUEUE_CAP];
    int back;
    int front;
    int size;
    int currentuploads;
    pthread_mutex_t lock;
    sem_t items;
    sem_t slots;
    sem_t concurrent;
} queue_t;

queue_t uploadQue;
pthread_mutex_t loglock = PTHREAD_MUTEX_INITIALIZER;

void error(const char *msg) {
    char *newmsg = strcat("[ERROR]: ", msg);
    perror(newmsg);
    exit(1);
}

void log_event(const char *fmt, ...) {
    FILE *f;
    time_t now;
    char timestamp[64];

    pthread_mutex_lock(&loglock);

    f = fopen("server.log", "a");
    if (!f) {
        perror("log file open");
        pthread_mutex_unlock(&loglock);
        return;
    }

    time(&now);
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", localtime(&now));
    fprintf(f, "%s ", timestamp);

    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);

    pthread_mutex_unlock(&loglock);
}

void write_fd(const int fd, const char *msg) {
    if (fd >= 0 && msg) {
        write(fd, msg, strlen(msg));
        fsync(fd);
    }
}

#endif //COMMON_H