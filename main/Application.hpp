#pragma once

#include "PulseDetector.hpp"
#include "StatusLed.hpp"
#include "Wireless.hpp"

class Application
{
public:
    Application()
    {
        assert(instance == nullptr);
        instance = this;
    }

    void run();

    static Application &getApplicationInstance();

private:
    bool isConnected = false;

    Wireless wireless{isConnected};
    StatusLed statusLed{isConnected};
    PulseDetector pulseDetector{};

    static inline Application *instance{nullptr};
};