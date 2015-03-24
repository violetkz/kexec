project(kexec)

cmake_minimum_required(VERSION 2.8)

set(SRC 
    dynamic_buffer.c 
    prog_exec.c
    shell_exec.c  
    shlex.c  
    trace.c
    )

add_definitions("-D __debug__ -g")
add_library(kexec STATIC ${SRC})

add_subdirectory(test)
