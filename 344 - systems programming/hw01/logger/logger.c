#include "logger.h"
#include "../common.h"

const int write_log(const LogLevel lev, const char *op, const char *details) {
    int fd = open(DEF_LOG_PATH, O_WRONLY | O_CREAT | O_APPEND, DEF_FILE_MODE);

    const char *timeStr = get_timestamp();
    const char *logLev = get_log_lev_str(lev);

    // LOG FORMAT = [timestamp] status: operation - details 
    write(fd, "[", strlen("["));
    write(fd, timeStr, strlen(timeStr));
    write(fd, "] ", strlen("] "));
    write(fd, logLev, strlen(logLev));
    write(fd, ": ", strlen(": "));
    write(fd, op, strlen(op));
    write(fd, " - ", strlen(" - "));
    write(fd, details, strlen(details));
    write(fd, "\n", strlen("\n"));
    
    close(fd);

    return EXIT_SUCCESSFUL;
}

const char *get_log_lev_str(const LogLevel lev) {
    switch (lev) {
        case LOG_LEV_INFO:      return "INFO";
        case LOG_LEV_SUCCESS:   return "SUCCESS";
        case LOG_LEV_WARNING:   return "WARNING";
        case LOG_LEV_ERROR:     return "ERROR";
        default:                return "UNKNOWN";
    }    
}

const char *get_timestamp(void) {
    time_t now = time(NULL);
    char *timeStr = (char *)malloc(TIME_STR_SIZE * sizeof(char));
    struct tm *tm_info = localtime(&now);
    strftime(timeStr, TIME_STR_SIZE, "%Y-%m-%d %H:%M:%S", tm_info);
    return timeStr;
}