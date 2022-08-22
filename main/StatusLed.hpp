#pragma once

#include "wrappers/Task.hpp"
#include "wrappers/gpio.hpp"

class StatusLed : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    explicit StatusLed(bool &isConnected)
        : TaskWithMemberFunctionBase("statusLedTask", 256, osPriorityBelowNormal3),
          isConnected(isConnected) //
          {};

protected:
    void taskMain() override;

private:
    bool &isConnected;

    util::wrappers::Gpio ledGpio{gpio_num_t::GPIO_NUM_5};
};