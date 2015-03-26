
include_directories(. ..)
add_executable(npx npexec.c)
add_executable(npx2 npexec2.c)
add_executable(npx3 npexec3.c)
target_link_libraries(npx kexec)
target_link_libraries(npx2 kexec)
target_link_libraries(npx3 kexec)

install(TARGETS npx npx2 npx3
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
