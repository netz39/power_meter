#include "PulseDetector.hpp"
#include "Timebase.hpp"
#include "helpers/freertos.hpp"
#include "wrappers/sync.hpp"

#include "esp_log.h"

using namespace util::wrappers;

[[noreturn]] void PulseDetector::taskMain()
{
    sync::waitForAll(sync::TimeIsSynchronized);
    ESP_LOGI(PrintTag, "Start pulse detection.");

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
        ESP_LOGI(PrintTag, "valid pulse arrived");
        pulseArrived = true;
        Timebase::printSystemTime();
        Timebase::Timestamp currentTimestamp = Timebase::getCurrentTimestamp();

        if (!timestampQueue.send(currentTimestamp, 0))
        {
            // queue seems to be full, so we pop the first element and try it again
            ESP_LOGW(PrintTag, "queue is full, overwrite first value");

            timestampQueue.receive(0);
            if (!timestampQueue.send(currentTimestamp, 0))
                ESP_LOGE(PrintTag, "Queue is full and cannot drop old values to get new ones!");
        }
    }
    break;

    case Button::Action::LongPress:
        ESP_LOGE(PrintTag, "Long press arrived. This was not expected in this case. ");
        break;

    case Button::Action::StopLongPress:
        ESP_LOGW(PrintTag, "Long press stopped.");
        break;

    default:
        break;
    }
}