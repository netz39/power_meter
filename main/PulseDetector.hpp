#pragma once

#include "Button.hpp"
#include "units/si/time.hpp"
#include "wrappers/Task.hpp"

class PulseDetector : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    static constexpr auto MinimumPulseLength = 30.0_ms;
    static constexpr auto UpdateTime = 10.0_ms;
	static constexpr auto InputPin = gpio_num_t::GPIO_NUM_32;

    PulseDetector() : TaskWithMemberFunctionBase("pulseDetectorTask", 1024, osPriorityNormal5)
    {
        constexpr auto Divisor = (MinimumPulseLength / UpdateTime).getMagnitude();
        static_assert(ceilf(Divisor) == Divisor,
                      "The update time is not an even divisor of minimum pulse length!");
    }

protected:
    void taskMain() override;

private:
    util::wrappers::Gpio pulseInputGpio{InputPin};
    Button pulseInput{pulseInputGpio,                                           //
                      [this](Button::Action action) { pulseCallback(action); }, //
                      MinimumPulseLength};

    void pulseCallback(Button::Action action);
};
