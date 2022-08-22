#pragma once

#include "esp_log.h"
#include "esp_sntp.h"

#include "wrappers/Queue.hpp"
#include "wrappers/sync.hpp"

#include "date/date.h"
#include <chrono>

using namespace util::wrappers;

// CAUTION, this is not Y2K38-issue aware, see
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html#bit-time-t
// for more details and possible workaround
// (at the moment of writing for fixing its needed compiling your own toolchain from stratch, why??)

/// This ensures we are getting the current UTC from time server.
/// Time synchronization will happens every
/// [CONFIG_LWIP_SNTP_UPDATE_DELAY] milliseconds (see sdkconfig)
class Timebase
{
public:
    static constexpr auto PrintTag = "Timebase";
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
    using TimestampQueue = util::wrappers::Queue<Timebase::Timestamp>;

    //--------------------------------------------------------------------------------------------------
    static void initTimeSychronization()
    {
        ESP_LOGI(PrintTag, "Initializing SNTP");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_set_time_sync_notification_cb(timeSynchronizationCallback);
        sntp_init();
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
                 getISOCurrentTimestamp<std::chrono::milliseconds>().c_str());
    }

    //--------------------------------------------------------------------------------------------------
    [[nodiscard]] static Timestamp getCurrentTimestamp()
    {
        return std::chrono::system_clock::now();
    }

    //--------------------------------------------------------------------------------------------------
    template <class Precision>
    [[nodiscard]] static std::string getISOCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        return date::format("%FT%TZ", date::floor<Precision>(now));
    }

    //--------------------------------------------------------------------------------------------------
    template <class Precision>
    [[nodiscard]] static std::string getISOTimestamp(Timestamp &now)
    {
        return date::format("%FT%TZ", date::floor<Precision>(now));
    }
};
