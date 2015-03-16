#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "prog_exec.h"
#include "dynamic_buffer.h"
#include "shlex.h"
#include "trace.h"

#define SHELL "/bin/sh"
#define PIPE_READ_PORT  0
#define PIPE_WRITE_PORT 1


struct prog_context {
    int state;
    pid_t child_pid;
    int     fd_in;
    int     fd_out;
    int     fd_err;

    unsigned char  child_io_stdin_pipe:2; 
    unsigned char  child_io_stdout_pipe:2; 
    unsigned char  child_io_stderr_pipe:2; 
    unsigned char  child_exec_with_shell:2; 
};

struct prog_context *progcnx_new() {
    struct prog_context *n = 
        (struct prog_context *) malloc (sizeof(struct prog_context));
    if (n) {
        memset(n, 0x00, sizeof(struct prog_context));
        n->fd_in = n->fd_out = n->fd_err  = -1;
        n->child_io_stdin_pipe = 1;
        n->child_io_stdout_pipe = 1;
        n->child_io_stderr_pipe = 1;
        /* init with -2 to avoid misuse, see `man 2 kill` */
        n->child_pid = -2;
    }
    return n;
}

void progcnx_free(struct prog_context *cnt) {
    if (cnt) {
        free(cnt);
    }
}

FILE *progcnx_get_stdin(struct prog_context *cnt) {
    return (cnt && cnt->fd_in != -1) ?  fdopen(cnt->fd_in, "w") : NULL;
}
FILE *progcnx_get_stdout(struct prog_context *cnt) {
    return (cnt && cnt->fd_out != -1) ? fdopen(cnt->fd_out, "r") : NULL;
}
FILE *progcnx_get_stderr(struct prog_context *cnt) {
    return (cnt && cnt->fd_err != -1) ? fdopen(cnt->fd_err, "r") : NULL;
}

struct prog_context * prog_exec(const char* cmd, int flg) {
    int status;
    pid_t pid;

    int  stdin_pipe[2];
    int  stdout_pipe[2];
    int  stderr_pipe[2];

    struct prog_context *cnt = progcnx_new();
    if (!cnt) {
        return NULL;
    }
    if (flg & CHILD_IO_STDIN_CLOSE)  cnt->child_io_stdin_pipe = 0;
    if (flg & CHILD_IO_STDOUT_CLOSE) cnt->child_io_stdout_pipe = 0; 
    if (flg & CHILD_IO_STDERR_CLOSE || flg & CHILD_IO_STDERR_REDIRECT_TO_STDOUT)
        cnt->child_io_stderr_pipe = 0; 

    // make pipe for stdin
    if (cnt->child_io_stdin_pipe && (pipe(stdin_pipe) < 0)) {
        cnt->state = -1; 
        return cnt;
    }

    // make pipe for stdout
    if (cnt->child_io_stdout_pipe && (pipe(stdout_pipe) < 0)) {
        cnt->state = -1; 
        return cnt;
    }

    // make pipe for stderr
    if (cnt->child_io_stderr_pipe && (pipe(stderr_pipe) < 0)) {
        cnt->state = -1; 
        return cnt;
    }

    pid = fork ();
    if (pid < 0) {
        /* The fork failed.  Report failure.  */
        status = cnt->state = -1;
    }
    else if (pid == 0) {
        /* This is the child process.  Execute the shell command. */
        if (cnt->child_io_stdin_pipe)  {
            close(stdin_pipe[PIPE_WRITE_PORT]);
            dup2(stdin_pipe[PIPE_READ_PORT],STDIN_FILENO);
        }

        if (cnt->child_io_stdout_pipe)    {
            close(stdout_pipe[PIPE_READ_PORT]);
            dup2(stdout_pipe[PIPE_WRITE_PORT],STDOUT_FILENO);
        }

        if (flg & CHILD_IO_STDERR_REDIRECT_TO_STDOUT) {
            dup2(stdout_pipe[PIPE_WRITE_PORT],STDERR_FILENO);
        }
        else if (cnt->child_io_stderr_pipe) {
            close(stderr_pipe[PIPE_READ_PORT]);
            dup2(stderr_pipe[PIPE_WRITE_PORT],STDERR_FILENO);
        }

        if (cnt->child_exec_with_shell) {
            dlog("exec %s\n", cmd);
            execl (SHELL, SHELL, "-c",cmd, NULL);
        }
        else {
            //FIXME: use dynamic array.
#define ARGV_BUF_LEN 256
            char * arr[ARGV_BUF_LEN];            
            int len = ARGV_BUF_LEN;
            shlex_split(cmd,arr, &len);

            if (len > ARGV_BUF_LEN) {
                fprintf(stderr, "error! too much arguments.");
                _exit(EXIT_FAILURE);
            }
#undef ARGV_BUF_LEN

#ifdef __debug__
            int i = 0;
            while (arr[i] != NULL) {
                dlog("arr[%d] <%s> ", i, arr[i]);
                i++; 
            }
#endif
            execvp(arr[0], arr);
        }

        //never perform this statement if 'execvp' successful.
        _exit (EXIT_FAILURE); 
    }
    else {
        /* This is the parent process. */
        cnt->child_pid = pid;

        if (cnt->child_io_stdin_pipe) {
            close(stdin_pipe[PIPE_READ_PORT]);
            cnt->fd_in = stdin_pipe[PIPE_WRITE_PORT];
        }
        if (cnt->child_io_stdout_pipe) {
            close(stdout_pipe[PIPE_WRITE_PORT]);
            cnt->fd_out = stdout_pipe[PIPE_READ_PORT];
        }
        if (cnt->child_io_stderr_pipe) {
            close(stderr_pipe[PIPE_WRITE_PORT]);
            cnt->fd_err = stderr_pipe[PIPE_READ_PORT];
        }
    }
    return cnt;
}

int prog_poll(struct prog_context *context) {
    int status = 0;
    if (context) {
        /*Wait for the child to complete.  */
        int  ret = waitpid (context->child_pid, &status, WNOHANG); 
        if (ret ==  0) { /* the child state is not changed */
            status = -1;
        }
        else if (ret == context->child_pid)  { /* handle the child exited status */
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

int prog_wait(struct prog_context *context) {
    int status = 0;
    if (context && context->state == 0) {
        //int rc = waitpid (pid, &status, WNOHANG); 
        /*Wait for the child to complete.  */
        if (waitpid (context->child_pid, &status, 0) != context->child_pid) {
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

int prog_communicate(struct prog_context *context, 
        const char* input,
        struct dynamic_buffer *out_buf,
        struct dynamic_buffer *err_buf) {

    if (!context) { 
        return -1;
    }

    fd_set read_fd_set, write_fd_set;

    int fd_out, fd_err, fd_in;
    fd_out = fd_err = fd_in = -1;

    int need_watch_out = 0;
    int need_watch_err = 0;
    int need_watch_in  = 0;

    struct timeval tv;

    if (context->child_io_stdout_pipe) {
        fd_out = context->fd_out;
        need_watch_out = 1;
        fcntl(fd_out, F_SETFL, O_NONBLOCK);
    }

    if (context->child_io_stderr_pipe) {
        fd_err = context->fd_err;
        need_watch_err = 1;
        fcntl(fd_err, F_SETFL, O_NONBLOCK);
    }

    if (context->child_io_stdin_pipe)  {
        fd_in  = context->fd_in;
        need_watch_in  = 1;
    }

    int in_left_len  = strlen(input);

    while(1) {
        // Nothing to do, break the loop.
        if (!need_watch_out && !need_watch_err) 
            break;

        /* set default timeout value 5s */
        tv.tv_sec  = 5; 
        tv.tv_usec = 0;

        FD_ZERO (&read_fd_set);
        FD_ZERO (&write_fd_set);
        if (need_watch_out)     FD_SET(fd_out, &read_fd_set);
        if (need_watch_err)     FD_SET(fd_err, &read_fd_set);
        if (need_watch_in && in_left_len > 0)    FD_SET(fd_in, &write_fd_set);

        int max_fd = (fd_out > fd_err) ? fd_out : fd_err;
        int ret = select(max_fd + 1, &read_fd_set, &write_fd_set, NULL, &tv);
        if (ret < 0) {
            perror("select");
            return -1;
        }
        else if (ret == 0) { /*("timeout");*/
            dlog("select func timeout.");
            break;
        };

        if (need_watch_out && FD_ISSET(fd_out, &read_fd_set)) {
            if (dynbuffer_acquire(out_buf, 512)) {
                int len = read(fd_out, dynbuffer_writable_pos(out_buf), 512); 
                if (len > 0 ) {
                    dlog("read bytes [%d] from fd_out\n", len);
                    dynbuffer_consume(out_buf, len);
                }
                else {
                    dlog("read eof for fd_out.\n");
                    need_watch_out = 0;
                    break;
                }
            }
        }	

        if (need_watch_err && FD_ISSET(fd_err, &read_fd_set))  {
            if (dynbuffer_acquire(err_buf, 128)) {
                int len = read(fd_err, dynbuffer_writable_pos(err_buf), 128); 
                if (len > 0) {
                    dlog("read bytes [%d] from fd_err\n", len);
                    dynbuffer_consume(err_buf, len);
                }
                else {
                    dlog("read eof for fd_err.\n");
                    need_watch_err = 0;
                    break;
                }
            }
        }

        if (need_watch_in && FD_ISSET(fd_in, &write_fd_set)) {
            int len = write(fd_in, input, in_left_len);
            in_left_len -= len;
            input += len;
            dlog("wrote bytes [%d] into fd_in", len);
        }
    }
    return 0;
}

/** terminal the child process */
int prog_terminal(struct prog_context* cnx) {
    if (cnx) {
        int rc = kill(cnx->child_pid, SIGINT);
        if (rc != 0) {
            dlog("kill SIGINT failed, errno:[%d]\n", errno);
        }
    }
}

/** kill the chile process */
int prog_kill(struct prog_context* cnx) {
    if (cnx) {
        int rc = kill(cnx->child_pid, SIGKILL);
        if (rc != 0) {
            dlog("kill SIGKILL failed, errno:[%d]\n", errno);
        }
    }
}

#undef SHELL
#undef PIPE_READ_PORT
#undef PIPE_WRITE_PORT
