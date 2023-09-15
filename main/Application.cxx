#include "Application.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include <memory>

// called by ESP-IDF
extern "C" void app_main(void)
{
    static constexpr auto PrintTag = "[Application]";

    ESP_LOGI(PrintTag, "Start application.");
    ESP_LOGI(PrintTag, "Free memory: %ld bytes", esp_get_free_heap_size());
    ESP_LOGI(PrintTag, "IDF version: %s", esp_get_idf_version());

    static auto app = std::make_unique<Application>();
    app->run();
}

//--------------------------------------------------------------------------------------------------
void Application::run()
{
    util::wrappers::Task::applicationIsReadyStartAllTasks();
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