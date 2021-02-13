/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include <tusb.h>

// https://github.com/Hermann-SW/pico-examples/blob/master/tools/flash
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char)
  { reset_usb_boot(0, 0); } // go to flash mode

static int cnt=0;
int cl;

void core1_entry() {
    printf("%d\n", ++cnt);

    uint32_t g = multicore_fifo_pop_blocking();
    multicore_fifo_push_blocking(123456);

    for(cl=g; cl>130000; --cl)
    {
      set_sys_clock_48mhz(); sleep_ms(2); printf("\r%d ",cl);

      if (set_sys_clock_khz(cl, false))
      {
        set_sys_clock_48mhz(); sleep_ms(2); printf("\n");
      }
    }

    printf("done\n");

    while (1)
        tight_loop_contents();
}

int main() {
    stdio_init_all();
    tud_cdc_set_wanted_char('\0');
    while (!tud_cdc_connected()) { sleep_ms(100);  }
    printf("tud_cdc_connected()\n");


    gpio_init(2);  gpio_set_dir(2, GPIO_OUT);
    gpio_init(3);  gpio_set_dir(3, GPIO_OUT);

    vreg_set_voltage(VREG_VOLTAGE_1_25);
    sleep_ms(1);

    printf("Hello, multicore!\n");

    multicore_launch_core1(core1_entry);

    multicore_fifo_push_blocking(408099);
    uint32_t g = multicore_fifo_pop_blocking();

  for(;;)
  {
    for(int old=999999; old!=cl; )
    {
      old=cl;
      sleep_ms(5);
    }

    set_sys_clock_48mhz(); sleep_ms(2); printf("cl=%d\n",cl);

    sleep_ms(10);
    multicore_reset_core1();  //needed
    sleep_ms(10);

    multicore_launch_core1(core1_entry);

    multicore_fifo_push_blocking(cl-1);
    g = multicore_fifo_pop_blocking();
  }

    for(;;){}
}
