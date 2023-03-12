add_subdirectory(asserv)
add_subdirectory(action)
add_subdirectory(com)
add_subdirectory(function)
add_executable(Asserv main.c)
target_link_libraries(Asserv function com)
pico_add_extra_outputs(Asserv)

