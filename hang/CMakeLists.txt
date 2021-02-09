add_executable(hang
        hang.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(hang pico_stdlib pico_multicore)

pico_enable_stdio_usb(hang 1)
pico_enable_stdio_uart(hang 0)

target_compile_definitions(hang PRIVATE PICO_ENTER_USB_BOOT_ON_EXIT=1)

# create map/bin/hex file etc.
pico_add_extra_outputs(hang)

# add url via pico_set_program_url
example_auto_set_url(hang)

pico_set_binary_type(hang copy_to_ram) 
