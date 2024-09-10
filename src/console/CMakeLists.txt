# build target
add_executable(hisa main.cpp
 REPL/Shell.cpp
 REPL/ShellCommands.cpp
)

target_link_libraries(hisa libhisa)