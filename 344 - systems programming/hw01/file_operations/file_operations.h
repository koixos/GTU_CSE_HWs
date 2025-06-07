#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

const int create_directory(const char *);
const int create_file(const char *);
const int delete_directory(const char *);
const int delete_file(const char *);
const int list(const char *);
const int list_by_extension(const char *, const char *);
const int read_file(const char *);
const int append_file(const char *, const char *);
const int show_logs(void);

// helper function(s)
const char *get_file_details(const char *, struct dirent *);

#endif