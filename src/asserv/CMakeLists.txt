
add_executable(main main.c)

add_library (Nyx PID.c encoder.c motion.c motor.c)
add_subdirectory(header)
target_link_libraries( Nyx PUBLIC pico_stdlib hardware_pwm hardware_irq hardware_gpio pico_time )  


target_link_libraries(main PUBLIC Nyx)
target_link_libraries(main PUBLIC pico_stdlib hardware_pwm hardware_irq hardware_gpio pico_time )  


pico_enable_stdio_usb(main ON)
pico_enable_stdio_uart(main ON)

pico_add_extra_outputs(main)

