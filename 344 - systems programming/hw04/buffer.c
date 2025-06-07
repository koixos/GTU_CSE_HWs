#include "buffer.h"
#include <string.h>
#include <stdlib.h>

extern volatile int keep_running;

Buffer *initialize(int buffer_size) {
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    if (buffer == NULL) return NULL;

    buffer->lines = (char **)malloc(buffer_size * sizeof(char *));
    if (buffer->lines == NULL) {
        free(buffer);
        return NULL;
    }

    for (int i = 0; i < buffer_size; i++)
        buffer->lines[i] = NULL;

    buffer->size = buffer_size;
    buffer->in = 0;
    buffer->out = 0;
    buffer->counter = 0;
    buffer->is_eof = 0;

    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->empty, NULL);
    pthread_cond_init(&buffer->full, NULL);

    return buffer;
}

int add_line(Buffer *buffer, const char *line) {
    pthread_mutex_lock(&buffer->mutex);

    while (buffer->counter == buffer->size && keep_running)
        pthread_cond_wait(&buffer->empty, &buffer->mutex);

    if (!keep_running) {
        pthread_mutex_unlock(&buffer->mutex);
        return 0;
    }

    buffer->lines[buffer->in] = strdup(line);
    if (buffer->lines[buffer->in] == NULL) {
        pthread_mutex_unlock(&buffer->mutex);
        return 0;
    }

    buffer->in = (buffer->in + 1) % buffer->size;
    buffer->counter++;

    pthread_cond_signal(&buffer->full);
    pthread_mutex_unlock(&buffer->mutex);

    return 0;
}

char *get_line(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);

    while (!buffer->is_eof && buffer->counter == 0 && keep_running)
        pthread_cond_wait(&buffer->full, &buffer->mutex);

    if (!keep_running) {
        pthread_mutex_unlock(&buffer->mutex);
        return NULL;
    }

    if (buffer->is_eof && buffer->counter == 0) {
        pthread_mutex_unlock(&buffer->mutex);
        return NULL;
    }

    char *line = buffer->lines[buffer->out];
    buffer->lines[buffer->out] = NULL;

    buffer->out = (buffer->out + 1) % buffer->size;
    buffer->counter--;

    pthread_cond_signal(&buffer->empty);
    pthread_mutex_unlock(&buffer->mutex);

    return line;
}

void set_eof(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->is_eof = 1;
    pthread_cond_broadcast(&buffer->full);
    pthread_mutex_unlock(&buffer->mutex);
}

int is_done(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    int done = buffer->is_eof && buffer->counter == 0;
    pthread_mutex_unlock(&buffer->mutex);
    return done;
}

void destroy(Buffer *buffer) {
    if (buffer == NULL) return;

    for (int i = 0; i < buffer->size; i++)
        if (buffer->lines[i] != NULL)
            free(buffer->lines[i]);

    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->empty);
    pthread_cond_destroy(&buffer->full);

    free(buffer->lines);
    free(buffer);
}