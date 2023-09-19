#pragma once

#include "units/si/time.hpp"
#include "util/gpio.hpp"

#include <functional>

using units::si::Time;
using util::Gpio;

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

    Button(Gpio &buttonGpio, Callback callback, const Time debouncingTime = 20.0_ms,
           const Time longPressTime = 500.0_ms, bool isInverted = false)
        : buttonGpio{buttonGpio},       //
          buttonCallback{callback},     //
          DebounceTime(debouncingTime), //
          LongPressTime{longPressTime}, //
          isInverted{isInverted}        //
    {
        buttonGpio.init(gpio_mode_t::GPIO_MODE_INPUT);
        buttonGpio.setPullMode(isInverted ? GPIO_PULLDOWN_ONLY : GPIO_PULLUP_ONLY);
    };

    void update(Time timePassed);

    /// returns true if button state is long pressed at the moment
    [[nodiscard]] bool isPressing() const;

private:
    void resetTimer();
    void updateTimer(Time timePassed);
    [[nodiscard]] Time getPassedTime() const;

    Gpio &buttonGpio;

    Callback buttonCallback;
    const Time DebounceTime = 20.0_ms;
    const Time LongPressTime = 500.0_ms;
    bool isInverted = false;

    enum class InternalState
    {
        Idle,
        Pressed,
        LongPress
    };

    InternalState internalState = InternalState::Idle;
    Time pressTimer{};
};