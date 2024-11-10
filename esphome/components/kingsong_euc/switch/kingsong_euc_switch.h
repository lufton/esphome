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

  void publish_state(bool state) {
    if (!this->is_state_expired() && this->state == state) return;
    switch_::Switch::publish_state(state);
    this->just_updated();
  }

  void update() override {
    if (!this->is_connected())
      return;
    if (this->get_last_updated() > 0)
      return;
    if (this->get_type() == "lift_sensor")
      this->get_parent()->get_lift_sensor();
    else if (this->get_type() == "magic_light")
      this->get_parent()->get_magic_light();
    else if (this->get_type() == "spectrum_light")
      this->get_parent()->get_spectrum_light();
    else if (this->get_type() == "strobe")
      this->get_parent()->get_strobe();
  }

 protected:
  void write_state(bool state) {
    if (!this->is_connected())
      return;
    if (this->type_ == "lift_sensor")
      this->get_parent()->set_lift_sensor(state);
    else if (this->get_type() == "magic_light")
      this->get_parent()->set_magic_light(state);
    else if (this->type_ == "spectrum_light")
      this->get_parent()->set_spectrum_light(state);
    else if (this->type_ == "strobe")
      this->get_parent()->set_strobe(state);
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
