#include "button.h"

namespace esphome {
namespace kenwood_xs {

void KenwoodXSButton::press_action() {
  if (this->parent_ != nullptr) {
    this->parent_->send_command(this->command_);
  }
}

}  // namespace kenwood_xs
}  // namespace esphome
