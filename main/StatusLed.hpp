#pragma once

#include "driver/ledc.h"
#include "settings.hpp"
#include "wrappers/Task.hpp"
#include "wrappers/gpio.hpp"

class StatusLed : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    static constexpr auto LedChannel = LEDC_CHANNEL_0;

    static constexpr auto PwmMode = LEDC_LOW_SPEED_MODE;
    static constexpr auto PwmResolution = LEDC_TIMER_8_BIT;
    static constexpr auto MaximumDuty = (1 << LEDC_TIMER_8_BIT) - 1;

    explicit StatusLed(bool &isConnected, bool &pulseArrived)
        : TaskWithMemberFunctionBase("statusLedTask", 256, osPriorityBelowNormal3),
          isConnected(isConnected), //
          pulseArrived(pulseArrived){};

protected:
    void taskMain() override;

private:
    bool &isConnected;
    bool &pulseArrived;

    void initLedPwm();

    void setLedDuty(uint8_t percentage);
};