#pragma once

#include "Button.hpp"
#include "Timebase.hpp"
#include "settings.hpp"
#include "units/si/time.hpp"
#include "wrappers/Task.hpp"

class PulseDetector : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    static constexpr auto PrintTag = "[PulseDetector]";

    explicit PulseDetector(Timebase::TimestampQueue &timestampQueue, bool &pulseArrived)
        : TaskWithMemberFunctionBase("pulseDetectorTask", 1024, osPriorityNormal5), //
          timestampQueue(timestampQueue),                                           //
          pulseArrived(pulseArrived)
    {
        constexpr auto Divisor =
            (settings::MinimumPulseLength / settings::DetectorUpdateTime).getMagnitude();
        static_assert(ceilf(Divisor) == Divisor,
                      "The update time is not an even divisor of minimum pulse length!");
    }

protected:
    void taskMain() override;

private:
    Timebase::TimestampQueue &timestampQueue;
    bool &pulseArrived;

    util::wrappers::Gpio pulseInputGpio{settings::PulseInputPin};
    Button pulseInput{pulseInputGpio,                                           //
                      [this](Button::Action action) { pulseCallback(action); }, //
                      settings::MinimumPulseLength};

    void pulseCallback(Button::Action action);
};
