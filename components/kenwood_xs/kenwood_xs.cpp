#include "kenwood_xs.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kenwood_xs {

static const char *const TAG = "kenwood_xs";

KenwoodXSComponent::KenwoodXSComponent(uint8_t ctrl_pin, uint8_t sdat_pin)
    : ctrl_pin_(ctrl_pin), sdat_pin_(sdat_pin), kenwood_(ctrl_pin, sdat_pin) {}

void KenwoodXSComponent::setup() {
  this->kenwood_.begin(KENWOOD_XS_TRANSMIT_ONLY);
}



void KenwoodXSComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Kenwood XS:");
  ESP_LOGCONFIG(TAG, "  CTRL Pin: GPIO%u", this->ctrl_pin_);
  ESP_LOGCONFIG(TAG, "  SDAT Pin: GPIO%u", this->sdat_pin_);
  ESP_LOGCONFIG(TAG, "  Bidirectional: No");
}

void KenwoodXSComponent::send_command(uint8_t command) {
  ESP_LOGD(TAG, "Sending command %u (0x%02X)", command, command);
  this->kenwood_.sendCommand(command);
}

}  // namespace kenwood_xs
}  // namespace esphome
