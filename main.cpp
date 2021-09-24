#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <stdio.h>

constexpr uint LED_PIN = PICO_DEFAULT_LED_PIN; // 25

int main() {
  // Project discripsion.
  bi_decl(bi_program_description("This is a jade firmware test for RP2040."));
  bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

  // Init stdio
  stdio_init_all();

  // Init GPIO
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  // Blink LED
  while (1) {
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
  }
}