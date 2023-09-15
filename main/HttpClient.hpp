#pragma once

#include "wrappers/Task.hpp"

#include "esp_event.h"
#include "esp_http_client.h"

#include "Timebase.hpp"

#include <array>

class HttpClient : public util::wrappers::TaskWithMemberFunctionBase
{
    static constexpr auto PrintTag = "[HttpClient]";

public:
    HttpClient(bool &isConnectedToWifi, Timebase::TimestampQueue &timestampQueue)
        : TaskWithMemberFunctionBase("httpClientTask", 2048, osPriorityNormal2), //
          isConnectedToWifi(isConnectedToWifi),                                  //
          timestampQueue(timestampQueue)                                         //
          {};

    bool postDataAsJson(std::string &data);

    static esp_err_t httpEventHandler(esp_http_client_event_t *event);

    class EspClient
    {
    public:
        EspClient(esp_http_client_config_t &config) : client(esp_http_client_init(&config)){};

        ~EspClient()
        {
            esp_http_client_cleanup(client);
        }

        esp_http_client_handle_t &getHandle()
        {
            return client;
        }

    private:
        esp_http_client_handle_t client;
    };

protected:
    void taskMain(void *) override;

private:
    bool &isConnectedToWifi;
    Timebase::TimestampQueue &timestampQueue;
};
