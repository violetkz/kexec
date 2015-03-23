
include_directories(. ..)
add_executable(npx npexec.c)
add_executable(npx2 npexec2.c)
target_link_libraries(npx kexec)
target_link_libraries(npx2 kexec)
