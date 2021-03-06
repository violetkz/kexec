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

    struct dynamic_buffer *obuf = dynbuffer_new();
    struct dynamic_buffer *ebuf = dynbuffer_new();

    //char buff[32];
    //snprintf(buff, 32, "==> %d\n", i);
    //printf("** in: %s", buff);
    prog_communicate(cnt, NULL, obuf, ebuf, 5);
    printf("** xout [%d]:%s.\n", dynbuffer_data_len(obuf), dynbuffer_data(obuf));
    printf("** xerr [%d]:%s.\n", dynbuffer_data_len(ebuf), dynbuffer_data(ebuf));

    rc = prog_wait(cnt); 
    printf("RC: [%d]\n", rc);
    dynbuffer_free(obuf);
    dynbuffer_free(ebuf);
    progcnx_free(cnt);
    return 0;
}

int main(int args, char* argv[]){
    //    do_test("ls -l /usr");
    //    do_test2("./mycat ");
    //    do_test2("python ./test.py ");
    //do_test("python ./interact.py", 0);
    FILE *log = fopen("npx2.log", "w+");
    if (log == NULL) {
        fprintf(stderr, "open log fail failed");
        return EXIT_FAILURE;
    }
    log_set_desc(log);    
    dlog("--------- test default arg");
    do_test("cat npexec.c", 0); 

    dlog("--------- test SUBPROC_IGNORE_STDOUT");
    do_test("cat npexec.c", SUBPROC_IGNORE_STDOUT); 

    dlog("--------- test SUBPROC_IGNORE_STDERR");
    do_test("python -c \"import sys; print '#'*10; sys.stderr.write('1234567890'*100)\"", SUBPROC_IGNORE_STDERR); 

    dlog("--------- test SUBPROC_STDERR_REDIRECT_TO_STDOUT");
    do_test("python -c \"import sys; print '#'*10; sys.stderr.write('1234567890'*100)\"", SUBPROC_STDERR_REDIRECT_TO_STDOUT); 

    dlog("--------- test SUBPROC_EXEC_WITH_SHELL");
    do_test("head -c 2K /bin/echo | base64", SUBPROC_EXEC_WITH_SHELL); 

    dlog("--------- test SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDOUT");
    do_test("head -c 2K /bin/echo | base64", SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDOUT); 

    dlog("--------- test SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDERR");
    do_test("head -c 2K /bin/echo | base64 - 1>&2", SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDERR); 
    
    dlog("--------- test SUBPROC_EXEC_WITH_SHELL|SUBPROC_STDERR_REDIRECT_TO_STDOUT");
    do_test("head -c 2K /bin/echo | base64 - 1>&2", SUBPROC_EXEC_WITH_SHELL|SUBPROC_STDERR_REDIRECT_TO_STDOUT); 

    dlog("--------- test SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDERR|SUBPROC_IGNORE_STDOUT");
    do_test("python -c \"import sys; print '#'*10; sys.stderr.write('1234567890'*100)\"", 
            SUBPROC_EXEC_WITH_SHELL|SUBPROC_IGNORE_STDERR|SUBPROC_IGNORE_STDOUT); 

    dlog("--------- test SUBPROC_EXEC_WITH_SHELL|SUBPROC_CLOSE_STDIN");
    do_test("echo | cat", SUBPROC_EXEC_WITH_SHELL|SUBPROC_CLOSE_STDIN); 

    return 0;
}
