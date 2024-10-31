#pragma once

// #include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../const.h"
// #include "../kingsong_euc.h"
#include "../kingsong_euc_component.h"

#define REGISTER_BINARY_SENSOR(name) \
 protected: \
  KingSongEUCBinarySensor *name##_binary_sensor_{nullptr}; \
\
 public: \
  void set_##name##_binary_sensor(KingSongEUCBinarySensor *binary_sensor) { \
    this->name##_binary_sensor_ = binary_sensor; \
    this->binary_sensors_[#name] = binary_sensor; \
    binary_sensor->set_parent(this); \
    binary_sensor->set_type(#name); \
  }

namespace esphome {
namespace kingsong_euc {

class KingSongEUCBinarySensor : public binary_sensor::BinarySensor, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_BINARY_SENSOR("  ", this->get_type().c_str(), this); }
  void set_state(bool state) {
    if (this->state != state)
      return this->publish_state(state);
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
