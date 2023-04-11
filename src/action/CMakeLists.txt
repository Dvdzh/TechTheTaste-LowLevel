add_library(action stepper.c motorpumpsvalve.c)
add_subdirectory(header)
target_link_libraries(action  pico_stdlib hardware_pwm hardware_irq hardware_gpio com)

