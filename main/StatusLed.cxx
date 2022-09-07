#include "StatusLed.hpp"

#include "helpers/freertos.hpp"

void StatusLed::taskMain()
{
    initLedPwm();

    while (true)
    {
        if (!isConnected)
        {
            setLedDuty(100);
            vTaskDelay(toOsTicks(300.0_ms));

            setLedDuty(0);
            vTaskDelay(toOsTicks(300.0_ms));
        }
        else if (pulseArrived)
        {
            setLedDuty(50);
            vTaskDelay(pdMS_TO_TICKS(700));
            pulseArrived = false;
        }
        else
        {
            setLedDuty(5);
            vTaskDelay(toOsTicks(50.0_ms));
        }
    }
}

//--------------------------------------------------------------------------------------------------
void StatusLed::initLedPwm()
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledcTimer = {.speed_mode = PwmMode,
                                     .duty_resolution = PwmResolution,
                                     .timer_num = LEDC_TIMER_0,
                                     .freq_hz = 5000, // frequency in Hz
                                     .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledcTimer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledcChannel = {.gpio_num = settings::StatusLedGpio,
                                         .speed_mode = PwmMode,
                                         .channel = LedChannel,
                                         .intr_type = LEDC_INTR_DISABLE,
                                         .timer_sel = LEDC_TIMER_0,
                                         .duty = 0, // Set duty to 0%
                                         .hpoint = 0,
                                         .flags = {0}};
    ESP_ERROR_CHECK(ledc_channel_config(&ledcChannel));
}

//--------------------------------------------------------------------------------------------------
void StatusLed::setLedDuty(uint8_t percentage)
{
    const uint32_t Duty = uint32_t(percentage) * uint32_t(MaximumDuty) / 100;

    ledc_set_duty(PwmMode, LedChannel, Duty);
    ledc_update_duty(PwmMode, LedChannel);
}