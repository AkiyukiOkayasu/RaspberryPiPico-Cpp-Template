#include "pico/binary_info.h"
#include "pico/float.h"
#include "pico/stdlib.h"
// #include "pico/time.h"
// #include "hardware/i2c.h"
// #include "hardware/pwm.h"
// #include "hardware/pio.h"
#include "SEGGER_RTT.h"
#include "constants.hpp"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"
#include "pico/time.h"
#include "pico/types.h"
#include "tusb.h"
#include "usb_descriptors.h"
// #include "pum.hpp"

#include <cstdint>
#include <cstdio>

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb (void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb (void)
{
    // blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb (bool remote_wakeup_en)
{
    // (void) remote_wakeup_en;
    // blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb (void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report (uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (! tud_hid_ready()) return;

    switch (report_id)
    {
        case REPORT_ID_KEYBOARD:
        {
            // use to avoid send multiple consecutive zero report for keyboard
            static bool has_keyboard_key = false;

            if (btn)
            {
                uint8_t keycode[6] = { 0 };
                keycode[0] = HID_KEY_A;

                tud_hid_keyboard_report (REPORT_ID_KEYBOARD, 0, keycode);
                has_keyboard_key = true;
            }
            else
            {
                // send empty key report if previously has key pressed
                if (has_keyboard_key) tud_hid_keyboard_report (REPORT_ID_KEYBOARD, 0, NULL);
                has_keyboard_key = false;
            }
        }
        break;

        case REPORT_ID_MOUSE:
        {
            int8_t const delta = 5;

            // no button, right + down, no scroll, no pan
            tud_hid_mouse_report (REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
        }
        break;

        case REPORT_ID_CONSUMER_CONTROL:
        {
            // use to avoid send multiple consecutive zero report
            static bool has_consumer_key = false;

            if (btn)
            {
                // volume down
                uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
                tud_hid_report (REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
                has_consumer_key = true;
            }
            else
            {
                // send empty key report (release key) if previously has key pressed
                uint16_t empty_key = 0;
                if (has_consumer_key) tud_hid_report (REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
                has_consumer_key = false;
            }
        }
        break;

        case REPORT_ID_GAMEPAD:
        {
            // use to avoid send multiple consecutive zero report for keyboard
            static bool has_gamepad_key = false;

            hid_gamepad_report_t report = {
                .x = 0,
                .y = 0,
                .z = 0,
                .rz = 0,
                .rx = 0,
                .ry = 0,
                .hat = 0,
                .buttons = 0
            };

            if (btn)
            {
                report.hat = GAMEPAD_HAT_UP;
                report.buttons = GAMEPAD_BUTTON_A;
                tud_hid_report (REPORT_ID_GAMEPAD, &report, sizeof (report));

                has_gamepad_key = true;
            }
            else
            {
                report.hat = GAMEPAD_HAT_CENTERED;
                report.buttons = 0;
                if (has_gamepad_key) tud_hid_report (REPORT_ID_GAMEPAD, &report, sizeof (report));
                has_gamepad_key = false;
            }
        }
        break;

        default:
            break;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task (void)
{
    // Poll every 10ms
    // const uint32_t interval_ms = 10;
    // static uint32_t start_ms = 0;

    // if (board_millis() - start_ms < interval_ms) return; // not enough time
    // start_ms += interval_ms;

    // uint32_t const btn = board_button_read();

    // Remote wakeup
    if (tud_suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }
    else
    {
        // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
        send_hid_report (REPORT_ID_KEYBOARD, 1);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb (uint8_t instance, uint8_t const* report, uint8_t len)
{
    (void) instance;
    (void) len;

    uint8_t next_report_id = report[0] + 1;

    if (next_report_id < REPORT_ID_COUNT)
    {
        send_hid_report (next_report_id, 1);
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb (uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb (uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1) return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                // blink_interval_ms = 0;
                // board_led_write (true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                // board_led_write (false);
                // blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

//=======================================
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

    tusb_init();

    // Init stdio
    // stdio_init_all();

    // Init GPIO
    gpio_init (pin::led);
    gpio_set_dir (pin::led, GPIO_OUT);
    gpio_put (pin::led, 1);

    sleep_ms (1000);

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
        tud_task();
        //LED blink
        gpio_put (pin::led, 1);
        sleep_ms (100);
        gpio_put (pin::led, 0);
        sleep_ms (100);
        // if (stdio_usb_connected())
        // {
        //     reset_usb_boot (0, 0);
        // }
    }
}