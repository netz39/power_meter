#include "StatusLed.hpp"

#include "wrappers/gpio.hpp"

util::wrappers::Gpio ledGpio{gpio_num_t::GPIO_NUM_5};

void StatusLed::taskMain()
{
    ledGpio.init(gpio_mode_t::GPIO_MODE_OUTPUT);

    while (true)
    {
        if (isConnected)
        {
            ledGpio.write(true);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        else
        {
            ledGpio.write(true);
            vTaskDelay(pdMS_TO_TICKS(500));

            ledGpio.write(false);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}