#pragma once

#include <KenwoodXS.h>
#include "esphome/core/component.h"

namespace esphome {
namespace kenwood_xs {

class KenwoodXSComponent : public Component {
 public:
  KenwoodXSComponent(uint8_t ctrl_pin, uint8_t sdat_pin);

  void setup() override;
  void dump_config() override;

  void send_command(uint8_t command);

 protected:

  uint8_t ctrl_pin_;
  uint8_t sdat_pin_;
  KenwoodXS kenwood_;
};

}  // namespace kenwood_xs
}  // namespace esphome
