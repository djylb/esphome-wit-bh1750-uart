import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    UNIT_LUX,
    DEVICE_CLASS_ILLUMINANCE,
    STATE_CLASS_MEASUREMENT,
    ICON_BRIGHTNESS_5,
)

# 定义命名空间
wit_bh1750_uart_ns = cg.esphome_ns.namespace("wit_bh1750_uart")
CustomLightSensorUART = wit_bh1750_uart_ns.class_(
    "CustomLightSensorUART", cg.Component, uart.UARTDevice, sensor.Sensor
)

# 传感器配置
CONFIG_SCHEMA = sensor.sensor_schema(
    CustomLightSensorUART,
    unit_of_measurement=UNIT_LUX,
    icon=ICON_BRIGHTNESS_5,
    device_class=DEVICE_CLASS_ILLUMINANCE,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(
    cv.COMPONENT_SCHEMA
).extend(
    uart.UART_DEVICE_SCHEMA
)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
