add_executable(hello_multicore
        multicore.c
        )

# Add pico_multicore which is required for multicore functionality
target_link_libraries(hello_multicore
        pico_stdlib
        pico_multicore)

pico_enable_stdio_usb(hello_multicore 1)
pico_enable_stdio_uart(hello_multicore 0)

target_compile_definitions(hello_multicore PRIVATE PICO_ENTER_USB_BOOT_ON_EXIT=1)

# create map/bin/hex file etc.
pico_add_extra_outputs(hello_multicore)

# add url via pico_set_program_url
example_auto_set_url(hello_multicore)

pico_set_binary_type(hello_multicore copy_to_ram)
