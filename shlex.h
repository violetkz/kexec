#ifndef _SIMPLE_SH_LEX_H____
#define _SIMPLE_SH_LEX_H____


/** 
 *  /bin/ls -rl /tmp => ['/bin/ls', '-rl', '/tmp']
 *  echo "ls -rl \" " =>['echo', 'ls -rl " ']
 */
int shlex_split(const char *cmd, char * argv[], int *argv_len); 


#endif //~_SIMPLE_SH_LEX_H____
