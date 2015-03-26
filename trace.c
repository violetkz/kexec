#include "trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>


//static int fd_log_desc = STDERR_FILENO;
FILE* fd_log_desc = NULL;

void log_set_desc(FILE* fp) {
    fd_log_desc = fp;
}

static void vlog(char* prefix, char* fmt, va_list ap) {
    FILE* fd_log =  (fd_log_desc != NULL) ? fd_log_desc : stderr;
        
    fprintf(fd_log, prefix);
    vfprintf(fd_log, fmt, ap);
    fprintf(fd_log, "\n");
    
    //due to share this between parent process and chile process, 
    //so has to disable the cache to avoid lose message.
    fflush(fd_log);
}

void dlog(char *fmt, ...) {
#ifdef __debug__ 
    va_list ap;    
    va_start(ap, fmt);
    char prefix[16];
    snprintf(prefix, 16, "L[%c] P[%d]: ",'D',getpid());
    vlog(prefix, fmt, ap);
    va_end(ap);
#endif
}
