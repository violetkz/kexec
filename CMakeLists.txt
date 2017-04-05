project(kexec)

cmake_minimum_required(VERSION 2.8)
##set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -D__debug__ -g")

set(SRC 
    dynamic_buffer.c 
    prog_exec.c
    shell_exec.c  
    shlex.c  
    trace.c
    )


add_library(kexec STATIC ${SRC})

set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/kexeclib)

install(FILES 
    dynamic_buffer.h
    shell_exec.h
    prog_exec.h
    DESTINATION include )

install(TARGETS kexec
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
add_subdirectory(test)
