#pragma once

#include "esp_log.h"
#include "esp_sntp.h"

#include "sync.hpp"
#include "wrappers/Queue.hpp"

#include "date/date.h"
#include <chrono>

using namespace util::wrappers;

/// This ensures we are getting the current UTC from time server. This is Y2K38-issue aware because
/// it is using 64-bit time representation.
/// Time synchronization will happens every [CONFIG_LWIP_SNTP_UPDATE_DELAY]
/// milliseconds (see sdkconfig)
class Timebase
{
public:
    static constexpr auto PrintTag = "[Timebase]";
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
    using TimestampQueue = util::wrappers::Queue<Timebase::Timestamp>;

    //--------------------------------------------------------------------------------------------------
    static void initTimeSychronization()
    {
        esp_sntp_stop();
        ESP_LOGI(PrintTag, "Initializing SNTP");
        esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
        esp_sntp_setservername(0, "pool.ntp.org");
        esp_sntp_set_time_sync_notification_cb(timeSynchronizationCallback);
        esp_sntp_init();
        ESP_LOGI(PrintTag, "Update system time every %d minutes.",
                 CONFIG_LWIP_SNTP_UPDATE_DELAY / 1000 / 60);
    }

    //--------------------------------------------------------------------------------------------------
    static void timeSynchronizationCallback(struct timeval *)
    {
        ESP_LOGI(PrintTag, "Time synchronization event arrived.");
        printSystemTime();
        sync::signal(sync::TimeIsSynchronized);
    }

    //--------------------------------------------------------------------------------------------------
    static void printSystemTime()
    {
        ESP_LOGI(PrintTag, "The current UTC is %s",
                 getISOTimestamp<std::chrono::milliseconds>(getCurrentTimestamp()).c_str());
    }

    //--------------------------------------------------------------------------------------------------
    [[nodiscard]] static Timestamp getCurrentTimestamp()
    {
        return std::chrono::system_clock::now();
    }

    //--------------------------------------------------------------------------------------------------
    template <class Precision>
    [[nodiscard]] static std::string getISOTimestamp(Timestamp now)
    {
        return date::format("%FT%TZ", date::floor<Precision>(now));
    }
};
