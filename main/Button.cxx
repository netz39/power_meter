#include "Button.hpp"

bool Button::isPressing() const
{
    return (internalState == InternalState::LongPress);
}

//--------------------------------------------------------------------------------------------------
void Button::update(const units::si::Time timePassed)
{
    // logical XORing pin state with inverted state
    State state = (buttonGpio.read() != isInverted) ? State::NotPressed : State::Pressed;

    switch (internalState)
    {
    case InternalState::Idle:
        if (state == State::Pressed)
        {
            internalState = InternalState::Pressed;
            resetTimer();
        }
        break;

    case InternalState::Pressed:
        updateTimer(timePassed);

        if (state == State::NotPressed)
        {
            if (getPassedTime() >= DebounceTime)
                buttonCallback(Action::ShortPress);

            internalState = InternalState::Idle;
        }
        else if (getPassedTime() >= LongPressTime)
        {
            buttonCallback(Action::LongPress);
            internalState = InternalState::LongPress;
        }
        break;

    case InternalState::LongPress:
        if (state == State::NotPressed)
        {
            buttonCallback(Action::StopLongPress);
            internalState = InternalState::Idle;
        }
        break;
    }
}

//--------------------------------------------------------------------------------------------------
void Button::resetTimer()
{
    pressTimer = 0.0_s;
}

//--------------------------------------------------------------------------------------------------
void Button::updateTimer(const units::si::Time timePassed)
{
    pressTimer += timePassed;
}

//--------------------------------------------------------------------------------------------------
units::si::Time Button::getPassedTime() const
{
    return pressTimer;
}
