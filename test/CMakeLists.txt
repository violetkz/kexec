
include_directories(. ..)
add_executable(test1 test.c)
add_executable(test2 test2.c)
add_executable(test3 test3.c)
target_link_libraries(test1 kexec)
target_link_libraries(test2 kexec)
target_link_libraries(test3 kexec)

install(TARGETS test1 test2 test3
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
