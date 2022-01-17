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
#include "pico/time.h"
#include "pico/types.h"
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
    gpio_put (pin::led, 1);

    sleep_ms (5000);

    //GPOUT0初期化
    clock_gpio_init (pin::clock::out0,                                 //出力GPOUT0
                     CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0, //GPINをクロックソースにする
                     1);                                               //分周比
    //GPIN0初期化
    clock_configure_gpin (clk_gpout0, //クロック出力先
                          20,         //GPIO 20 or 22 only
                          12288000,   //12.288MHz この値がどう使われるのかは不明
                          12288000);  //この値がどう使われるのかは不明

    const int q = 15; //Q15 16bit固定小数点数指定

    {
        //pico_float version
        absolute_time_t begin;
        absolute_time_t end;
        volatile float x = 0.0f;
        begin = get_absolute_time();
        for (volatile int m = 0; m < 20000; ++m)
        {
            x = fix2float (m, 15);
        }
        end = get_absolute_time();
        const auto t = absolute_time_diff_us (begin, end);
        std::printf ("fix2float time: %llu\n", t);
    }

    {
        //division fix to float version
        absolute_time_t begin;
        absolute_time_t end;
        volatile float x = 0.0f;
        begin = get_absolute_time();
        for (volatile int m = 0; m < 20000; ++m)
        {
            x = m / 32768.0f;
        }
        end = get_absolute_time();
        const auto t = absolute_time_diff_us (begin, end);
        std::printf ("float div time: %llu\n", t);
    }

    // fix2float time: 25798
    // float div time: 20512

    while (1)
    {
        //LED blink
        gpio_put (pin::led, 1);
        sleep_ms (250);
        gpio_put (pin::led, 0);
        sleep_ms (250);
    }
}