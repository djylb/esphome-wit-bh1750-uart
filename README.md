# ESPHome External Component - WIT BH1750 UART Light Sensor

This is a custom ESPHome component to read light intensity from a **WIT BH1750 sensor** connected via UART.

## Usage

Add the following to your ESPHome YAML:

```yaml
external_components:
  - source: github://djylb/esphome-wit-bh1750-uart

uart:
  id: uart_bus
  rx_pin: GPIO13
  baud_rate: 9600

sensor:
  - platform: wit_bh1750_uart
    name: "Light Intensity"
```
