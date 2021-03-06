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
#include "prog_exec.h"
#include "shell_exec.h"
#include "dynamic_buffer.h"
#include "trace.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int do_test(const char *cmd, int flg) {
    printf("------------------------\n");
    printf("cmd: [%s] Flg: [%d]\n", cmd, flg);
    struct prog_context *cnt = prog_exec(cmd, flg); 
    int rc = -1;

    char *c;
#if defined(__APPLE__)
    rc = shell_exec("base64 -b 80 /bin/echo",  &c);
#elif defined(__linux__) || defined(__unix)
    rc = shell_exec("base64 /bin/echo", &c);
#endif
    
    if (rc != 0) {
        fprintf(stderr, "shex_exec failed.");
        exit(EXIT_FAILURE);
    }
    printf("**str len: %ld\n", strlen(c));

    struct dynamic_buffer *obuf = dynbuffer_new();
    struct dynamic_buffer *ebuf = dynbuffer_new();

    prog_communicate(cnt, c, obuf, ebuf, 5);
    printf("** xout [%d]:%s.\n", dynbuffer_data_len(obuf), dynbuffer_data(obuf));
    printf("** xerr [%d]:%s.\n", dynbuffer_data_len(ebuf), dynbuffer_data(ebuf));

    dynbuffer_free(obuf);
    dynbuffer_free(ebuf);

    rc = prog_wait(cnt); 
    printf("RC: [%d]\n", rc);
    progcnx_free(cnt);

    free(c);
    return 0;
}

int main(int args, char* argv[]){
    FILE *log = fopen("logfifo", "w");
    
    if (log == NULL) { 
        fprintf(stderr, "open log failed");
        return -1;
    }
    log_set_desc(log);
    do_test("cat -b -", 0);
    return 0;
}
