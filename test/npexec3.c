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

    struct dynamic_buffer *obuf = dynbuffer_new();
    struct dynamic_buffer *ebuf = dynbuffer_new();

    //char buff[32];
    //snprintf(buff, 32, "==> %d\n", i);
    //printf("** in: %s", buff);
    prog_communicate(cnt, NULL, obuf, ebuf, 5);
    printf("** xout [%d]:%s.\n", dynbuffer_data_len(obuf), dynbuffer_data(obuf));
    printf("** xerr [%d]:%s.\n", dynbuffer_data_len(ebuf), dynbuffer_data(ebuf));

    dynbuffer_free(obuf);
    dynbuffer_free(ebuf);

    rc = prog_wait(cnt); 
    printf("RC: [%d]\n", rc);
    progcnx_free(cnt);
    return 0;
}

int main(int args, char* argv[]){

    return 0;
}
