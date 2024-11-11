#pragma once

// #include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "const.h"
#include "component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_BINARY_SENSOR(name) \
 protected: \
  KingSongEUCBinarySensor *name##_binary_sensor_{nullptr}; \
\
 public: \
  void set_##name##_binary_sensor(KingSongEUCBinarySensor *binary_sensor) { \
    this->name##_binary_sensor_ = binary_sensor; \
    binary_sensor->set_parent(this); \
  }

enum class KingSongEUCBinarySensorType {
  CHARGING,
  FAN,
};

class KingSongEUCBinarySensor : public binary_sensor::BinarySensor, public KingSongEUCComponent {

 public:
  KingSongEUCBinarySensor(KingSongEUCBinarySensorType binary_sensor_type, uint32_t report_interval) : KingSongEUCComponent(report_interval) {
    this->binary_sensor_type_ = binary_sensor_type;
  }

  void dump_config() {
    // LOG_BINARY_SENSOR("  ", this->get_type().c_str(), this);
  }

  void publish_state(bool state) {
    bool prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (state != prev_state) this->report_state();
  }

  void report_state() override {
    if (!this->has_state_ || this->last_updated_ == 0) return;
    binary_sensor::BinarySensor::publish_state(this->state);
    this->just_reported();
  }

  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    switch (this->binary_sensor_type_) {
      default: break;
    }
  }

 protected:
  KingSongEUCBinarySensorType binary_sensor_type_;

};

}  // namespace kingsong_euc
}  // namespace esphome
