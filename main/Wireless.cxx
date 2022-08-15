#include "Wireless.hpp"
#include "loginData.hpp"
#include "wrappers/sync.hpp"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"

#include <cstring>

using namespace util::wrappers;

//--------------------------------------------------------------------------------------------------
void Wireless::taskMain()
{
    startNvs();
    startStation();

    while (true)
    {
        if (isConnected)
        {
            sync::waitForAll(sync::ConnectionFailed);
            isConnected = false;
        }
        else
        {
            sync::waitForAll(sync::ConnectedToWifi);
            isConnected = true;
        }
    }
}

//--------------------------------------------------------------------------------------------------
void Wireless::startNvs()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

//--------------------------------------------------------------------------------------------------
void Wireless::startStation()
{
    // init TCP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // create event loop to get actions like connected/fail to connect
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instanceAnyId;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &Wireless::eventHandler, NULL, &instanceAnyId));

    esp_event_handler_instance_t instanceGotIp;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &Wireless::eventHandler, NULL, &instanceGotIp));

    wifi_sta_config_t staConfig{WifiSsid, WifiPassword};
    staConfig.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    staConfig.pmf_cfg.capable = true;
    staConfig.pmf_cfg.required = false;

    wifi_config_t wifiConfig;
    wifiConfig.sta = staConfig;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());
}

//--------------------------------------------------------------------------------------------------
void Wireless::eventHandler(void *arg, esp_event_base_t eventBase, int32_t eventId, void *eventData)
{
    static bool firstReconnect = true;

    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
        ESP_LOGI("eventHandler", "WIFI_EVENT_STA_START");
    }

    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (!firstReconnect)
            vTaskDelay(pdMS_TO_TICKS(5000));

        sync::clearEvents(sync::ConnectedToWifi);
        sync::signal(sync::ConnectionFailed);

        firstReconnect = false;
        ESP_LOGI("eventHandler", "try to reconnnect");
        esp_wifi_connect();
    }
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP)
    {
        // ip_event_got_ip_t *event = static_cast<ip_event_got_ip_t *>(eventData);
        ESP_LOGI("eventHandler", "Established a wifi connection to %s", WifiSsid);
        firstReconnect = true;

        sync::clearEvents(sync::ConnectionFailed);
        sync::signal(sync::ConnectedToWifi);
    }
}