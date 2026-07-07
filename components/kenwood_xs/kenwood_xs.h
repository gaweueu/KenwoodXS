#pragma once

#include <KenwoodXS.h>
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace kenwood_xs {

class KenwoodXSComponent : public Component {
 public:
  KenwoodXSComponent(uint8_t ctrl_pin, uint8_t sdat_pin);

  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_bidirectional(bool bidirectional) { this->bidirectional_ = bidirectional; }
  void set_last_command_sensor(sensor::Sensor *sensor) { this->last_command_sensor_ = sensor; }
  void set_last_command_text_sensor(text_sensor::TextSensor *sensor) { this->last_command_text_sensor_ = sensor; }

  void send_command(uint8_t command);

 protected:
  const char *command_name_(uint8_t command) const;
  void publish_received_command_(uint8_t command);

  uint8_t ctrl_pin_;
  uint8_t sdat_pin_;
  bool bidirectional_{true};
  KenwoodXS kenwood_;
  sensor::Sensor *last_command_sensor_{nullptr};
  text_sensor::TextSensor *last_command_text_sensor_{nullptr};
};

}  // namespace kenwood_xs
}  // namespace esphome
