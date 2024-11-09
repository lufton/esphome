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
  void update() override {
    if (!this->is_connected())
      return;
    if (this->get_last_updated() > 0)
      return;
    if (this->get_type() == ALARM_1 || this->get_type() == ALARM_2 || this->get_type() == ALARM_3 ||
        this->get_type() == TILT_BACK)
      this->get_parent()->send_request(CMD_GET_ALARMS);
    if (this->get_type() == STANDBY_DELAY)
      this->get_parent()->send_request(CMD_GET_STANDBY_DELAY);
  }
  void publish_state(float state) {
    number::Number::publish_state(state);
    this->just_updated();
  }

 protected:
  void control(float value) {
    if (!this->is_connected())
      return;
    uint16_t decimal = (uint16_t) value;
    if (this->get_type() == ALARM_1 || this->get_type() == ALARM_2 || this->get_type() == ALARM_3 ||
        this->get_type() == TILT_BACK) {
      this->publish_state(value);
      this->get_parent()->send_alarms();
    } else if (this->get_type() == STANDBY_DELAY)
      this->get_parent()->send_request(CMD_SET_STANDBY_DELAY, 1, {{4, decimal & 0xFF}, {5, (decimal >> 8) & 0xFF}});
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
