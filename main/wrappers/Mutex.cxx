#include "wrappers/Mutex.hpp"

namespace util::wrappers
{
Mutex::Mutex() : m_mutex(xSemaphoreCreateMutex())
{
    assert(m_mutex != nullptr);
}

void Mutex::lock()
{
    xSemaphoreTake(m_mutex, portMAX_DELAY);
}
bool Mutex::lockWithTimeout(TickType_t timeToWait)
{
    return xSemaphoreTake(m_mutex, timeToWait) == pdPASS;
}
void Mutex::unlock()
{
    xSemaphoreGive(m_mutex);
}
} // namespace util::wrappers
