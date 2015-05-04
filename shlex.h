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
#ifndef _SIMPLE_SH_LEX_H____
#define _SIMPLE_SH_LEX_H____


/** 
 * A simple lexical analyzers for command-line string of unix shell.
 * 
 * e.g.
 * `/bin/ls -rl /tmp`  => ['/bin/ls', '-rl', '/tmp']
 * `echo "ls -rl \" "` => ['echo', 'ls -rl " ']
 * 
 * @code
 * char argv[256];
 * int arrlen = sizeof(argv) / sizeof(argv[0]);
 * 
 * const char *cmd = "/bin/ls -rl /tmp";
 * int rc = shlex_split(cmd, argv, &arrlen);
 * if (rc != 0) {
 *      fprintf(stderr,"func shlex_split failed. cmd:[%s].", cmd);
 * }
 * // do something else.
 *
 * //finally, we must free the memory used by argv
 * free_shlex_argv(argv, arrlen);
 * }
 * @endcode
 */
int shlex_split(const char *cmd, char * argv[], int *argv_len); 

/* free the memory used by func shlex_split. */
void free_shlex_argv(char* argv[], int argv_len);

#endif //~_SIMPLE_SH_LEX_H____
