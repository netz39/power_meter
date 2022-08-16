#pragma once
#include "IMutex.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace util::wrappers
{
/**
 * Wrapper for FreeRTOS non-recursive mutex
 */
class Mutex : public IMutex
{
public:
    Mutex();
    virtual ~Mutex() = default;

    virtual void lock() override;
    virtual bool lockWithTimeout(TickType_t timeToWait) override;
    virtual void unlock() override;

private:
    SemaphoreHandle_t mutexHandle;
};
} // namespace util::wrappers