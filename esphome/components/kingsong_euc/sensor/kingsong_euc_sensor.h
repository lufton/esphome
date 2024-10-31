#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "../const.h"
#include "../kingsong_euc.h"
#include "../kingsong_euc_component.h"

#define REGISTER_SENSOR(name) \
 protected: \
  KingSongEUCSensor *name##_sensor_{nullptr}; \
\
 public: \
  void set_##name##_sensor(KingSongEUCSensor *sensor) { \
    this->name##_sensor_ = sensor; \
    this->sensors_[#name] = sensor; \
    sensor->set_parent(this); \
    sensor->set_type(#name); \
  }

namespace esphome {
namespace kingsong_euc {

class KingSongEUCSensor : public sensor::Sensor, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_SENSOR("  ", this->get_type().c_str(), this); }
  void set_state(float state) {
    if (this->state != state)
      return this->publish_state(state);
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
