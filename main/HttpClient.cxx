#include "helpers/freertos.hpp"
#include "wrappers/sync.hpp"

#include "HttpClient.hpp"
#include "esp_log.h"

using namespace util::wrappers;

namespace
{
constexpr auto Tag = "HttpClient";
constexpr auto DelayBetweenRetries = 5.0_s;

constexpr auto EndpointName = "10.0.0.20";
constexpr auto EndpointPort = 8080;
constexpr auto EndpointPath = "/pulse";
constexpr auto AuthKey = "Bearer abc";

} // namespace

//--------------------------------------------------------------------------------------------------
void HttpClient::taskMain()
{
    sync::waitForAll(sync::TimeIsSynchronized);
    ESP_LOGI(Tag, "Wait for pulses to post.");

    while (true)
    {
        // we only peek from queue and drop it after successful post to rest endpoint
        auto result = timestampQueue.peek(portMAX_DELAY);

        if (!isConnectedToWifi)
            sync::waitForAll(sync::ConnectedToWifi);

        if (result.has_value())
        {
            std::string dataToSend;
            dataToSend += "{ \"timestamp\": \"";
            dataToSend += Timebase::getISOTimestamp<std::chrono::milliseconds>(result.value());
            dataToSend += "\" }";

            if (postDataAsJson(dataToSend))
            {
                // send was successful, drop from queue
                timestampQueue.receive(0);
            }
            else
            {
                ESP_LOGI(Tag, "Send was not successful. Resend it in %d seconds.",
                         DelayBetweenRetries.getMagnitude<size_t>());

                vTaskDelay(toOsTicks(DelayBetweenRetries)); // wait a little bit until next retry
                ESP_LOGI(Tag, "Resending");
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
bool HttpClient::postDataAsJson(std::string &data)
{
    esp_http_client_config_t config = {
        .host = EndpointName,
        .port = EndpointPort,
        .path = EndpointPath,
        //.disable_auto_redirect = true,
        .event_handler = HttpClient::httpEventHandler,
    };

    EspClient espClient(config);

    esp_http_client_set_method(espClient.getHandle(), HTTP_METHOD_POST);
    esp_http_client_set_header(espClient.getHandle(), "Content-Type", "application/json");
    esp_http_client_set_header(espClient.getHandle(), "Authorization", AuthKey);
    esp_http_client_set_post_field(espClient.getHandle(), data.data(), data.size());

    esp_err_t err = esp_http_client_perform(espClient.getHandle());

    if (err == ESP_OK)
    {
        auto httpStatusCode = esp_http_client_get_status_code(espClient.getHandle());
        auto contentLength = esp_http_client_get_content_length(espClient.getHandle());

        switch (httpStatusCode)
        {
        case 201:
            ESP_LOGI(Tag, "POST was successful. HTTP status: %d", httpStatusCode);
            return true;

        case 400:
        case 401:
            ESP_LOGE(Tag, "HTTP status: %d, content_length = %d", httpStatusCode, contentLength);
            return true; // becaus we want to reject this timestamp

        case 504:
        default:
            ESP_LOGW(Tag, "HTTP status: %d, content_length = %d", httpStatusCode, contentLength);
            return false;
        }
    }

    ESP_LOGE(Tag, "HTTP POST request failed: %s", esp_err_to_name(err));
    return false;
}

//--------------------------------------------------------------------------------------------------
esp_err_t HttpClient::httpEventHandler(esp_http_client_event_t *event)
{
    switch (event->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(Tag, "HTTP_EVENT_ERROR");
        break;

    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(Tag, "HTTP_EVENT_ON_CONNECTED");
        break;

    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(Tag, "HTTP_EVENT_HEADER_SENT");
        break;

    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(Tag, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", event->header_key,
                 event->header_value);
        break;

    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(Tag, "HTTP_EVENT_ON_DATA, len=%d", event->data_len);
        break;

    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(Tag, "HTTP_EVENT_ON_FINISH");
        break;

    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(Tag, "HTTP_EVENT_DISCONNECTED");
        break;
    }

    return ESP_OK;
}