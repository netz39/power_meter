#pragma once

#include "HttpClient.hpp"
#include "PulseDetector.hpp"
#include "StatusLed.hpp"
#include "Timebase.hpp"
#include "Wireless.hpp"

class Application
{
public:
    Application()
    {
        assert(instance == nullptr);
        instance = this;
    }

    [[noreturn]] void run();

    static Application &getApplicationInstance();

private:
    bool isConnected = false;
    bool pulseArrived = false;

    // 64-bit x 8192 = 64kB
    // with a continous load of 400 Watt, the queue can save timestamps for 20,5 hours
    Timebase::TimestampQueue timestampQueue{8192};

    Wireless wireless{isConnected};
    StatusLed statusLed{isConnected, pulseArrived};
    PulseDetector pulseDetector{timestampQueue, pulseArrived};
    HttpClient restClient{isConnected, timestampQueue};

    static inline Application *instance{nullptr};
};