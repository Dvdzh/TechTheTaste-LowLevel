project(Debugasserv LANGUAGES C CXX ASM)

add_executable(Debugasserv main.c)

add_library (Nyx PID.c encoder.c motion.c motor.c)
add_subdirectory(header)
target_link_libraries( Nyx PUBLIC pico_stdlib hardware_pwm hardware_irq hardware_gpio pico_time )  


target_link_libraries(Debugasserv PUBLIC Nyx)
target_link_libraries(Debugasserv PUBLIC pico_stdlib hardware_pwm hardware_irq hardware_gpio pico_time )  


pico_enable_stdio_usb(Debugasserv ON)
pico_enable_stdio_uart(Debugasserv ON)

pico_add_extra_outputs(Debugasserv)

