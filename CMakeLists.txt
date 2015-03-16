project(kexec)

set(SRC 
    dynamic_buffer.c 
    prog_exec.c
    shell_exec.c  
    shlex.c  
    trace.c
    )

add_library(kexec STATIC ${SRC})

add_subdirectory(test)
