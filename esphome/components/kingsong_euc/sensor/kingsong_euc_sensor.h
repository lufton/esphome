#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SENSOR(name) \
 protected: \
  KingSongEUCSensor *name##_sensor_{nullptr}; \
\
 public: \
  void set_##name##_sensor(SensorType type, KingSongEUCSensor *sensor, float hysteresis) { \
    this->name##_sensor_ = sensor; \
    sensor->set_parent(this); \
    sensor->set_type(type); \
    sensor->set_hysteresis(hysteresis); \
  }

enum class SensorType {
  NONE = 0,
  CPU_RATE,
  ERROR_CODE,
  CURRENT,
  MOSFET_TEMPERATURE,
  MOTOR_HALL,
  MOTOR_PHASE_LINE,
  ODOMETER,
  POWER,
  PWM,
  SPEED,
  TRIP_DISTANCE,
  TRIP_MAX_SPEED,
  UPTIME,
  VOLTAGE,
};

class KingSongEUCSensor : public sensor::Sensor, public KingSongEUCComponent {

 public:
  void dump_config() { LOG_SENSOR("  ", this->get_type().c_str(), this); }

  void publish_state(float state) {
    if (!this->is_state_expired() && std::abs(this->get_state() - state) < this->hysteresis_) return;
    sensor::Sensor::publish_state(state);
    this->just_updated();
  }

  void set_hysteresis(float hysteresis) {
    this->hysteresis_ = hysteresis;
  }

  void set_type(SensorType type) {
    this->type_ = type;
  }

  void update() override {
    if (!this->is_connected())
      return;
    // if (this->get_type() == SensorType::VOLTAGE);
  }

 protected:
  SensorType type_ = SensorType::NONE;
  float hysteresis_ = 0xFFFFFFFF;

};

}  // namespace kingsong_euc
}  // namespace esphome
