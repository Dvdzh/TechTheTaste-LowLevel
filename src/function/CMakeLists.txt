add_library(function function.c)
add_subdirectory(header)
target_link_libraries(function com Nyx pico_stdlib )
