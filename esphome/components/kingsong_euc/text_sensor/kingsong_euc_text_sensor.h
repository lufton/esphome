#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../const.h"
#include "../kingsong_euc.h"
#include "../kingsong_euc_component.h"

#define REGISTER_TEXT_SENSOR(name) \
 protected: \
  KingSongEUCTextSensor *name##_text_sensor_{nullptr}; \
\
 public: \
  void set_##name##_text_sensor(KingSongEUCTextSensor *text_sensor) { \
    this->name##_text_sensor_ = text_sensor; \
    this->text_sensors_[#name] = text_sensor; \
    text_sensor->set_parent(this); \
    text_sensor->set_type(#name); \
  }

namespace esphome {
namespace kingsong_euc {

class KingSongEUCTextSensor : public text_sensor::TextSensor, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_TEXT_SENSOR("  ", this->get_type().c_str(), this); }
  void set_state(std::string state) {
    if (this->state != state)
      return this->publish_state(state);
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
