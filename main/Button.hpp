#pragma once

#include "units/si/time.hpp"
#include "wrappers/gpio.hpp"

#include <functional>

using units::si::Time;
using util::wrappers::Gpio;

class Button
{
public:
    enum class State
    {
        NotPressed = 0,
        Pressed = 1
    };

    enum class Action
    {
        ShortPress,
        LongPress,
        StopLongPress
    };

    using Callback = std::function<void(Action action)>;

    explicit Button(Gpio buttonGpio, bool isInverted = false, Callback callback = nullptr)
        : buttonGpio{buttonGpio}, isInverted{isInverted}, callback{callback} {};

    Button(Gpio &buttonGpio, const Time debouncingTime, const Time longPressTime = 500.0_ms,
           bool isInverted = false, Callback callback = nullptr)
        : buttonGpio{buttonGpio}, DebounceTime(debouncingTime), LongPressTime{longPressTime},
          isInverted{isInverted}, callback{callback} {};

    void update(Time timePassed);

    void setCallback(Callback newCallback)
    {
        callback = std::move(newCallback);
    }

    /// returns true if button state is long pressed at the moment
    [[nodiscard]] bool isPressing() const;

private:
    void resetTimer();
    void updateTimer(Time timePassed);
    [[nodiscard]] Time getPassedTime() const;

    Gpio &buttonGpio;

    const Time DebounceTime = 20.0_ms;
    const Time LongPressTime = 500.0_ms;
    bool isInverted = false;
    Callback callback;

    enum class InternalState
    {
        Idle,
        Pressed,
        LongPress
    };

    InternalState internalState = InternalState::Idle;
    Time pressTimer{};

    void buttonCallback(Action action);
};