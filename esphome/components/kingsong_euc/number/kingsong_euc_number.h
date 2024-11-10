#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/number/number.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

static const std::string ALARM_1 = "alarm_1";
static const std::string ALARM_2 = "alarm_2";
static const std::string ALARM_3 = "alarm_3";
static const std::string STANDBY_DELAY = "standby_delay";
static const std::string TILT_BACK = "tilt_back";

#define REGISTER_NUMBER(name) \
 protected: \
  KingSongEUCNumber *name##_number_{nullptr}; \
\
 public: \
  void set_##name##_number(KingSongEUCNumber *number) { \
    this->name##_number_ = number; \
    number->set_parent(this); \
    number->set_type(#name); \
  }

class KingSongEUCNumber : public number::Number, public KingSongEUCComponent {

 public:
  void dump_config() { LOG_NUMBER("  ", this->get_type().c_str(), this); }

  void publish_state(float state) {
    if (!this->is_state_expired() && this->state == state) return;
    number::Number::publish_state(state);
    this->just_updated();
  }

  void update() override {
    if (!this->is_connected())
      return;
    if (this->get_last_updated() > 0)
      return;
    if (this->get_type() == ALARM_1 || this->get_type() == ALARM_2 || this->get_type() == ALARM_3 ||
        this->get_type() == TILT_BACK)
      this->get_parent()->get_alarms();
    if (this->get_type() == STANDBY_DELAY)
      this->get_parent()->get_standby_delay();
  }

 protected:
  void control(float value) {
    if (!this->is_connected())
      return;
    // uint16_t decimal = (uint16_t) value;
    if (this->get_type() == ALARM_1 || this->get_type() == ALARM_2 || this->get_type() == ALARM_3 || this->get_type() == TILT_BACK) {
      this->publish_state(value);
      this->get_parent()->set_alarms(40, 45, 50, 55);
    } else if (this->get_type() == STANDBY_DELAY)
      this->get_parent()->set_standby_delay(value);
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
