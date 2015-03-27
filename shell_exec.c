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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "shell_exec.h"

/* Execute the command using this shell program.  */
#define SHELL "/bin/sh"
#define PIPE_READ_PORT  0
#define PIPE_WRITE_PORT 1

int shell_exec (const char *command, char **out)
{
    int status;
    pid_t pid;
    int  stdout_pipe[2];

    if (pipe(stdout_pipe) < 0) {
        return -1;
    }
    pid = fork ();
    if (pid == 0) {
        /* This is the child process.*/
        close(stdout_pipe[PIPE_READ_PORT]);
        dup2(stdout_pipe[PIPE_WRITE_PORT],STDOUT_FILENO);
        dup2(stdout_pipe[PIPE_WRITE_PORT],STDERR_FILENO);
        /*Execute the shell command. */
        execl (SHELL, SHELL, "-c", command, NULL);
        _exit (EXIT_FAILURE);

    }
    else if (pid < 0) {
        /* The fork failed.  Report failure.  */
        status = -1;
    }
    else { /* This is the parent process. */
        close(stdout_pipe[PIPE_WRITE_PORT]);
        
        if (out) { /* read the output to buf */
            int bufsize = 4096;
            char *buf = (char *)malloc(bufsize);
            if (!buf) return -2;
            FILE *fd = fdopen(stdout_pipe[PIPE_READ_PORT], "r");
            int  sum = 0;
            while(!feof(fd) ) {
                if(sum + 128 > bufsize ) {
                    bufsize *= 2;
                    char *newbuf = (char*)realloc(buf, bufsize);
                    if (!newbuf) { free(buf); return  -3; }
                    buf = newbuf;
                }
                size_t t = fread(buf + sum , 1, 128, fd); 
                sum += t;
            }
            *(buf + sum) = '\0'; 
            *out = buf;
            
            fclose(fd);
        }

        /*Wait for the child to complete.  */
        if (waitpid (pid, &status, 0) != pid) {
            status = -1;
        }
        else { /* handle the child exited status */
            int ret; 
            if (WIFEXITED(status))        { ret = WEXITSTATUS(status); }
            else if (WIFSIGNALED(status)) { ret = -2; }
            else if (WIFSTOPPED(status))  { ret = -3; }
            else                          { ret = -4; }
            status = ret;
        }
    }
    return status;
}

#undef SHELL
#undef PIPE_READ_PORT
#undef PIPE_WRITE_PORT
