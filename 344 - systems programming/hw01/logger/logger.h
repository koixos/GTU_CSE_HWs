#ifndef LOGGER_H
#define LOGGER_H

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define LOG_FAILED "WARNING: could not write to the log file\n"

typedef enum {
    LOG_LEV_INFO,
    LOG_LEV_SUCCESS,
    LOG_LEV_WARNING,
    LOG_LEV_ERROR
} LogLevel;

const int write_log(LogLevel, const char *, const char *);
const char *get_log_lev_str(const LogLevel);
const char *get_timestamp(void);

#endif