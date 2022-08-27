# power meter (firmware for ESP32)

The ESP32 detect the pulses outputing by a power meter and send it to an endpoint (REST-API). The endpoint used in Netz39 is implemented **[here](https://github.com/netz39/power-meter-pulse-gateway)**.

## Configuration

Currently the configurations are hardcoded. In the future you may change it to dynamic settings configuring over TCP or UART.

For `WifiSsid`, `WifiPassword` and `Authkey`, you must create a file named `loginData.hpp`, which is ignored by git, and define those constants here.
AuthKey can be  like `Bearer abc`.

Here is an overview of constants you can find in `settings.hpp`:

| setting name       | description |
|--------------------|-------------|
| EndpointName | domain or IP address of endpoint |
| EndpointPort | port number of endpoint |
| EndpointPath | like `/pulse` |
| DelayBetweenRetries | delay between post retries after any failure |
| MinimumPulseLength | Specifies the minimum pulse length of power meter |
| DetectorUpdateTime | polling time of pulse detector, must be an even divisor to `MinimumPulseLength` |
| PulseInputPin      | pin number, at where the ESP is connected to power meters output |
| StatusLedGpio      | pin number, at where the LED is connected |

## API

### Pulse Message

Pulses are encoded as messages with a [ISO8601 timestamp](https://en.wikipedia.org/wiki/ISO_8601) in the JSON form:
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


## Maintainers

* Maximilian Grau ([@MG-5](https://github.com/mg-5))

## License

[MIT](LICENSE) © 2022 Netz39 e.V. and contributors
