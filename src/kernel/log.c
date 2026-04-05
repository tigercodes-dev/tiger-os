#include "log.h"
#include "stdio.h"
#include <stdarg.h>

void print_level(LogLevel level) {
    switch (level) {
        case DEBUG:
            fputs("\e[1m[DEBUG]:\e[0m ", STDDBG);
            break;
        case INFO:
            fputs("\e[36;1m[INFO]:\e[0m ", STDDBG);
            break;
        case WARN:
            fputs("\e[33;1m[WARN]:\e[0m ", STDDBG);
            break;
        case ERROR:
            fputs("\e[91;1m[ERROR]:\e[0m ", STDDBG);
            break;
        case CRITICAL:
            fputs("\e[31;1m[CRITICAL]:\e[0m ", STDDBG);
            break;
    }
}

void log(LogLevel level, const char* s) {
    print_level(level);
    fputs(s, STDDBG);
    fputc('\n', STDDBG);
}

void logf(LogLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_level(level);
    vfprintf(STDDBG, fmt, args);
    fputc('\n', STDDBG);
    va_end(args);
}