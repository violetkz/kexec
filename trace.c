#include "trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>


static int fd_log_desc = STDERR_FILENO;

void log_set_desc(FILE* fp) {
    //fd_log_desc = fp;
}

void dlog(char *fmt, ...) {
    
    fprintf(stderr, "[DEBUG]");

    va_list ap;    
    va_start(ap, fmt);
    //vfprintf(fd_log_desc, fmt, ap);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}
