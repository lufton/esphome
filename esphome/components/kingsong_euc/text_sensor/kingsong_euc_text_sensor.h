#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_TEXT_SENSOR(name) \
 protected: \
  KingSongEUCTextSensor *name##_text_sensor_{nullptr}; \
\
 public: \
  void set_##name##_text_sensor(KingSongEUCTextSensor *text_sensor) { \
    this->name##_text_sensor_ = text_sensor; \
    text_sensor->set_parent(this); \
    text_sensor->set_type(#name); \
  }

class KingSongEUCTextSensor : public text_sensor::TextSensor, public KingSongEUCComponent {

 public:

  void dump_config() { LOG_TEXT_SENSOR("  ", this->get_type().c_str(), this); }

  void publish_state(const std::string &state) {
    if (!this->is_state_expired() && this->state == state) return;
    text_sensor::TextSensor::publish_state(state);
    this->just_updated();
  }

  void update() override {
    if (!this->is_connected())
      return;
    if (this->get_last_updated() > 0)
      return;
    if (this->get_type() == "model")
      this->get_parent()->get_model();
    else if (this->get_type() == "serial")
      this->get_parent()->get_serial();
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
