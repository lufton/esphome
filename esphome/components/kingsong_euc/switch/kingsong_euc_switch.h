#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/switch/switch.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SWITCH(name) \
 protected: \
  KingSongEUCSwitch *name##_switch_{nullptr}; \
\
 public: \
  void set_##name##_switch(KingSongEUCSwitch *switch_) { \
    this->name##_switch_ = switch_; \
    switch_->set_parent(this); \
    switch_->set_type(#name); \
  }

class KingSongEUCSwitch : public switch_::Switch, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_SWITCH("  ", this->get_type().c_str(), this); }
  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    if (this->get_type() == "lift_sensor") this->get_parent()->send_request(CMD_GET_LIFT_SENSOR);
    else if (this->get_type() == "strobe") this->get_parent()->send_request(CMD_GET_STROBE);
  }
  void publish_state(bool state) {
    switch_::Switch::publish_state(state);
    this->just_updated();
  }
  void set_state(bool state) {
    if (this->state != state)
      return this->publish_state(state);
  }

 protected:
  void write_state(bool state) {
    if (!this->is_connected()) return;
    if (this->type_ == "lift_sensor") {
      this->get_parent()->send_request(CMD_SET_LIFT_SENSOR, state ? 1 : 0);
    } else if (this->type_ == "spectrum_light") {
      this->get_parent()->send_request(CMD_SET_SPECTRUM_LIGHT, state ? 1 : 0);
      this->get_parent()->send_request(CMD_GET_SPECTRUM_LIGHT);
    } else if (this->type_ == "strobe") {
      this->get_parent()->send_request(CMD_SET_STROBE, state ? 1 : 0);
      this->get_parent()->send_request(CMD_GET_STROBE);
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
