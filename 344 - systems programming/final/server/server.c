#include "../common.h"

void *client_handler(void *);
void *upload_handler(void *);
void init_upload_queue(queue_t *);
void enqueue(queue_t *, file_t);
file_t dequeue(queue_t *);
int handle_command(client_t *, char *);
void handle_file_transfer(file_t);
char *generate_unique_name(const char *, const char *, const char *);
int is_name_valid(const char *, const int);
int does_usr_exist(const char *, const int);
void leave_room(client_t *);
void destroy_client(int);
void display_help(const int);
void display_users(const int);
void display_rooms(const int);
void setup_signal_handlers(void);
void start_upload_handlers(void);
void sig_handler(int);
void cleanup_upload_queue(queue_t *);
void cleanup_server_resources(void);

int sockfd, clicounter = 0, roomcounter = 0;
room_t *rooms[MAX_ROOMS];
client_t *clients[MAX_CLIENTS];
pthread_t *upload_threads = NULL;
int num_upload_threads = MAX_UPLOADS;
pthread_mutex_t clilock = PTHREAD_MUTEX_INITIALIZER;
volatile sig_atomic_t server_running = 1;

int main(int argc, char *argv[]) {
    int clisockfd, *newsockfd, portno, opt;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    pthread_t tid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    setup_signal_handlers();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("opening server socket\n");

    opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("setsockopt SO_REUSEADDR failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    bzero((char *) &servaddr, sizeof(servaddr));
    portno = atoi(argv[1]);
    if (portno <= 0 || portno > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", portno);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        close(sockfd);
        error("binding server failed\n");
    }

    if (listen(sockfd, MAX_CLIENTS) < 0) {
        close(sockfd);
        error("listen failed\n");
    }

    init_upload_queue(&uploadQue);
    start_upload_handlers();

    for (int i = 0; i < MAX_CLIENTS; i++) 
        clients[i] = NULL;

    for (int i = 0; i < MAX_ROOMS; i++) 
        rooms[i] = NULL;

        
    printf("-------------------------- Welcome to ZChat ---------------------------\n");
    printf("\033[0;34m[INFO]: Waiting for clients on port: %d\033[0m\n", portno);
    printf("-----------------------------------------------------------------------\n");
    
    clilen = sizeof(cliaddr);
    
    while (server_running) {
        clisockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
        
        if (!server_running) {
            if (clisockfd > 0) close(clisockfd);
            break;
        }

        if (clisockfd < 0) {
            if (errno == EINTR)
                continue;
            if (server_running)
                error("accept failed");
            break;
        }

        newsockfd = malloc(sizeof(int));
        if (!newsockfd) {
            close(clisockfd);
            error("malloc newsockfd\n");
        }

        *newsockfd = clisockfd;

        if (pthread_create(&tid, NULL, client_handler, (void *)newsockfd)) {
            free(newsockfd);
            close(clisockfd);
            error("creating client thread\n");
        }

        pthread_detach(tid);
    }

    cleanup_server_resources();
    close(sockfd);
    return 0;
}

void *client_handler(void *sockdesc) {
    char buf[BUFLEN], usrname[USRSIZ+1];
    int n, serversockfd;
    client_t *cli = NULL;

    serversockfd = *(int *)sockdesc;

    while (1) {
        bzero(usrname, USRSIZ+1);
        n = read(serversockfd, usrname, USRSIZ);
        if (n <= 0) {
            close(serversockfd);
            free(sockdesc);
            pthread_exit(NULL);
        } 
        usrname[n] = '\0';

        if (!is_name_valid(usrname, 0) || does_usr_exist(usrname, -1) >= 0)
            write_fd(serversockfd, "error");
        else
            break;
    }

    pthread_mutex_lock(&clilock);
    if (clicounter >= MAX_CLIENTS) {
        pthread_mutex_unlock(&clilock);
        write_fd(serversockfd, "full");
        close(serversockfd);
        free(sockdesc);
        pthread_exit(NULL);
    }

    cli = malloc(sizeof(client_t));
    if (!cli) {
        pthread_mutex_unlock(&clilock);
        close(serversockfd);
        free(sockdesc);
        pthread_exit(NULL);
    }

    cli->sockfd = serversockfd;
    strncpy(cli->usrname, usrname, USRSIZ);
    cli->usrname[USRSIZ] = '\0';
    cli->room[0] = '\0';

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = cli;
            clicounter++;
            break;
        }
    }
    pthread_mutex_unlock(&clilock);

    bzero(buf, BUFLEN);
    sprintf(buf, "@%s connected\n", usrname);
    write_fd(serversockfd, buf);
    printf("\033[0;32m[SUCCESS]: Client connected with username \"%s\"\033[0m\n", usrname);
    log_event("[SUCCESS]: Client connected with username \"%s\"", usrname);

    while ((n = read(serversockfd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';

        pthread_mutex_lock(&clilock);
        int found = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == cli) {
                found = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clilock);

        if (!found) break;

        if (strlen(buf) <= 0 || buf[0] != '/') {
            display_help(cli->sockfd);
            continue;
        } 
        
        if (!handle_command(cli, buf)) break;
    }

    pthread_mutex_lock(&clilock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && clients[i]->sockfd == serversockfd) {
            destroy_client(i);
            break;
        }
    }
    pthread_mutex_unlock(&clilock);

    printf("\033[0;34m[INFO]: Client disconnected\033[0m\n");
    log_event("[SUCCESS]: Client disconnected");

    free(sockdesc);
    pthread_exit(NULL);
}

void *upload_handler(void *arg) {
    while (1) {
        sem_wait(&uploadQue.items);
        sem_wait(&uploadQue.concurrent);

        file_t req = dequeue(&uploadQue);

        pthread_mutex_lock(&uploadQue.lock);
        uploadQue.currentuploads++;
        pthread_mutex_unlock(&uploadQue.lock);

        handle_file_transfer(req);

        pthread_mutex_lock(&uploadQue.lock);
        uploadQue.currentuploads--;
        pthread_mutex_unlock(&uploadQue.lock);

        sem_post(&uploadQue.concurrent);
    }

    return NULL;
}

void start_upload_handlers(void) {
    upload_threads = malloc(sizeof(pthread_t) * num_upload_threads);
    if (!upload_threads) {
        error("Failed to allocate memory for upload threads");
    }
    
    for (int i = 0; i < num_upload_threads; i++) {
        if (pthread_create(&upload_threads[i], NULL, upload_handler, NULL) != 0) {
            error("Failed to create upload handler thread");
            exit(EXIT_FAILURE);
        }
    }
}

void init_upload_queue(queue_t *q) {
    q->front = q->back = q->size = q->currentuploads = 0;
    pthread_mutex_init(&q->lock, NULL);
    sem_init(&q->items, 0, 0);
    sem_init(&q->slots, 0, MAX_QUEUE_CAP);
    sem_init(&q->concurrent, 0, 5);
}

void enqueue(queue_t *q, file_t req) {
    pthread_mutex_lock(&q->lock);
    int currsize = q->size;
    int active = q->currentuploads;
    pthread_mutex_unlock(&q->lock);

    char feedback[BUFSIZ];
    if (active >= 5) {
        int estimwait = (currsize + 1) * 2;
        snprintf(feedback, sizeof(feedback), 
            "\033[0;33m[SERVER]: Upload queue is full (%d active). Estimated wait time: %d seconds\033[0m\n", 
            active, estimwait);
        write_fd(req.sendersock, feedback);
    }

    sem_wait(&q->slots);

    pthread_mutex_lock(&q->lock);
    req.enqueue_time = time(NULL);
    req.status = 0;
    q->que[q->back] = req;
    q->back = (q->back + 1) % MAX_QUEUE_CAP;
    q->size++;
    pthread_mutex_unlock(&q->lock);

    sem_post(&q->items);

    snprintf(feedback, sizeof(feedback), 
        "\033[0;33m[SERVER]: File queued. Position in queue: %d\033[0m\n", 
        currsize + 1);
    log_event("[SERVER]: File queued. Position in queue: %d", currsize + 1);
    write_fd(req.sendersock, feedback);
}

file_t dequeue(queue_t *q) {
    pthread_mutex_lock(&q->lock);
    file_t req = q->que[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_CAP;
    q->size--;
    pthread_mutex_unlock(&q->lock);

    sem_post(&q->slots);

    return req;
}

int handle_command(client_t *cli, char *msg) {
    char buf[BUFLEN];
    bzero(buf, BUFLEN);

    if (strncmp(msg, "/join ", 6) == 0) {
        char *room = msg + 6;
        room[strcspn(room, "\n")] = 0;

        if (!is_name_valid(room, 1)) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Invalid room name\033[0m\n");
            return 1;
        }
        
        pthread_mutex_lock(&clilock);
        int room_ind = -1;
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i] && strcmp(rooms[i]->name, room) == 0) {  
                if (strcmp(cli->room, room) == 0) {
                    pthread_mutex_unlock(&clilock);
                    write_fd(cli->sockfd, "\033[0;33m[SERVER]: You are already in this room\033[0m\n");
                    return 1;
                }
                room_ind = i;
                break;
            }
        }

        if (room_ind == -1) {
            if (roomcounter >= MAX_ROOMS) {
                pthread_mutex_unlock(&clilock);
                write_fd(cli->sockfd, "\033[0;33m[SERVER]: Could not create the room since maximum number of rooms reached. Join one or try later\033[0m\n");
                return 1;
            }
            
            room_t *r = malloc(sizeof(room_t));
            if (!r) {
                pthread_mutex_unlock(&clilock);
                return 1;
            }

            r->capacity = 1;
            strncpy(r->name, room, ROMSIZ);
            r->name[ROMSIZ] = '\0';
            
            for (int i = 0; i < MAX_ROOMS; i++) {
                if (!rooms[i]) {
                    rooms[i] = r;
                    ++roomcounter;
                    printf("\033[0;34m[INFO]: Room created: %s\033[0m\n", r->name);
                    log_event("[SERVER]: Room created: %s", r->name);
                    break;
                }
            }

            log_event("[SERVER]: No room with name %s. Creating it...", r->name);
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: No room with this name. Creating it...\033[0m\n");
        } else {
            // check room capacity
            if (rooms[room_ind]->capacity >= MAX_ROOM_CAP) {
                pthread_mutex_unlock(&clilock);
                write_fd(cli->sockfd, "\033[0;33m[SERVER]: Room is full. Try later\033[0m\n");
                return 1;
            }
            rooms[room_ind]->capacity++;
        }

        if (cli->room[0] != '\0') {
            log_event("[SERVER]: %s switched from %s to %s", cli->usrname, cli->room, room);
            sprintf(buf, "\033[0;33m[SERVER]: Switched from %s to %s\033[0m\n", cli->room, room);
            leave_room(cli);
        } else {
            log_event("[SERVER]: %s joined %s", cli->usrname, room);
            sprintf(buf, "\033[0;33m[SERVER]: Joined the room %s\033[0m\n", room);
        }

        strncpy(cli->room, room, ROMSIZ);
        cli->room[ROMSIZ] = '\0';
        pthread_mutex_unlock(&clilock);
        
        write_fd(cli->sockfd, buf);
        printf("\033[0;34m[INFO]: %s joined room %s\033[0m\n", cli->usrname, cli->room);
    } else if (strncmp(msg, "/leave", 6) == 0) {
        pthread_mutex_lock(&clilock);
        if (cli->room[0] != '\0')
            leave_room(cli);
        else
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: You are not in any room\033[0m\n");
        pthread_mutex_unlock(&clilock);
    } else if (strncmp(msg, "/broadcast ", 11) == 0) {
        if (cli->room[0] == '\0') {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Join a room first\033[0m\n");
            return 1;
        }
        snprintf(buf, sizeof(buf), "\r[%s broadcasted]: %s\n", cli->usrname, msg + 11);
        pthread_mutex_lock(&clilock);
        for (int i = 0; i < MAX_CLIENTS; i++)
            if (clients[i] && clients[i]->sockfd != cli->sockfd && strcmp(clients[i]->room, cli->room) == 0)
                write_fd(clients[i]->sockfd, buf);
        pthread_mutex_unlock(&clilock);
        log_event("[SERVER]: %s sent a broadcast to room %s", cli->usrname, cli->room);
        write_fd(cli->sockfd, "\033[0;33m[SERVER]: Broadcast is sent\033[0m\n");
    } else if (strncmp(msg, "/whisper ", 9) == 0) {
        char *sp = strchr(msg + 9, ' ');
        if (!sp) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Invalid command. Run /help to see user manual\033[0m\n");
            return 1;
        }

        char target[USRSIZ+1];
        size_t target_len = sp - (msg + 9);
        strncpy(target, msg + 9, target_len);
        target[target_len] = '\0';
        int isval = is_name_valid(target, 0);
        if (!isval) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: No user found with this name\033[0m\n");
            return 1;
        }

        char *priv_msg = sp + 1;
        priv_msg[strcspn(priv_msg, "\n")] = '\0';
        log_event("%s whispered to %s", cli->usrname, target);
        snprintf(buf, sizeof(buf), "\r[%s whispered]: %s\n", cli->usrname, priv_msg);

        int ishere = does_usr_exist(target, cli->sockfd);
        if (ishere == -1) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: No user found with this name\033[0m\n");
        } else if (ishere == -2) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: You cannot send a message to yourself\033[0m\n");
        } else {
            write_fd(clients[ishere]->sockfd, buf);
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Private message sent\033[0m\n");     
        }
    } else if (strncmp(msg, "/sendfile ", 10) == 0) {
        char *sp = strchr(msg + 10, ' ');
        if (!sp) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Usage: /sendfile <filename> <username>\033[0m\n");
            return 1;
        }

        char filename[FILSIZ+1];
        size_t len = sp - (msg + 10);
        if (len >= FILSIZ) len = FILSIZ;
        strncpy(filename, msg + 10, len);
        filename[len] = '\0';

        char *pt = strchr(filename, '.');
        if (!pt) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: File cannot be sent. Provide an extension (.pdf, .txt, .png, .jpg)\033[0m\n");
            return 1;
        }
        
        *pt = '\0';
        char *ext = pt + 1;
        if (strcmp(ext, "txt") != 0 && strcmp(ext, "pdf") != 0 && strcmp(ext, "png") != 0 && strcmp(ext, "jpg") != 0) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: File cannot be sent. Only acceptable extensions: .pdf, .txt, .png, .jpg\033[0m\n");
            return 1;
        }

        char filepath[FILSIZ + 1];
        snprintf(filepath, sizeof(filepath), "./testfiles/%s.%s", filename, ext);

        FILE *fp = fopen(filepath, "rb");
        if (!fp) {
            snprintf(buf, sizeof(buf), "\033[0;33m[SERVER]: File not found at %s\033[0m\n", filepath);
            write_fd(cli->sockfd, buf);
            return 1;
        }

        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        fclose(fp);

        if (size > 3145728) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: Cannot send the file. File size limit: 3MB\033[0m\n");
            return 1;
        }
        
        char *target = sp + 1;
        target[strcspn(target, "\n")] = 0;
        if (!is_name_valid(target, 0)) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: No user found with this name\033[0m\n");
            return 1;
        }

        int ishere = does_usr_exist(target, cli->sockfd);
        if (ishere == -1) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: No user found with this name\033[0m\n");
        } else if (ishere == -2) {
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: You cannot send a file to yourself\033[0m\n");
        } else {
            file_t req;
            req.sendersock = cli->sockfd;
            req.receiversock = clients[ishere]->sockfd;
            strncpy(req.receiver, target, USRSIZ);
            req.receiver[USRSIZ] = '\0';
            strncpy(req.sender, cli->usrname, USRSIZ);
            req.sender[USRSIZ] = '\0';
            strncpy(req.path, filepath, BUFSIZ);  
            req.path[BUFSIZ] = '\0';  
            strncpy(req.name, filename, FILSIZ);  
            req.name[FILSIZ] = '\0';  
            strncpy(req.ext, ext, 4);  
            req.ext[3] = '\0';  
            enqueue(&uploadQue, req);
            log_event("[SERVER]: File %s.%s added to upload queue", req.name, req.ext);
            write_fd(cli->sockfd, "\033[0;33m[SERVER]: File added to upload queue\033[0m\n");     
        }
    } else if (strcmp(msg, "/help") == 0) {
        display_help(cli->sockfd);
    } else if (strcmp(msg, "/users") == 0) {
        display_users(cli->sockfd);
    } else if (strcmp(msg, "/rooms") == 0) {
        display_rooms(cli->sockfd);
    } else if (strcmp(msg, "/exit") == 0) {
        log_event("[SERVER]: %s disconnected", cli->usrname);
        write_fd(cli->sockfd, "\033[0;33m[SERVER]: Goodbye!\033[0m\n");
        return 0;
    } else if (strcmp(msg, "/queue_status") == 0) {
        pthread_mutex_lock(&uploadQue.lock);
        int queuesize = uploadQue.size;
        int active = uploadQue.currentuploads;
        pthread_mutex_unlock(&uploadQue.lock);
        
        snprintf(buf, sizeof(buf), "\033[0;33m[SERVER]: Queue Status - Active uploads: %d/5, Waiting in queue: %d\033[0m\n", active, queuesize);
        write_fd(cli->sockfd, buf);
    } else {
        write_fd(cli->sockfd, "\033[0;33m[SERVER]: Unknown command\033[0m\n");
        display_help(cli->sockfd);
    }

    return 1;
}

void handle_file_transfer(file_t req) {
    char buf[BUFSIZ];
    time_t start_time = time(NULL);
    time_t wait_time = start_time - req.enqueue_time;

    if (wait_time > 0) {
        snprintf(buf, sizeof(buf), 
            "\033[0;33m[SERVER]: Your file waited %ld seconds in queue\033[0m\n", wait_time);
        write_fd(req.sendersock, buf);
    }

    FILE *fp = fopen(req.path, "rb");
    if (!fp) {
        snprintf(buf, sizeof(buf), "\033[0;33m[SERVER]: File not found at %s\033[0m\n", req.path);
        write_fd(req.sendersock, buf);
        return;
    }

    char* uniquename = generate_unique_name(req.name, req.ext, req.receiver);

    snprintf(buf, sizeof(buf), "\033[0;33m[SERVER]: Uploading %s (processing time: %ld sec)...\033[0m\n",  uniquename, wait_time);
    write_fd(req.sendersock, buf);

    snprintf(buf, sizeof(buf), "\r\033[0;33m[SERVER]: Incoming file: %s from %s...\033[0m\n", uniquename, req.sender);
    write_fd(req.receiversock, buf);

    bzero(buf, BUFSIZ);
    size_t bytes;
    if (strcmp(req.ext, "txt") == 0) {
        write_fd(req.receiversock, "\r\033[0;33m[SERVER]: Displaying first 512 chars of file:\033[0m\n");
        while((bytes = fread(buf, 1, sizeof(buf), fp)) > 0)
            write(req.receiversock, buf, bytes);
    }

    fclose(fp);

    sleep(2);
    log_event("[SERVER]: File %s.%s is sent successfully from %s to %s", req.name, req.ext, req.sender, req.receiver);
    write_fd(req.sendersock, "\r\033[0;33m[SERVER]: File sent successfully\033[0m\n");
    write_fd(req.receiversock, "\n\033[0;33m[SERVER]: File received\033[0m\n");
}

char *generate_unique_name(const char *original, const char *ext, const char *receiver) {
    static char uniqueName[FILSIZ + 1];
    char testpath[BUFSIZ];
    int counter = 1;
    
    snprintf(uniqueName, sizeof(uniqueName), "%s.%s", original, ext);
    snprintf(testpath, sizeof(testpath), "./received_files/%s_%s", receiver, uniqueName);
    
    while (access(testpath, F_OK) == 0) {
        snprintf(uniqueName, sizeof(uniqueName), "%s_%d.%s", original, counter, ext);
        snprintf(testpath, sizeof(testpath), "./received_files/%s_%s", receiver, uniqueName);
        counter++;
    }
    
    return uniqueName;
}

int is_name_valid(const char *name, const int flag) {
    if (!name) return 0;
    int len = (flag == 0) ? USRSIZ : ROMSIZ; // username : room name
    if (strlen(name) > len || strlen(name) <= 0)
        return 0;

    for (int i = 0; i < strlen(name); i++)
        if (!isalnum(name[i]))
            return 0;
    
    return 1;
}

int does_usr_exist(const char *usrname, const int clisock) {
    pthread_mutex_lock(&clilock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] && strcmp(clients[i]->usrname, usrname) == 0) {
            pthread_mutex_unlock(&clilock);
            return (clients[i]->sockfd == clisock) ? -2 : i;
        }
    }
    pthread_mutex_unlock(&clilock);
    return -1;
}

void leave_room(client_t *cli) {
    if (!cli || cli->room[0] == '\0') return;

    char buf[64];
    log_event("[SERVER]: %s left the room %s", cli->usrname, cli->room);
    snprintf(buf, sizeof(buf), "\033[0;33m[SERVER]: Left the room %s\033[0m\n", cli->room);
    write_fd(cli->sockfd, buf);
    printf("\033[0;34m[INFO]: %s left room %s\033[0m\n", cli->usrname, cli->room);

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i] && strcmp(rooms[i]->name, cli->room) == 0) {
            rooms[i]->capacity--;
            if (rooms[i]->capacity <= 0) {
                log_event("[SERVER]: Room %s closed", rooms[i]->name);
                printf("\033[0;34m[INFO]: Room closed: %s\033[0m\n", rooms[i]->name);
                free(rooms[i]);
                rooms[i] = NULL;
                roomcounter--;
            }
            break;
        }
    }
    cli->room[0] = '\0';
}

void destroy_client(int ind) {
    if (ind < 0 || ind >= MAX_CLIENTS || !clients[ind])
        return;

    if (clients[ind]->sockfd != -1)
        close(clients[ind]->sockfd);
    
    if (clients[ind]->room[0] != '\0')
        leave_room(clients[ind]);

    free(clients[ind]);
    clients[ind] = NULL;
    clicounter--;
}

void display_help(const int fd) {
    const char *help = "\033[0;33m[SERVER]: Available commands:\n\t- /join <roomname>\t\t\tjoin/create a room\n\t- /leave\t\t\t\tleave the current room\n\t- /broadcast <msg>\t\t\tsend a message to everyone in a room\n\t- /whisper <usrname> <msg>\t\tsend private message\n\t- /sendfile <usrname> <filename>\tsend a file (MAX 3 Mb)\n\t- /queue_status\t\t\t\tdisplay current queue status\n\t- /help\t\t\t\t\tdisplay help\n\t- /users\t\t\t\tdisplay active users\n\t- /rooms\t\t\t\tdisplay active rooms\n\t- /exit\t\t\t\t\tdisconnect from the server\033[0m\n";
    write_fd(fd, help);
}

void display_users(const int fd) {
    pthread_mutex_lock(&clilock);
    write_fd(fd, "\033[0;33m[SERVER]: Currently active users:\033[0m\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            char usr[128];
            snprintf(usr, sizeof(usr), "\t\033[0;33m  # username: %s & room: %s\033[0m\n", clients[i]->usrname, (clients[i]->room[0] == '\0') ? "-" : clients[i]->room);
            write_fd(fd, usr);
        }
    }
    pthread_mutex_unlock(&clilock);
}

void display_rooms(int fd) {
    char rline[BUFLEN], uline[BUFLEN];

    pthread_mutex_lock(&clilock);
    if (roomcounter <= 0) {
        pthread_mutex_unlock(&clilock);
        write_fd(fd, "\033[0;33m[SERVER]: No active room found\033[0m\n");
        return;
    }

    write_fd(fd, "\033[0;33m[SERVER]: Currently active rooms:\033[0m");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i]) {
            bzero(rline, BUFLEN); 
            snprintf(rline, sizeof(rline), "\n\t\033[0;33m  # room: %s & capacity: %d/%d\n\t   |_ users:\033[0m\n", rooms[i]->name, rooms[i]->capacity, MAX_ROOM_CAP);
            write_fd(fd, rline);
            for (int j = 0; j < MAX_CLIENTS; j++) {
                if (clients[j] && strcmp(clients[j]->room, rooms[i]->name) == 0) {
                    bzero(uline, BUFLEN);
                    snprintf(uline ,sizeof(uline), "\t\033[0;33m     |_ @%s\033[0m\n", clients[j]->usrname);
                    write_fd(fd, uline);
                }
            }
        }
    }
    pthread_mutex_unlock(&clilock);
}

void setup_signal_handlers(void) {
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);   
    sigaction(SIGTERM, &sa, NULL);  
    sigaction(SIGQUIT, &sa, NULL);  
    
    signal(SIGPIPE, SIG_IGN);
}

void sig_handler(int sig) {
    pthread_mutex_lock(&clilock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            write_fd(clients[i]->sockfd, "shutdown");
            shutdown(clients[i]->sockfd, SHUT_RDWR);
            destroy_client(i);
        }
    }    
    pthread_mutex_unlock(&clilock);
    
    log_event("[SERVER]: CTRL+C received. Shutting down...");
    printf("\r\033[0;34m[INFO]: CTRL+C received. Shutting down...\033[0m\n");
    close(sockfd);
    exit(0);
}

void cleanup_upload_queue(queue_t *q) {
    printf("\033[0;34m[INFO]: Cleaning up upload queue...\033[0m\n");
    
    pthread_mutex_lock(&q->lock);
    int active = q->currentuploads;
    pthread_mutex_unlock(&q->lock);
    
    if (active > 0) {
        printf("\033[0;34m[INFO]: Waiting for %d active uploads to complete...\033[0m\n", active);
        while (active > 0) {
            sleep(1);
            pthread_mutex_lock(&q->lock);
            active = q->currentuploads;
            pthread_mutex_unlock(&q->lock);
        }
    }
    
    if (sem_destroy(&q->items) != 0) 
        error("Failed to destroy items semaphore");

    if (sem_destroy(&q->slots) != 0) 
        error("Failed to destroy slots semaphore");

    if (sem_destroy(&q->concurrent) != 0)
        error("Failed to destroy concurrent semaphore");
    
    if (pthread_mutex_destroy(&q->lock) != 0) 
        error("Failed to destroy queue mutex");
    
    printf("\033[0;34m[INFO]: Upload queue cleanup completed\033[0m\n");
}

void cleanup_server_resources(void) {
    printf("[INFO]: Cleaning up server resources...\n");
    
    if (upload_threads) {
        printf("[INFO]: Stopping upload handler threads...\n");
        for (int i = 0; i < num_upload_threads; i++) {
            pthread_cancel(upload_threads[i]);
            pthread_join(upload_threads[i], NULL);
        }
        free(upload_threads);
        upload_threads = NULL;
    }
    
    cleanup_upload_queue(&uploadQue);
    
    pthread_mutex_lock(&clilock);
    printf("[INFO]: Closing client connections...\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            close(clients[i]->sockfd);
            free(clients[i]);
            clients[i] = NULL;
        }
    }
    clicounter = 0;
    pthread_mutex_unlock(&clilock);
    
    printf("[INFO]: Cleaning up chat rooms...\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i]) {
            free(rooms[i]);
            rooms[i] = NULL;
        }
    }
    roomcounter = 0;
    
    if (sockfd > 0) {
        close(sockfd);
        sockfd = -1;
    }
    
    pthread_mutex_destroy(&clilock);
    
    printf("[INFO]: Server cleanup completed\n");
}
