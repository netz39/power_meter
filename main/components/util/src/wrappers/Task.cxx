#include "wrappers/Task.hpp"
#include <algorithm>

namespace util::wrappers
{
size_t Task::taskListIndex{0};
std::array<TaskHandle_t, Task::MaxTasks> Task::taskList{};
EventGroupHandle_t Task::syncEventGroup = xEventGroupCreate();

Task::Task(TaskFunction_t taskCode, const char *name, uint16_t stackDepth, void *parameter,
           UBaseType_t priority)
    : taskCode(taskCode), parameters(parameter)
{
    assert(taskCode != nullptr);

    const size_t StackSizeInBytes = stackDepth * 4; // ESP-IDF want bytes, not words as stack size
    xTaskCreate(&Task::taskMain, name, StackSizeInBytes, reinterpret_cast<void *>(this), priority,
                &taskHandle);

    assert(taskHandle != nullptr);
    registerTask(taskHandle);
}

void Task::registerTask(TaskHandle_t handle)
{
    if (taskListIndex == 0)
    {
        std::fill(taskList.begin(), taskList.end(), nullptr);
    }

    assert(taskListIndex < taskList.size());
    taskList.at(taskListIndex++) = handle;
}

BaseType_t Task::notifyWait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit,
                            uint32_t *pulNotificationValue, TickType_t xTicksToWait)
{
    return xTaskNotifyWait(ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue,
                           xTicksToWait);
}

BaseType_t Task::notify(uint32_t ulValue, eNotifyAction eAction)
{
    return xTaskNotify(taskHandle, ulValue, eAction);
}

BaseType_t Task::notifyFromISR(uint32_t ulValue, eNotifyAction eAction,
                               BaseType_t *pxHigherPriorityTaskWoken)
{

    return xTaskNotifyFromISR(taskHandle, ulValue, eAction, pxHigherPriorityTaskWoken);
}

Task::~Task()
{
    if (taskHandle != nullptr)
    {
        vTaskDelete(taskHandle);
    }
}

[[noreturn]] void Task::taskMain(void *instance)
{
    Task *task = reinterpret_cast<Task *>(instance);
    xEventGroupWaitBits(syncEventGroup, AllTasksWaitFlag, pdFALSE, pdFALSE, portMAX_DELAY);
    task->taskCode(task->parameters);
    for (;;)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

void Task::notifyGive()
{
    xTaskNotifyGive(taskHandle);
}

void Task::notifyTake(const TickType_t waittime)
{
    ulTaskNotifyTake(pdTRUE, waittime);
}

void Task::applicationIsReadyStartAllTasks()
{
    xEventGroupSetBits(syncEventGroup, AllTasksWaitFlag);
}

} // namespace util::wrappers