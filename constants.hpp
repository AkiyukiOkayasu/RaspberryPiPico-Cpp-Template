#pragma once

#include "pico/stdlib.h"

#include <cstddef>
#include <cstdint>

/** Pin constants.
    @see https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiR9OXd58b0AhXV7GEKHS54ChwQFnoECBYQAQ&url=https%3A%2F%2Fdatasheets.raspberrypi.com%2Fpico%2FPico-R3-A4-Pinout.pdf&usg=AOvVaw31SQWFWkcgN8X2VgWD_CRm
*/
namespace pin
{
inline constexpr unsigned int led = PICO_DEFAULT_LED_PIN; // 25

namespace I2C0
{
    /** I2C SDA pin.
        いくつかの候補がある. pinout参照.
        @see PICO_DEFAULT_I2C_SDA_PIN
    */
    inline constexpr unsigned int SDA = 8;
    /** I2C SCL pin.
        いくつかの候補がある. pinout参照.
        @see PICO_DEFAULT_I2C_SCL_PIN
    */
    inline constexpr unsigned int SCL = 9;
} // namespace I2C0

namespace ADC
{
    inline constexpr unsigned int ch0 = 26;
    inline constexpr unsigned int ch1 = 27;
    inline constexpr unsigned int ch2 = 28;
    //PicoのADC 3chは使用済み.
} // namespace ADC

///オンボードDCDCのモード変更用 効率重視 or リップル低減重視.
namespace DCDC
{
    inline constexpr unsigned int PS_CONTROL = 23; //GPIO23
}

namespace clock
{
    inline constexpr unsigned int in0 = 20;
    inline constexpr unsigned int in1 = 22;
    inline constexpr unsigned int out0 = 21;
    inline constexpr unsigned int out1 = 23;
    inline constexpr unsigned int out2 = 24;
    inline constexpr unsigned int out3 = 25; //onboard ledにつながっているので注意
} // namespace clock
} // namespace pin

namespace ADC
{
inline constexpr float ConversionFactor = 3.3f / (1 << 12); //12bit
inline constexpr unsigned int ch0 = 0;
inline constexpr unsigned int ch1 = 1;
inline constexpr unsigned int ch2 = 2;
} // namespace ADC

/** Onboard 3.3V DCDC converter mode for improve ripple.
    @see pin::DCDC_PS_CONTROL    
*/
namespace DCDC
{
inline constexpr unsigned int PFM = 0; //PFM mode (default, best efficiency)
inline constexpr unsigned int PWM = 1; //PWM mode (improved ripple)
} // namespace DCDC