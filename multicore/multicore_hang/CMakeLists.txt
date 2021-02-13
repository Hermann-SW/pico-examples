add_executable(multicore_hang
        multicore.c
        )

# Add pico_multicore which is required for multicore functionality
target_link_libraries(multicore_hang
        pico_stdlib
        pico_multicore)

pico_enable_stdio_usb(multicore_hang 1)
pico_enable_stdio_uart(multicore_hang 0)

target_compile_definitions(multicore_hang PRIVATE PICO_ENTER_USB_BOOT_ON_EXIT=1)

# create map/bin/hex file etc.
pico_add_extra_outputs(multicore_hang)

# add url via pico_set_program_url
example_auto_set_url(multicore_hang)

pico_set_binary_type(multicore_hang copy_to_ram)
