#pragma once

#include "esp_log.h"
#include "esp_sntp.h"

#include "wrappers/sync.hpp"

using namespace util::wrappers;

// CAUTION, this is not Y2K38-issue aware, see
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html#bit-time-t
// for more details and possible workaround
// (at the moment of writing for fixing its needed compiling your own toolchain from stratch, why??)

/// This ensures we are getting the current time from time server. 
/// Time synchronization will happens every [CONFIG_LWIP_SNTP_UPDATE_DELAY] milliseconds (see sdkconfig)
class Timebase
{
public:
    static void initTimeSychronization()
    {
        ESP_LOGI("sntp", "Initializing SNTP");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_set_time_sync_notification_cb(timeSynchronizationCallback);
        sntp_init();
        ESP_LOGI("sntp", "Update system time every %d minutes.",
                 CONFIG_LWIP_SNTP_UPDATE_DELAY / 1000 / 60);

        // set timezone to Berlin (incl. winter/summer-time)
        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
        tzset();
    }

    static void timeSynchronizationCallback(struct timeval *)
    {
        ESP_LOGI("sntp", "Time synchronization event arrived.");
        printSystemTime();
        sync::signal(sync::TimeIsSynchronized);
    }

    static void printSystemTime()
    {
        char buffer[64];
        time_t currentTime;
        static tm timeinfo;

        time(&currentTime);
        localtime_r(&currentTime, &timeinfo);

        strftime(buffer, sizeof(buffer), "%c", &timeinfo);
        ESP_LOGI("sntp", "The current date/time in Berlin is: %s", buffer);
        ESP_LOGI("sntp", "The current UTC in milliseconds is: %" PRId64, getUTCinMilliseconds());
    }

    static int64_t getUTCinSeconds()
    {
        struct timeval tvNow;
        gettimeofday(&tvNow, NULL);
        return tvNow.tv_sec;
    }

    static int64_t getUTCinMilliseconds()
    {
        struct timeval tvNow;
        gettimeofday(&tvNow, NULL);
        return (int64_t)tvNow.tv_sec * 1000L + (int64_t)tvNow.tv_usec / 1000L;
    }
};
