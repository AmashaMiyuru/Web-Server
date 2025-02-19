#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void log_message(const char *format, ...) {
    FILE *log_file = fopen("./log/server.log", "a");
    if (!log_file) return;

    va_list args;
    va_start(args, format);

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_file, "[%s] ", time_str);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");

    va_end(args);
    fclose(log_file);
}
