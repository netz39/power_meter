#include "Application.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include <memory>

// called by ESP-IDF
extern "C" void app_main(void)
{
    ESP_LOGI(Application::PrintTag, "Start application.");
    ESP_LOGI(Application::PrintTag, "Free memory: %ld bytes", esp_get_free_heap_size());
    ESP_LOGI(Application::PrintTag, "IDF version: %s", esp_get_idf_version());

    static auto app = std::make_unique<Application>();
    app->run();
}

//--------------------------------------------------------------------------------------------------
void Application::run()
{
    util::wrappers::Task::applicationIsReadyStartAllTasks();
    sync::waitForAll(sync::ConnectedToWifi);
    Timebase::initTimeSychronization();
    resetTimer();
    sync::waitForAll(sync::TimeIsSynchronized);
    stopTimer();

    while (true)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

//--------------------------------------------------------------------------------------------------
Application &Application::getApplicationInstance()
{
    // Not constructing Application in this singleton, to avoid bugs where something tries to
    // access this function, while application constructs which will cause infinite recursion
    return *instance;
}

//--------------------------------------------------------------------------------------------------
void Application::onTimeout(TimerHandle_t)
{
    ESP_LOGW(PrintTag, "No response from SNTP. Restart it.");
    Timebase::initTimeSychronization();
    resetTimer();
}