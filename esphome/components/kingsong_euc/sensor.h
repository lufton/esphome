#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SENSOR(name) \
 protected: \
  KingSongEUCSensor *name##_sensor_{nullptr}; \
\
 public: \
  void set_##name##_sensor(KingSongEUCSensor *sensor) { \
    this->sensors_.push_back(sensor); \
    this->name##_sensor_ = sensor; \
    sensor->set_parent(this); \
  }

enum class KingSongEUCSensorType {
  CPU_RATE,
  ERROR_CODE,
  CURRENT,
  MOSFET_TEMPERATURE,
  MOTOR_HALL,
  MOTOR_PHASE_LINE,
  MOTOR_TEMPERATURE,
  ODOMETER,
  POWER,
  PWM,
  RIDE_TIME,
  SPEED,
  SPEED_LIMIT,
  TRIP_DISTANCE,
  TRIP_MAX_SPEED,
  UPTIME,
  VOLTAGE,
};

class KingSongEUCSensor : public sensor::Sensor, public KingSongEUCBaseEntity {
 public:
  KingSongEUCSensor(KingSongEUCSensorType sensor_type, uint32_t report_interval, float hysteresis)
      : KingSongEUCBaseEntity(report_interval) {
    this->hysteresis_ = hysteresis;
    this->sensor_type_ = sensor_type;
  }

  void dump_config() {
    // LOG_SENSOR("  ", this->get_type().c_str(), this);
  }

  bool has_state() override { return this->has_state_ && KingSongEUCBaseEntity::has_state(); }

  void publish_state(float state) {
    float prev_state = this->state;
    this->state = state;
    this->has_state_ = true;
    this->just_updated();
    if (prev_state == NAN || std::abs(prev_state - state) >= this->hysteresis_)
      this->report_state();
  }

  void report_state() {
    sensor::Sensor::publish_state(this->state);
    this->just_reported();
  }

  void request_state() override {
    switch (this->sensor_type_) {
      default:
        break;
    }
  }

 protected:
  KingSongEUCSensorType sensor_type_;
  float hysteresis_ = 0xFFFFFFFF;
};

}  // namespace kingsong_euc
}  // namespace esphome
