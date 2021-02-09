/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include <tusb.h>

// https://gist.github.com/Hermann-SW/ca07f46b7f9456de41f0956d81de01a7
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char)
  { reset_usb_boot(0, 0); } // go to flash mode

#define FLAG_VALUE 123

static int cnt=0;

void core1_entry() {
    printf("%d\n", ++cnt);

    multicore_fifo_push_blocking(FLAG_VALUE);

    uint32_t g = multicore_fifo_pop_blocking();

    if (g != FLAG_VALUE)
        printf("Hmm, that's not right on core 1!\n");
    else
        printf("Its all gone well on core 1!\n");

    while (1)
        tight_loop_contents();
}

int main() {
    stdio_init_all();
    tud_cdc_set_wanted_char('\0');
    while (!tud_cdc_connected()) { sleep_ms(100);  }
    printf("tud_cdc_connected()\n");

    printf("Hello, multicore!\n");

    /// \tag::setup_multicore[]

    multicore_launch_core1(core1_entry);

    // Wait for it to start up

    uint32_t g = multicore_fifo_pop_blocking();

    if (g != FLAG_VALUE)
        printf("Hmm, that's not right on core 0!\n");
    else {
        multicore_fifo_push_blocking(FLAG_VALUE);
        printf("It's all gone well on core 0!\n");
    }

    sleep_ms(10);
    multicore_reset_core1();  //needed
    sleep_ms(10);

    printf("Hello, multicore!b\n");

    multicore_launch_core1(core1_entry);
    g = multicore_fifo_pop_blocking();
    if (g != FLAG_VALUE)
        printf("Hmm, that's not right on core 0!b\n");
    else {
        multicore_fifo_push_blocking(FLAG_VALUE);
        printf("It's all gone well on core 0!b\n");
    }

    /// \end::setup_multicore[]

    for(;;){}
}
