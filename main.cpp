#include "pico/binary_info.h"
#include "pico/float.h"
#include "pico/stdlib.h"
// #include "pico/time.h"
// #include "hardware/i2c.h"
// #include "hardware/pwm.h"
// #include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
// #include "tusb.h"

#include "SEGGER_RTT.h"
#include "constants.hpp"
// #include "pum.hpp"

#include <cstdint>
#include <cstdio>

///DCDC PSM control for reduce 3.3V output ripple.
void reduceDcdcRipple()
{
    gpio_init (pin::DCDC::PS_CONTROL);
    gpio_set_dir (pin::DCDC::PS_CONTROL, GPIO_OUT);
    gpio_put (pin::DCDC::PS_CONTROL, DCDC::PWM); // PWM mode for reduce ripple.
}

int main()
{
    // Project discripsion.
    bi_decl (bi_program_description ("This is a skelton project for RP2040."));
    bi_decl (bi_1pin_with_name (pin::led, "Pico on-board LED"));

    reduceDcdcRipple();

    // Init stdio
    stdio_init_all();

    // Init GPIO
    gpio_init (pin::led);
    gpio_set_dir (pin::led, GPIO_OUT);

    while (1)
    {
        busy_wait_ms (1);

        std::printf ("test\n");                                     //USB-Serial console output
        SEGGER_RTT_WriteString (0, "Hello World from SEGGER!\r\n"); //SEGGER RTT console output

        //LED blink
        gpio_put (pin::led, 1);
        sleep_ms (250);
        gpio_put (pin::led, 0);
        sleep_ms (250);
    }
}