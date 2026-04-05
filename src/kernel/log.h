#ifndef _LOG_H_
#define _LOG_H_

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
} LogLevel;

void log(LogLevel level, const char* s);
void logf(LogLevel level, const char* fmt, ...);

#define puts_dbg(s) log_dbg(DEBUG, s);
#define printf_dbg(fmt, ...) logf_dbg(DEBUG, __VA_ARGS__);

#endif
