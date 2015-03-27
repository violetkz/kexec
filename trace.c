/*
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 */
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
        
    fprintf(fd_log, "%s ", prefix);
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
