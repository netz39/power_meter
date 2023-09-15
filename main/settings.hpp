#pragma once

#include "units/si/time.hpp"
#include "util/gpio.hpp"

namespace settings
{
// HTTP stuff
constexpr auto EndpointUrl = "https://pwr-meter-pulse-gw-19i.svc.n39.eu/pulse";
constexpr auto DelayBetweenRetries = 10.0_s;

// PulseDetector
constexpr auto MinimumPulseLength = 30.0_ms;
constexpr auto DetectorUpdateTime = 10.0_ms;
constexpr auto PulseInputPin = gpio_num_t::GPIO_NUM_12;

constexpr auto StatusLedGpio = gpio_num_t::GPIO_NUM_19;

} // namespace settings
