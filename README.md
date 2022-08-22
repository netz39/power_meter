# power meter (firmware for ESP32)

The ESP32 should detect the pulses outputing by a power meter and send it to an endpoint (REST-API). The endpoint used in Netz39 is implemented **[here](https://github.com/netz39/power-meter-pulse-gateway)**.

## Configuration

Currently the configuration are hardcoded. In the future you may change it to dynamic settings configuring over TCP or UART.

Here is an overview of neccessary (not all) settings and their file locations:

| setting name       | description | file |
|--------------------|-------------|------|
| WifiSsid      | ssid of wireless network | loginData.hpp (create file and add #define - file is ignored by git) |
| WifiPassword      | password of wireless network | loginData.hpp (create file and add #define - file is ignored by git) |
| MinimumPulseLength | Specifies the minimum pulse length of power meter | PulseDetector.hpp |
| InputPin           | pin number, at where the ESP is connected to power meters output | PulseDetector.hpp |
| DelayBetweenRetries | delay between post retries after any failure | HttpClient.cxx |
| EndpointName | domain or IP address of endpoint | HttpClient.cxx |
| EndpointPort | port number of endpoint | HttpClient.cxx |
| EndpointPath | like `/pulse` | HttpClient.cxx |
| AuthKey      | like `Bearer abc` | HttpClient.cxx |

## API

### Pulse Message

Pulses are encoded as messages with a [ISO8601 timestamp](https://en.wikipedia.org/wiki/ISO_8601) in the JSON form
```json
{
  "timestamp": 2022-08-22T02:57:55,123Z
}
```

### REST

At every detected pulse , a message will post at given endpoint with the above pulse message JSON form.

## Deployment

### Development

The code is written in C++17 and uses the [ESP-IDF](https://github.com/espressif/esp-idf).


## Build

Please consult the [documention](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#build-the-project) of ESP-IDF.


## Maintainers

* Maximilian Grau ([@MG-5](https://github.com/mg-5))

## License

[MIT](LICENSE.txt) © 2022 Netz39 e.V. and contributors
