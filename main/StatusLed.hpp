#pragma once

#include "settings.hpp"
#include "wrappers/Task.hpp"
#include "wrappers/gpio.hpp"

class StatusLed : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    explicit StatusLed(bool &isConnected, bool &pulseArrived)
        : TaskWithMemberFunctionBase("statusLedTask", 256, osPriorityBelowNormal3),
          isConnected(isConnected), //
          pulseArrived(pulseArrived){};

protected:
    void taskMain() override;

private:
    bool &isConnected;
    bool &pulseArrived;

    util::wrappers::Gpio ledGpio{settings::StatusLedGpio};
};