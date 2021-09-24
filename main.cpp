#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <stdio.h>

const uint LED_PIN = 25;

int main() {
  // Project discripsion.
  bi_decl(bi_program_description("This is a jade firmware test for RP2040."));
  bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

  // Init stdio
  stdio_init_all();

  // Init GPIO
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (1) {
  }
}