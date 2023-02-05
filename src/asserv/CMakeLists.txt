add_library (Nyx PID.c encoder.c motion.c motor.c)

target_link_libraries(Nyx PUBLIC pico_stdlib hardware_pwm hardware_irq hardware_gpio pico_time )  

