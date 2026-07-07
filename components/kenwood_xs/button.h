#pragma once

#include "kenwood_xs.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace kenwood_xs {

class KenwoodXSButton : public button::Button {
 public:
  void set_parent(KenwoodXSComponent *parent) { this->parent_ = parent; }
  void set_command(uint8_t command) { this->command_ = command; }

 protected:
  void press_action() override;

  KenwoodXSComponent *parent_{nullptr};
  uint8_t command_{0};
};

}  // namespace kenwood_xs
}  // namespace esphome
