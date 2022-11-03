# power meter (firmware for ESP32)
[![Build status](https://github.com/netz39/power_meter/actions/workflows/build.yml/badge.svg)](https://github.com/netz39/power_meter/actions/workflows/build.yml)

The ESP32 detect the pulses outputing by a power meter and send it to an endpoint (REST-API). The endpoint used in Netz39 is implemented **[here](https://github.com/netz39/power-meter-pulse-gateway)**.

## Configuration

Currently the configurations are hardcoded. In the future you may change it to dynamic settings configuring over TCP or UART.

For `WifiSsid`, `WifiPassword` and `Authkey` create a file named `loginData.hpp` in `main` folder. This file is in the gitignore.
```cpp
// both must be defined with #define
#define WifiSsid "Ssid"
#define WifiPassword "password"

// must be constexpr instead #define
constexpr auto AuthKey = "Bearer abc";
```

Here is an overview of constants you can find in `settings.hpp`:

| setting name       | description |
|--------------------|-------------|
| EndpointUrl | endpoint URL in URI schemata |
| DelayBetweenRetries | delay between post retries after any failure |
| MinimumPulseLength | Specifies the minimum pulse length of power meter |
| DetectorUpdateTime | polling time of pulse detector, must be an even divisor to `MinimumPulseLength` |
| PulseInputPin      | pin number, at where the ESP is connected to power meters output |
| StatusLedGpio      | pin number, at where the LED is connected |

## API

### Pulse Message

Pulses are encoded as messages with an [ISO8601 timestamp](https://en.wikipedia.org/wiki/ISO_8601) in the JSON form:
```json
{
  "timestamp": "2022-08-22T02:57:55,123Z"
}
```

### REST

At every detected pulse , a message will post to the given endpoint with the above pulse message in JSON form.

## Deployment

### Development

The code is written in C++17 and uses the [ESP-IDF](https://github.com/espressif/esp-idf).


## Build

Please consult the [documention](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#build-the-project) of ESP-IDF to compile that project. 

## Firmware structure
This firmware contains the logic code written in C++.

This firmware is designed to work without global variables and so-called "externs" declarations. This is also known as [Dependency injection](https://en.wikipedia.org/wiki/Dependency_injection).
To implement this, an application class is created, from there finally the whole classes with the logic code and also tasks are created.

With this structure one can clearly understand, which dependencies between the objects exist. This handling is especially important for unit testing.

## RTOS Tasks
RTOS tasks are integrated in classes. These classes derive from a base class, which automatically creates a task when constructing the class. With this a class has to override its member `taskMain` function. After Application class is completely finished with the initializing the tasks will be called.

| classes with RTOS task     | description |
|-------------------|-------------|
| HttpClient      | new pulse timestamps in queue will be posted to REST-API, failures will be treated|
| PulseDetector   | polling pulse input and put timestamp to queue at valid pulse |
| StatusLed       | controls status LED according to current state  |
| Wireless        | ensure that a connection to Wifi-AP is available |

## Maintainers

* Maximilian Grau ([@MG-5](https://github.com/mg-5))

## License

[MIT](LICENSE) © 2022 Netz39 e.V. and contributors
