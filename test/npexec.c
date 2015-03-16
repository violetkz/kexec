#include "prog_exec.h"
#include "shell_exec.h"
#include "dynamic_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int do_test(const char* cmd) {
   char buf[1024];
   struct prog_context * cnt = prog_exec(cmd, 0); 
   FILE *out = progcnx_get_stdout(cnt);
   FILE *err = progcnx_get_stderr(cnt);
   while (!feof(out)) {
       memset(buf, 0x00, 1024);
       fread(buf, 1, 1024, out);
       printf("**out: %s \n", buf); 
       fread(buf, 1, 1024, err); 
       printf("**err: %s \n", buf); 
   }
   int ret = prog_wait(cnt);
   printf("** exit status: %d\n", ret);
   progcnx_free(cnt);
   return 0;
}

int do_test2(const char* cmd) {
   struct prog_context *cnt = prog_exec(cmd, 0); 
   struct dynamic_buffer *obuf = dynbuffer_new();
   struct dynamic_buffer *ebuf = dynbuffer_new();
   
   prog_communicate(cnt, "keyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n", obuf, ebuf);
   printf("** xout: %s\n", dynbuffer_data(obuf));
   int ret = prog_wait(cnt);
   printf("** exit status: %d\n", ret);
   dynbuffer_free(obuf);
   dynbuffer_free(ebuf);
   progcnx_free(cnt);
   return 0;
}

int do_interact(const char *cmd) {
   struct prog_context *cnt = prog_exec(cmd, 0); 
   int i = 0;
   while (prog_poll(cnt) == -1) {
       struct dynamic_buffer *obuf = dynbuffer_new();
       struct dynamic_buffer *ebuf = dynbuffer_new();

       char buff[32];
       snprintf(buff, 32, "==> %d\n", i);
       printf("** in: %s", buff);
       prog_communicate(cnt, buff, obuf, ebuf);
       printf("** xout: %s\n", dynbuffer_data(obuf));

       dynbuffer_free(obuf);
       dynbuffer_free(ebuf);
   }
   return 0;
}

int main(int args, char* argv[]){
//    do_test("ls -l /usr");
//    do_test2("./mycat ");
//    do_test2("python ./test.py ");
   do_interact("python ./interact.py");
    
    return 0;
}
