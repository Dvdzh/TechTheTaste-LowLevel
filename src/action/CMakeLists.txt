add_library(action stepper.c)
add_subdirectory(header)
target_link_libraries(action pico_stdlib hardware_pwm hardware_irq com)

