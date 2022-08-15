#pragma once

#include "driver/gpio.h"
#include <cstdint>

namespace util::wrappers
{
class Gpio
{
public:
    constexpr Gpio(gpio_num_t pin) : pin{pin} {};

    void init(gpio_mode_t mode)
    {
        gpio_pad_select_gpio(pin);
        gpio_set_direction(pin, mode);
    }

    void setPullMode(gpio_pull_mode_t mode)
    {
        gpio_set_pull_mode(pin, mode);
    }

    bool read()
    {
        return gpio_get_level(pin);
    }

    void write(bool state)
    {
        gpio_set_level(pin, state);
    }

private:
    gpio_num_t pin;
};
} // namespace util::wrappers