#include "kenwood_xs.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kenwood_xs {

static const char *const TAG = "kenwood_xs";

KenwoodXSComponent::KenwoodXSComponent(uint8_t ctrl_pin, uint8_t sdat_pin)
    : ctrl_pin_(ctrl_pin), sdat_pin_(sdat_pin), kenwood_(ctrl_pin, sdat_pin) {}

void KenwoodXSComponent::setup() {
  this->kenwood_.begin(this->bidirectional_ ? KENWOOD_XS_BIDIRECTIONAL : KENWOOD_XS_TRANSMIT_ONLY);
}

void KenwoodXSComponent::loop() {
  if (!this->bidirectional_) {
    return;
  }

  byte command;
  if (this->kenwood_.readCommand(command)) {
    this->publish_received_command_(command);
  }
}

void KenwoodXSComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Kenwood XS:");
  ESP_LOGCONFIG(TAG, "  CTRL Pin: GPIO%u", this->ctrl_pin_);
  ESP_LOGCONFIG(TAG, "  SDAT Pin: GPIO%u", this->sdat_pin_);
  ESP_LOGCONFIG(TAG, "  Bidirectional: %s", YESNO(this->bidirectional_));
}

void KenwoodXSComponent::send_command(uint8_t command) {
  ESP_LOGD(TAG, "Sending command %u (0x%02X)", command, command);
  this->kenwood_.sendCommand(command);
}

const char *KenwoodXSComponent::command_name_(uint8_t command) const {
  switch (command) {
    case POWER_ON:
      return "POWER_ON";
    case POWER_OFF:
      return "POWER_OFF";
    case PLUS_10:
      return "PLUS_10";
    case TAPE_2_ON:
      return "TAPE_2_ON";
    case TAPE_2_OFF:
      return "TAPE_2_OFF";
    case VIDEO_2:
      return "VIDEO_2";
    case PHONO:
      return "PHONO";
    case TAPE_1:
      return "TAPE_1";
    case VIDEO_1:
      return "VIDEO_1";
    case TUNER:
      return "TUNER";
    case CD:
      return "CD";
    case TA_FAST_FORWARD:
      return "TA_FAST_FORWARD";
    case TA_STOP:
      return "TA_STOP";
    case TA_PLAY:
      return "TA_PLAY";
    case TA_RECORD:
      return "TA_RECORD";
    case TA_REWIND:
      return "TA_REWIND";
    case TA_PAUSE:
      return "TA_PAUSE";
    case TA_REWIND_SLOW:
      return "TA_REWIND_SLOW";
    case TB_FAST_FORWARD:
      return "TB_FAST_FORWARD";
    case TB_STOP:
      return "TB_STOP";
    case TB_PLAY:
      return "TB_PLAY";
    case TB_RECORD:
      return "TB_RECORD";
    case TB_REWIND:
      return "TB_REWIND";
    case TB_PAUSE:
      return "TB_PAUSE";
    case TB_REWIND_SLOW:
      return "TB_REWIND_SLOW";
    case CD_PLAY_PAUSE:
      return "CD_PLAY_PAUSE";
    case CD_STOP:
      return "CD_STOP";
    case CD_DISC:
      return "CD_DISC";
    case CD_FAST_FORWARD:
      return "CD_FAST_FORWARD";
    case CD_REWIND:
      return "CD_REWIND";
    case CD_NEXT_TRACK:
      return "CD_NEXT_TRACK";
    case CD_PREV_TRACK:
      return "CD_PREV_TRACK";
    case CD_SCRUB_STOP:
      return "CD_SCRUB_STOP";
    case CD_START:
      return "CD_START";
    default:
      return "UNKNOWN";
  }
}

void KenwoodXSComponent::publish_received_command_(uint8_t command) {
  ESP_LOGD(TAG, "Received command %u (0x%02X)", command, command);

  if (this->last_command_sensor_ != nullptr) {
    this->last_command_sensor_->publish_state(command);
  }

  if (this->last_command_text_sensor_ != nullptr) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s (%u / 0x%02X)", this->command_name_(command), command, command);
    this->last_command_text_sensor_->publish_state(buffer);
  }
}

}  // namespace kenwood_xs
}  // namespace esphome
