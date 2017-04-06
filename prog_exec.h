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

/*                      
 *                                              Ken <proxy.vk@gmail.com>
 *                                                            2015-10-22
 */
#ifndef __PROG_EXEC_H___
#define __PROG_EXEC_H___

#include <stdio.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif 

/*
 * spawn new processes, connect to their input/output/error pipes,
 * and obtain their return codes.
 */


/* the context info used by function 'prog_exec' */
struct prog_context;
/* create a context */
struct prog_context *progcnx_new();
/* free the context */
void  progcnx_free(struct prog_context *cnt);

/* return FILE pointer which can write the data to stdin of he child process 
 * 
 * NOTE: should call `fclose' after using.
 */
FILE *progcnx_get_stdin(struct prog_context *cnt);

/* return FILE pointer which can read the data which the child process 
 * output in stdout
 * 
 * NOTE: should call `fclose' after using.
 */
FILE *progcnx_get_stdout(struct prog_context *cnt);

/* return FILE pointer which can read the data which the child process 
 * output in stderr
 * NOTE: should call `fclose' after using.
 */
FILE *progcnx_get_stderr(struct prog_context *cnt);

#define SUBPROC_EXEC_BY_DEFAULT              0x0
#define SUBPROC_CLOSE_STDIN                  0x1<<0
#define SUBPROC_IGNORE_STDOUT                0x1<<1
#define SUBPROC_IGNORE_STDERR                0x1<<2
#define SUBPROC_STDERR_REDIRECT_TO_STDOUT    0x1<<3
#define SUBPROC_EXEC_WITH_SHELL              0x1<<4

/* spawn a new process */
struct prog_context *prog_exec(const char* cmd, int flg);

/* wait for the status of process changed
 * return the exit code of child process 
 */ 
int prog_wait(struct prog_context *context);

/* check if the process exited
 * @return if child process do not exit, return -1;
 *         if child process exited,return the exit code [0-255]
 */
int prog_poll(struct prog_context *context);

struct dynamic_buffer;
/* Interact with process: Send data to stdin. Read data from stdout
 * and stderr, until end-of-file is reached. Wait for process to terminate. 
 */
int prog_communicate(struct prog_context *context, 
               const char* input,
               struct dynamic_buffer *out_buf, 
               struct dynamic_buffer *err_buf,
               int timeout);

/** terminal the child process */
int prog_terminal(struct prog_context* cnx);

/** kill the chile process */
int prog_kill(struct prog_context* cnx);

#ifdef  __cplusplus
}
#endif 
#endif //~__PROG_EXEC_H___
