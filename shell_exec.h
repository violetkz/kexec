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
#ifndef ___shell_exec__h_
#define ___shell_exec__h_
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif 
/**
 *  Execute a command with default shell (/bin/sh) and save 
 *  the output to 'out'. 
 *  
 *  @param  cmd         the command string end with '\0'.
 *  @param  out [out]   the pointer of the output buffer will be saved int 
 *                      '*out'
 *  @return if return value >= 0, it mean the exit code of child process.
 *          others value mean there is the exception during execute process.
 *             -1: fork/pipe error
 *             -2: exit due to singal
 *             -3: process is not terminate. it in stopped.
 *  
 *  @note: 1) Must free the memory after using.
 *         2) Don't use this function to execute program which make large output.
 *            it cause to cost large memory.
 *         3) please be careful to write your command. 
 *         for example,  shell_exec("ls /tmp; rm -rf /");  // GOD bless U!
 *
 *  @code
 *      char *buf;
 *      int ret = shell_exec("ls -h /tmp", &buf);
 *      printf("%s \n", buf);
 *      free(buf);
 *  @endcode
 */
int shell_exec(const char *cmd, char **out);
#ifdef  __cplusplus
}
#endif 
#endif /*~___shell_exec__h_ */
