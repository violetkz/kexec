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
#ifndef _SIMPLE_SH_LEX_H____
#define _SIMPLE_SH_LEX_H____


/** 
 *  /bin/ls -rl /tmp => ['/bin/ls', '-rl', '/tmp']
 *  echo "ls -rl \" " =>['echo', 'ls -rl " ']
 */
int shlex_split(const char *cmd, char * argv[], int *argv_len); 


#endif //~_SIMPLE_SH_LEX_H____
