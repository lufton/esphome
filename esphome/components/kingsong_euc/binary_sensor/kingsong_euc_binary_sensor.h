#pragma once

// #include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCBinarySensor : public binary_sensor::BinarySensor, public KingSongEUCComponent {

 public:
  void dump_config() { LOG_BINARY_SENSOR("  ", this->get_type().c_str(), this); }

  void update() override {
    if (!this->is_connected())
      return;
    if (this->get_last_updated() > 0)
      return;
    // if (this->get_type() == "lock") this->get_parent()->send_request_(CMD_GET_LOCK);
  }

  void publish_state(bool state) {
    if (!this->is_state_expired() && this->state == state) return;
    binary_sensor::BinarySensor::publish_state(state);
    this->just_updated();
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
