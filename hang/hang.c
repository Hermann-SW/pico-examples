#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "pico/bootrom.h"
#include <tusb.h>

// https://gist.github.com/Hermann-SW/ca07f46b7f9456de41f0956d81de01a7
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char)
  { reset_usb_boot(0, 0); } // go to flash mode

int main() {
    stdio_init_all();
    tud_cdc_set_wanted_char('\0');
    while (!tud_cdc_connected()) { sleep_ms(100);  }
    printf("tud_cdc_connected()\n");

    gpio_init(2);  gpio_set_dir(2, GPIO_OUT);
    gpio_init(3);  gpio_set_dir(3, GPIO_OUT);

    vreg_set_voltage(VREG_VOLTAGE_1_10);
    sleep_ms(1);

    for(int cl=408099; cl>130000; --cl)
    {
      set_sys_clock_48mhz(); sleep_ms(2); printf("\r%d ",cl);

      if (set_sys_clock_khz(cl, false))
      {
        set_sys_clock_48mhz(); sleep_ms(2); printf("\n");
      }
    }

    printf("done\n");
}
