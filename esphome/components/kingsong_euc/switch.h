#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/switch/switch.h"
#include "const.h"
#include "component.h"

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
  }

enum class KingSongEUCSwitchType {
  LIFT_SENSOR,
  MUSIC_BLUETOOTH,
  SPECTRUM_LIGHT,
  STROBE,
};

class KingSongEUCSwitch : public switch_::Switch, public KingSongEUCComponent {

 public:
  KingSongEUCSwitch(KingSongEUCSwitchType switch_type, uint32_t report_interval) : KingSongEUCComponent(report_interval) {
    this->switch_type_ = switch_type;
  }

  void dump_config() {
    // LOG_SWITCH("  ", this->get_type().c_str(), this);
  }

  void publish_state(bool state) {
    bool prev_state = this->state;
    this->state = state;
    this->just_updated();
    if (state != prev_state) this->report_state();
  }

  void report_state() override {
    if (this->last_updated_ == 0) return;
    switch_::Switch::publish_state(this->state);
    this->just_reported();
  }

  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    switch (this->switch_type_) {
      case KingSongEUCSwitchType::LIFT_SENSOR: return this->get_parent()->get_lift_sensor();
      case KingSongEUCSwitchType::MUSIC_BLUETOOTH: return this->get_parent()->get_music_bluetooth();
      case KingSongEUCSwitchType::SPECTRUM_LIGHT: return this->get_parent()->get_spectrum_light();
      case KingSongEUCSwitchType::STROBE: return this->get_parent()->get_strobe();
      default: break;
    }
  }

 protected:
  KingSongEUCSwitchType switch_type_;

  void write_state(bool state) {
    if (!this->is_connected()) return;
    switch (this->switch_type_) {
      case KingSongEUCSwitchType::LIFT_SENSOR: return this->get_parent()->set_lift_sensor(state);
      case KingSongEUCSwitchType::MUSIC_BLUETOOTH: return this->get_parent()->set_music_bluetooth(state);
      case KingSongEUCSwitchType::SPECTRUM_LIGHT: return this->get_parent()->set_spectrum_light(state);
      case KingSongEUCSwitchType::STROBE: return this->get_parent()->set_strobe(state);
      default: break;
    }
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
