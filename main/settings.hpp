#pragma once

#include "units/si/time.hpp"
#include "wrappers/gpio.hpp"

namespace settings
{
// HTTP stuff
constexpr auto EndpointName = "radon";
constexpr auto EndpointPort = 9003;
constexpr auto EndpointPath = "/pulse";
constexpr auto DelayBetweenRetries = 10.0_s;

// PulseDetector
constexpr auto MinimumPulseLength = 30.0_ms;
constexpr auto DetectorUpdateTime = 10.0_ms;
constexpr auto PulseInputPin = gpio_num_t::GPIO_NUM_12;

constexpr auto StatusLedGpio = gpio_num_t::GPIO_NUM_19;

} // namespace settings