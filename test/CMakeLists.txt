
include_directories(. ..)
add_executable(npx npexec.c)
target_link_libraries(npx kexec)
