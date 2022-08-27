#pragma once

#include "Button.hpp"
#include "Timebase.hpp"
#include "units/si/time.hpp"
#include "wrappers/Task.hpp"

class PulseDetector : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    static constexpr auto PrintTag = "PulseDetector";

    static constexpr auto MinimumPulseLength = 30.0_ms;
    static constexpr auto UpdateTime = 10.0_ms;
    static constexpr auto InputPin = gpio_num_t::GPIO_NUM_32;

    explicit PulseDetector(Timebase::TimestampQueue &timestampQueue, bool &pulseArrived)
        : TaskWithMemberFunctionBase("pulseDetectorTask", 1024, osPriorityNormal5), //
          timestampQueue(timestampQueue),                                           //
          pulseArrived(pulseArrived)
    {
        constexpr auto Divisor = (MinimumPulseLength / UpdateTime).getMagnitude();
        static_assert(ceilf(Divisor) == Divisor,
                      "The update time is not an even divisor of minimum pulse length!");
    }

protected:
    void taskMain() override;

private:
    Timebase::TimestampQueue &timestampQueue;
    bool &pulseArrived;

    util::wrappers::Gpio pulseInputGpio{InputPin};
    Button pulseInput{pulseInputGpio,                                           //
                      [this](Button::Action action) { pulseCallback(action); }, //
                      MinimumPulseLength};

    void pulseCallback(Button::Action action);
};
