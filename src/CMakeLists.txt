add_subdirectory(com)
add_subdirectory(action)
add_subdirectory(asserv)
add_subdirectory(function)

add_executable(TechTheTaste main.c)
target_link_libraries(TechTheTaste function com pico_stdlib)
pico_add_extra_outputs(TechTheTaste)
