#include "constants.hpp"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <cstdint>
#include <cstdio>

int main() {
  // Project discripsion.
  bi_decl(bi_program_description("This is a skelton project for RP2040."));
  bi_decl(bi_1pin_with_name(pin::led, "Pico on-board LED"));

  // Init stdio
  stdio_init_all();

  // Init GPIO
  gpio_init(pin::led);
  gpio_set_dir(pin::led, GPIO_OUT);

  // Blink LED
  while (1) {
    std::printf("test\n");
    gpio_put(pin::led, 1);
    sleep_ms(250);
    gpio_put(pin::led, 0);
    sleep_ms(250);
  }
}