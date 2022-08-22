#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace util::wrappers
{

class BinarySemaphore
{
public:
    BinarySemaphore();
    ~BinarySemaphore();

    BinarySemaphore(const BinarySemaphore &) = delete;
    BinarySemaphore(BinarySemaphore &&) = delete;
    BinarySemaphore &operator=(const BinarySemaphore &) = delete;
    BinarySemaphore &operator=(BinarySemaphore &&) = delete;

    BaseType_t take(TickType_t blocktime);
    BaseType_t giveFromISR(BaseType_t *pxHigherPriorityTaskWoken);
    BaseType_t give();

private:
    SemaphoreHandle_t semaphoreHandle{nullptr};
};

} // namespace util::wrappers