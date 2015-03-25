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
    int i = 0;
    int rc = -1;

    char *c;
#if defined(__APPLE__)
    rc = shell_exec("base64 -b 80 /bin/echo",  &c);
#elif defined(__linux__)
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
    do_test("cat -b -", 0);
    return 0;
}
