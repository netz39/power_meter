#include "wrappers/Mutex.hpp"

namespace util::wrappers
{
Mutex::Mutex() : mutexHandle(xSemaphoreCreateMutex())
{
    assert(mutexHandle != nullptr);
}

void Mutex::lock()
{
    xSemaphoreTake(mutexHandle, portMAX_DELAY);
}
bool Mutex::lockWithTimeout(TickType_t timeToWait)
{
    return xSemaphoreTake(mutexHandle, timeToWait) == pdPASS;
}
void Mutex::unlock()
{
    xSemaphoreGive(mutexHandle);
}
} // namespace util::wrappers
