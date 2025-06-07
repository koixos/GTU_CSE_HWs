#include "file_operations.h"
#include "../common.h"
#include "../logger/logger.h"

const int create_directory(const char *dirName) {
    const char *op = "CREATE DIRECTORY";
    char details[MAX_LENGTH];

    if (OP_SUCCESS == access(dirName, F_OK)) {
        strcpy(details, "Directory already exists: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    if (OP_SUCCESS != mkdir(dirName, DEF_DIR_MODE)) {
        strcpy(details, "Failed to create directory: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }
    
    strcpy(details, "Directory created: ");
    strcat(details, dirName);
    strcat(details, "\n");

    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int create_file(const char *fileName) {
    const char *op = "CREATE FILE";
    char details[MAX_LENGTH];

    if (OP_SUCCESS == access(fileName, F_OK)) {
        strcpy(details, "File already exists: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    int fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    const char *timeStr = get_timestamp();
    write(fd, timeStr, strlen(timeStr));
    write(fd, "\n", strlen("\n"));
    close(fd);

    strcpy(details, "File created: ");
    strcat(details, fileName);
    strcat(details, "\n");

    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int delete_directory(const char *dirName) {
    const char *op = "DELETE DIRECTORY";
    char details[MAX_LENGTH];

    if (OP_SUCCESS != access(dirName, F_OK)) {
        strcpy(details, "Directory not found: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    if (OP_SUCCESS != rmdir(dirName)) {
        strcpy(details, "Failed to delete directory: ");
        if (ENOTEMPTY == errno)
            strcat(details, "(directory not empty) ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    strcpy(details, "Directory deleted: ");
    strcat(details, dirName);
    strcat(details, "\n");

    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int delete_file(const char *fileName) {
    const char *op = "DELETE FILE";
    char details[MAX_LENGTH];

    if (OP_SUCCESS != access(fileName, F_OK)) {
        strcpy(details, "File not found: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    if (OP_SUCCESS != unlink(fileName)) {
        strcpy(details, "Failed to delete file: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    strcpy(details, "File deleted: ");
    strcat(details, fileName);
    strcat(details, "\n");
    
    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int list(const char *dirName) {
    const char *op = "LIST DIRECTORY";
    char details[MAX_LENGTH];
    struct dirent *entry;
    char path[PATH_MAX_LENGTH];

    if (OP_SUCCESS != access(dirName, F_OK)) {
        strcpy(details, "Directory not found: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    DIR *dir = opendir(dirName);
    if (NULL == dir) {
        strcpy(details, "Failed to open directory: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    strcpy(details, "Directory contents of: ");
    strcat(details, dirName);
    strcat(details, "\n");
    strcat(details, "---------------------------\n");
    write(STDERR_FILENO, details, strlen(details));

    while (NULL != (entry = readdir(dir))) {
        if (OP_SUCCESS == strcmp(entry->d_name, ".") || OP_SUCCESS == strcmp(entry->d_name, ".."))
            continue;

        strcpy(path, dirName);
        strcat(path, "/");
        strcat(path, entry->d_name);

        const char *entryDetails = get_file_details(path, entry);    
        if (NULL == entryDetails)
            continue;  
        write(STDOUT_FILENO, entryDetails, strlen(entryDetails));
    }
    closedir(dir);

    write(STDOUT_FILENO, "---------------------------\n", strlen("---------------------------\n"));
    strcpy(details, "Directory listed successfully: ");
    strcat(details, dirName);
    strcat(details, "\n");
    
    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int list_by_extension(const char *dirName, const char *ext) {
    const char *op = "LIST BY EXTENSION";
    char details[MAX_LENGTH];
    struct dirent *entry;
    char path[PATH_MAX_LENGTH];

    if (OP_SUCCESS != access(dirName, F_OK)) {
        strcpy(details, "Directory not found: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    if ('.' == ext[0])
        ++ext;

    DIR *dir = opendir(dirName);
    if (NULL == dir) {
        strcpy(details, "Failed to open directory: ");
        strcat(details, dirName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    strcpy(details, "Files with extension: ");
    strcat(details, ext);
    strcat(details, "\n");
    strcat(details, "---------------------------\n");
    write(STDERR_FILENO, details, strlen(details));

    while (NULL != (entry = readdir(dir))) {
        if (OP_SUCCESS == strcmp(entry->d_name, ".") || OP_SUCCESS == strcmp(entry->d_name, ".."))
            continue;

        const char *fileExt = strrchr(entry->d_name, '.');
        if (NULL == fileExt)
            continue;
        ++fileExt;

        if (OP_SUCCESS == strcmp(fileExt, ext)) {
            strcpy(path, dirName);
            strcat(path, "/");
            strcat(path, entry->d_name);
        
            const char *entryDetails = get_file_details(path, entry);
            if (NULL == entryDetails)
                continue;
            write(STDOUT_FILENO, entryDetails, strlen(entryDetails));
        }
    }
    closedir(dir);

    write(STDOUT_FILENO, "---------------------------\n", strlen("---------------------------\n"));
    strcpy(details, "Directory searched successfully (");
    strcat(details, dirName);
    strcat(details, ") with extension: ");
    strcat(details, ext);
    strcat(details, "\n");
    
    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int read_file(const char *fileName) {
    const char *op = "READ FILE";
    char details[MAX_LENGTH];

    if (OP_SUCCESS != access(fileName, F_OK)) {
        strcpy(details, "File not found: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    int fd = open(fileName, O_RDONLY);

    int bufferSize = MAX_LENGTH;
    char *buffer = (char *)malloc(bufferSize * sizeof(char));
    if (NULL == buffer) {
        write(STDERR_FILENO, "Failed to allocate memory\n", strlen("Failed to allocate memory\n"));
        close(fd);
        return EXIT_FAIL;
    }

    ssize_t totalReadBytes = 0;
    while (1) {
        ssize_t readBytes = read(fd, buffer+totalReadBytes, bufferSize-totalReadBytes);
        if (0 > readBytes) {
            write(STDERR_FILENO, "Failed to read file\n", strlen("Failed to read file\n"));
            free(buffer);
            close(fd);
            return EXIT_FAIL;
        } else if (0 == readBytes) {
            break;
        } else {
            totalReadBytes += readBytes;
            if (totalReadBytes >= bufferSize) {
                bufferSize += MAX_LENGTH;
                char *temp = (char *)realloc(buffer, bufferSize);
                if (NULL == temp) {
                    write(STDERR_FILENO, "Failed to reallocate memory\n", strlen("Failed to reallocate memory\n"));
                    free(buffer);
                    close(fd);
                    return EXIT_FAIL;
                }
                buffer = temp;
            }
        }
    }

    buffer[totalReadBytes] = '\0';
    close(fd);

    write(STDOUT_FILENO, "\n", strlen("\n"));
    write(STDOUT_FILENO, buffer, totalReadBytes);
    free(buffer);

    strcpy(details, "File read: ");
    strcat(details, fileName);
    strcat(details, "\n");
    
    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int append_file(const char *fileName, const char *content) {
    const char *op = "APPEND FILE";
    char details[MAX_LENGTH];
    struct flock fl;

    if (NULL == content) {
        strcpy(details, "Content cannot be NULL\n");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    if (OP_SUCCESS != access(fileName, F_OK)) {
        strcpy(details, "File not found: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_WARNING, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    }

    int fd = open(fileName, O_WRONLY | O_APPEND);
    
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    if (-1 == fcntl(fd, F_SETLKW, &fl)) {
        strcpy(details, "Failed to lock file: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    ssize_t written = write(fd, content, strlen(content));
    if (written != (ssize_t)strlen(content)) {
        strcpy(details, "Failed to write to file: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));

        fl.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &fl);
        close(fd);
        return EXIT_FAIL;
    }

    write(fd, "\n", strlen("\n"));

    fl.l_type = F_UNLCK;
    if (-1 == fcntl(fd, F_SETLK, &fl)) {
        strcpy(details, "Failed to lock file: ");
        strcat(details, fileName);
        strcat(details, "\n");
        write(STDERR_FILENO, details, strlen(details));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, details))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }

    close(fd);

    strcpy(details, "File appended: ");
    strcat(details, fileName);
    strcat(details, "\n");
    
    if (EXIT_FAIL == write_log(LOG_LEV_SUCCESS, op, details)) {
        write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_WARNING;
    };

    return EXIT_SUCCESSFUL;
}

const int show_logs(void) {
    const char *op = "SHOW LOGS";
    const char *errMsg = "Failed to show logs\n";
    if (EXIT_FAIL == read_file(DEF_LOG_PATH)) {
        write(STDERR_FILENO, errMsg, strlen(errMsg));
        if (EXIT_FAIL == write_log(LOG_LEV_ERROR, op, errMsg))
            write(STDERR_FILENO, LOG_FAILED, strlen(LOG_FAILED));
        return EXIT_FAIL;
    }
    return EXIT_SUCCESSFUL;
}

// HELPER FUNCTION(S)
const char *get_file_details(const char *path, struct dirent *entry) {
    struct stat statBuf;
    if (-1 == stat(path, &statBuf))
        return NULL;
    char *type = S_ISDIR(statBuf.st_mode) ? "d" : "-";
    char perms[11];
    strcpy(perms, type);
    strcat(perms, ((statBuf.st_mode & S_IRUSR) ? "r" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IWUSR) ? "w" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IXUSR) ? "x" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IRGRP) ? "r" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IWGRP) ? "w" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IXGRP) ? "x" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IROTH) ? "r" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IWOTH) ? "w" : "-"));
    strcat(perms, ((statBuf.st_mode & S_IXOTH) ? "x" : "-"));
    
    char *entryDetails = (char *)malloc((PATH_MAX_LENGTH + MAX_LENGTH) * sizeof(char));
    strcpy(entryDetails, perms);
    strcat(entryDetails, " ");
    strcat(entryDetails, entry->d_name);
    strcat(entryDetails, "\n");

    return entryDetails;
}