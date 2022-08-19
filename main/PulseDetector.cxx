#include "PulseDetector.hpp"
#include "Timebase.hpp"
#include "helpers/freertos.hpp"
#include "wrappers/sync.hpp"

#include "esp_log.h"

using namespace util::wrappers;

[[noreturn]] void PulseDetector::taskMain()
{
    sync::waitForAll(sync::TimeIsSynchronized);

    auto lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        pulseInput.update(UpdateTime);

        vTaskDelayUntil(&lastWakeTime, toOsTicks(UpdateTime));
    }
}

//--------------------------------------------------------------------------------------------------
void PulseDetector::pulseCallback(Button::Action action)
{
    switch (action)
    {
    case Button::Action::ShortPress:
    {
        ESP_LOGI("pulseDetector", "valid pulse arrived");
        Timebase::printSystemTime();

        // ToDo: send to queue
    }
    break;

    case Button::Action::LongPress:
        ESP_LOGE("pulseDetector", "Long press arrived. This was not expected in this case. ");
        break;

    case Button::Action::StopLongPress:
        ESP_LOGW("pulseDetector", "Long press stopped.");
        break;

    default:
        break;
    }
}