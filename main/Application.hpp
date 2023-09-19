#pragma once

#include "HttpClient.hpp"
#include "PulseDetector.hpp"
#include "StatusLed.hpp"
#include "Timebase.hpp"
#include "Wireless.hpp"

#include "helpers/freertos.hpp"

class Application
{
public:
    static constexpr auto PrintTag = "[Application]";

    Application()
    {
        assert(instance == nullptr);
        instance = this;
        timeoutTimer = xTimerCreate("timeoutTimer", toOsTicks(10.0_s), pdFALSE, nullptr, onTimeout);
    }

    [[noreturn]] void run();

    static Application &getApplicationInstance();

    static void onTimeout(TimerHandle_t);

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
    inline static TimerHandle_t timeoutTimer = nullptr;

    static void stopTimer()
    {
        xTimerStop(timeoutTimer, 0);
    }

    static void resetTimer()
    {
        xTimerReset(timeoutTimer, 0);
    }
};