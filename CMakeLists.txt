project(kexec)

cmake_minimum_required(VERSION 2.8)

set(SRC 
    dynamic_buffer.c 
    prog_exec.c
    shell_exec.c  
    shlex.c  
    trace.c
    )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -D __debug__ -g")
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
