#define _POSIX_C_SOURCE 200112L

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int id;
} Worker;

volatile int keep_running = 1;

Buffer *buffer;
int num_workers, *matches;
char *search_term;

pthread_t *workers, manager;
pthread_barrier_t barrier;

void handle_sigint(int sig)
{
    signal(sig, SIG_IGN);
    printf("\nReceived signal %d. Do you want to stop the program? (y/n): ", sig);
    fflush(stdout);
    
    char c = getchar();
    if (c == 'y' || c == 'Y') {
        keep_running = 0;
        if (buffer) {
            pthread_mutex_lock(&buffer->mutex);
            pthread_cond_broadcast(&buffer->empty);
            pthread_cond_broadcast(&buffer->full);
            pthread_mutex_unlock(&buffer->mutex);
        }
    } else {
        printf("Continuing...\n");
        signal(sig, handle_sigint);
        return;
    }
}

void *fmanager(void *arg) {
    FILE *fd = (FILE *)arg;
    char *line = NULL;
    ssize_t len = 0;

    while (keep_running) {
        ssize_t read = getline(&line, &len, fd);
        if (read == -1) {
            set_eof(buffer);
            break;
        }

        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        if (!add_line(buffer, line) && !keep_running) break;
    }
    
    free(line);
    pthread_exit(NULL);
}

void *fworker(void *arg) {
    Worker *w = (Worker *)arg;
    int counter = 0, id = w->id;

    while (keep_running) {
        char *line = get_line(buffer);
        if (line == NULL) break;

        if (strstr(line, search_term) != NULL) {
            printf("Worker %d found match: %s\n", id, line);
            ++counter;
        }

        free(line);
    }

    matches[id] = counter;

    pthread_barrier_wait(&barrier);

    if (id == 0) {
        int total = 0;
        printf("\n--- Results ---\n");
        for (int i = 0; i < num_workers; i++) {
            printf("Worker %d: Found %d matches\n", i, matches[i]);
            total += matches[i];
        }
        printf("Total matches: %d\n", total);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr, "Usage: ./LogAnalyzer <buffer_size> <num_workers> <log_file> <search_term>\n");
        return 1;
    }

    int buffer_size = atoi(argv[1]);
    if (buffer_size <= 0) {
        fprintf(stderr, "Error: buffer_size must be a positive integer.\n");
        return 1;
    }
    
    num_workers = atoi(argv[2]);
    if (buffer_size <= 0 || num_workers <= 0) {
        fprintf(stderr, "Error: buffer_size and num_workers must be positive integers.\n");
        return 1;
    }

    FILE *fd = fopen(argv[3], "r");
    if (fd == NULL) {
        fprintf(stderr, "Error: Could not open log file %s.\n", argv[3]);
        return 1;
    }

    search_term = argv[4];
    if (strlen(search_term) == 0) {
        fprintf(stderr, "Error: search_term cannot be empty.\n");
        fclose(fd);
        return 1;
    }

    printf("\t- LogAnalyzer -\n");
    printf("\n\t# Buffer size: %d\n", buffer_size);
    printf("\n\t# Worker count: %d\n", num_workers);
    printf("\n\t# Log file: %s\n", argv[3]);
    printf("\n\t# Search term: \"%s\"\n\n", search_term);

    buffer = initialize(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for buffer.\n");
        fclose(fd);
        return 1;
    }

    workers = (pthread_t *)malloc(num_workers * sizeof(pthread_t));
    Worker *w = (Worker *)malloc(num_workers * sizeof(Worker));
    matches = (int *)calloc(num_workers, sizeof(int));

    if (workers == NULL || w == NULL || matches == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for workers.\n");
        destroy(buffer);
        fclose(fd);
        return 1;
    }

    pthread_barrier_init(&barrier, NULL, num_workers);

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGQUIT, handle_sigint);
    signal(SIGHUP, handle_sigint);

    if (pthread_create(&manager, NULL, fmanager, (void *)fd) != 0) {
        fprintf(stderr, "Error: Failed to create manager thread.\n");
        goto cleanup;
    }

    for (int i = 0; i < num_workers; i++) {
        w[i].id = i;
        if (pthread_create(&workers[i], NULL, fworker, &w[i]) != 0) {
            fprintf(stderr, "Error: Failed to create worker thread %d\n", i);
            goto cleanup;
        }
    }

    pthread_join(manager, NULL);

    for (int i = 0; i < num_workers; i++)
        pthread_join(workers[i], NULL);

cleanup:
    pthread_barrier_destroy(&barrier);
    destroy(buffer);
    free(workers);
    free(w);
    free(matches);
    fclose(fd);
    
    return 0;
}