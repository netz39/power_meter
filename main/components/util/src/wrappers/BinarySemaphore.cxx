#include "wrappers/BinarySemaphore.hpp"

namespace util::wrappers
{

BinarySemaphore::BinarySemaphore() : semaphoreHandle(xSemaphoreCreateBinary())
{
    assert(semaphoreHandle != nullptr);
}

BinarySemaphore::~BinarySemaphore()
{
    if (semaphoreHandle != nullptr)
    {
        vSemaphoreDelete(semaphoreHandle);
    }
}

BaseType_t BinarySemaphore::take(TickType_t blocktime)
{
    return xSemaphoreTake(semaphoreHandle, blocktime);
}

BaseType_t BinarySemaphore::giveFromISR(BaseType_t *pxHigherPriorityTaskWoken)
{
    return xSemaphoreGiveFromISR(semaphoreHandle, pxHigherPriorityTaskWoken);
}
BaseType_t BinarySemaphore::give()
{
    return xSemaphoreGive(semaphoreHandle);
}

} // namespace util::wrappers