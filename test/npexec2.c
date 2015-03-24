#include "prog_exec.h"
#include "shell_exec.h"
#include "dynamic_buffer.h"
#include "trace.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int do_interact(const char *cmd, int flg) {
    printf("------------------------\n");
    printf("cmd: [%s] Flg: [%d]\n", cmd, flg);
    struct prog_context *cnt = prog_exec(cmd, flg); 
    int i = 0;
    do {
        struct dynamic_buffer *obuf = dynbuffer_new();
        struct dynamic_buffer *ebuf = dynbuffer_new();

        char buff[32];
        snprintf(buff, 32, "==> %d\n", i);
        printf("** in: %s", buff);
        prog_communicate(cnt, buff, obuf, ebuf);
        printf("** xout [%d]:%s.\n", dynbuffer_data_len(obuf), dynbuffer_data(obuf));
        printf("** xerr [%d]:%s.\n", dynbuffer_data_len(ebuf), dynbuffer_data(ebuf));

        dynbuffer_free(obuf);
        dynbuffer_free(ebuf);
    } while(prog_poll(cnt) == -1);
    return 0;
}

int main(int args, char* argv[]){
    //    do_test("ls -l /usr");
    //    do_test2("./mycat ");
    //    do_test2("python ./test.py ");
    //do_interact("python ./interact.py", 0);
    FILE *log = fopen("npx2.log", "w+");
    log_set_desc(log);    
    dlog("--------- test default arg");
    do_interact("cat npexec.c", 0); 
    dlog("--------- test CHILD_IO_STDOUT_CLOSE");
    do_interact("cat npexec.c", CHILD_IO_STDOUT_CLOSE); 
    dlog("--------- test CHILD_IO_STDERR_CLOSE");
    do_interact("python -c \"import sys; sys.stderr.write('1234567890'*100)\"", CHILD_IO_STDERR_CLOSE); 
    dlog("--------- test CHILD_IO_STDERR_REDIRECT_TO_STDOUT");
    do_interact("python -c \"import sys; sys.stderr.write('1234567890'*100)\"", CHILD_IO_STDERR_REDIRECT_TO_STDOUT); 

    return 0;
}
