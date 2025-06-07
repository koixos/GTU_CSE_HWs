#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex; 
    pthread_cond_t empty; 
    pthread_cond_t full;

    char **lines;
    int size;
    int in;
    int out;
    int counter;
    int is_eof;
} Buffer;

Buffer *initialize(int);
int add_line(Buffer *, const char *);
char *get_line(Buffer *);
void set_eof(Buffer *);
int is_done(Buffer *);
void destroy(Buffer *);

#endif //BUFFER_H