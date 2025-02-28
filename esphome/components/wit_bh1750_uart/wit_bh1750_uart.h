#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace wit_bh1750_uart {

class CustomLightSensorUART : public esphome::Component, public esphome::uart::UARTDevice, public esphome::sensor::Sensor {
 public:
  CustomLightSensorUART() {}

  void setup() override {
    buffer_index_ = 0;
    value_sum_ = 0;
    value_count_ = 0;
    last_read_ = millis();
  }

  void loop() override {
    const uint32_t TIMEOUT_MS = 2000;  // 2秒超时
    const int MIN_COUNT = 3;  // 最少3次平均

    if (millis() - last_read_ > TIMEOUT_MS && value_count_ > 0) {
      publish_average_and_reset();
    }

    while (available()) {
      char c = read();
      last_read_ = millis();

      if (c == 'L') {
        buffer_index_ = 0;
      }

      if (buffer_index_ < sizeof(buffer_) - 1) {
        buffer_[buffer_index_++] = c;
      }

      if (c == '\n') {
        buffer_[buffer_index_] = '\0';
        parse_and_store_value();
        buffer_index_ = 0;
      }
    }
  }

 protected:
  char buffer_[32];
  int buffer_index_ = 0;
  unsigned long last_read_ = 0;
  float value_sum_ = 0;
  int value_count_ = 0;

  void parse_and_store_value() {
    if (strncmp(buffer_, "Ligth:", 6) == 0) {
      float value = atof(buffer_ + 6);
      if (value >= 0 && value <= 65536) {
        value_sum_ += value;
        value_count_++;
        if (value_count_ >= 3) {
          publish_average_and_reset();
        }
      }
    }
  }

  void publish_average_and_reset() {
    if (value_count_ > 0) {
      float average = value_sum_ / value_count_;
      publish_state(average);
      ESP_LOGD("wit_bh1750_uart", "Published average light: %.1f lx", average);
      value_sum_ = 0;
      value_count_ = 0;
    }
  }
};

}  // namespace wit_bh1750_uart
}  // namespace esphome
