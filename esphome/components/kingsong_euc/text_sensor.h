#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "const.h"
#include "component.h"

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
  }

enum class KingSongEUCTextSensorType {
  MODEL,
  SERIAL,
};

class KingSongEUCTextSensor : public text_sensor::TextSensor, public KingSongEUCComponent {

 public:
  KingSongEUCTextSensor(KingSongEUCTextSensorType text_sensor_type, uint32_t report_interval) : KingSongEUCComponent(report_interval) {
    this->text_sensor_type_ = text_sensor_type;
  }

  void dump_config() {
    // LOG_TEXT_SENSOR("  ", this->get_type().c_str(), this);
  }

  void publish_state(const std::string state) {
    const std::string prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state) this->report_state();
  }

  void report_state() override {
    if (!this->has_state_ || this->last_updated_ == 0) return;
    text_sensor::TextSensor::publish_state(this->state);
    this->just_reported();
  }

  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    switch (this->text_sensor_type_) {
      case KingSongEUCTextSensorType::MODEL: return this->get_parent()->get_model();
      case KingSongEUCTextSensorType::SERIAL: return this->get_parent()->get_serial();
      default: break;
    }
  }

 protected:
  KingSongEUCTextSensorType text_sensor_type_;

};

}  // namespace kingsong_euc
}  // namespace esphome
