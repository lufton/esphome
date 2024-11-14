#pragma once

#ifdef USE_ESP32

#include <vector>
#include "esphome/core/component.h"
#include "binary_sensor.h"
#include "button.h"
#include "client.h"
#include "lock.h"
#include "number.h"
#include "select.h"
#include "sensor.h"
#include "switch.h"
#include "text_sensor.h"

namespace esphome {
namespace kingsong_euc {

static const uint32_t UPDATE_INTERVAL = 1000;

#define PUBLISH_STATE(entity, state) \
  if (entity != nullptr) \
    entity->publish_state(state);

class KingSongEUC : public KingSongEUCClient, public Component {
 public:
  void dump_config() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  // void loop() override;
  void setup() override { this->codec_ = make_unique<KingSongEUCCodec>(); }
  // void update() override { }
  // void update() override;

  REGISTER_BINARY_SENSOR(charging)
  REGISTER_BINARY_SENSOR(fan)
  REGISTER_BUTTON(beep)
  REGISTER_BUTTON(horn)
  REGISTER_BUTTON(power_off)
  REGISTER_LOCK(lock)
  REGISTER_NUMBER(alarm_1)
  REGISTER_NUMBER(alarm_2)
  REGISTER_NUMBER(alarm_3)
  REGISTER_NUMBER(standby_delay)
  REGISTER_NUMBER(tilt_back)
  REGISTER_SELECT(magic_light_mode)
  REGISTER_SELECT(main_light_mode)
  REGISTER_SELECT(ride_mode)
  REGISTER_SELECT(spectrum_light_mode)
  REGISTER_SELECT(voice_language)
  REGISTER_SENSOR(current)
  REGISTER_SENSOR(error_code)
  REGISTER_SENSOR(mosfet_temperature)
  REGISTER_SENSOR(motor_temperature)
  REGISTER_SENSOR(odometer)
  REGISTER_SENSOR(power)
  REGISTER_SENSOR(ride_time)
  REGISTER_SENSOR(speed)
  REGISTER_SENSOR(speed_limit)
  REGISTER_SENSOR(trip_distance)
  REGISTER_SENSOR(trip_max_speed)
  REGISTER_SENSOR(uptime)
  REGISTER_SENSOR(voltage)
  REGISTER_SWITCH(circle_light)
  REGISTER_SWITCH(lift_sensor)
  REGISTER_SWITCH(music_bluetooth)
  REGISTER_SWITCH(spectrum_light)
  REGISTER_SWITCH(strobe)
  REGISTER_SWITCH(voice)
  REGISTER_TEXT_SENSOR(error_description)
  REGISTER_TEXT_SENSOR(model)
  REGISTER_TEXT_SENSOR(serial)

  // REGISTER_SENSOR(bms_1_cell_1_voltage)
  // REGISTER_SENSOR(bms_1_cell_2_voltage)
  // REGISTER_SENSOR(bms_1_cell_3_voltage)
  // REGISTER_SENSOR(bms_1_cell_4_voltage)
  // REGISTER_SENSOR(bms_1_cell_5_voltage)
  // REGISTER_SENSOR(bms_1_cell_6_voltage)
  // REGISTER_SENSOR(bms_1_cell_7_voltage)
  // REGISTER_SENSOR(bms_1_cell_8_voltage)
  // REGISTER_SENSOR(bms_1_cell_9_voltage)
  // REGISTER_SENSOR(bms_1_cell_10_voltage)
  // REGISTER_SENSOR(bms_1_cell_11_voltage)
  // REGISTER_SENSOR(bms_1_cell_12_voltage)
  // REGISTER_SENSOR(bms_1_cell_13_voltage)
  // REGISTER_SENSOR(bms_1_cell_14_voltage)
  // REGISTER_SENSOR(bms_1_cell_15_voltage)
  // REGISTER_SENSOR(bms_1_cell_16_voltage)
  // REGISTER_SENSOR(bms_1_cell_17_voltage)
  // REGISTER_SENSOR(bms_1_cell_18_voltage)
  // REGISTER_SENSOR(bms_1_cell_19_voltage)
  // REGISTER_SENSOR(bms_1_cell_20_voltage)
  // REGISTER_SENSOR(bms_1_cell_21_voltage)
  // REGISTER_SENSOR(bms_1_cell_22_voltage)
  // REGISTER_SENSOR(bms_1_cell_23_voltage)
  // REGISTER_SENSOR(bms_1_cell_24_voltage)
  // REGISTER_SENSOR(bms_1_cell_25_voltage)
  // REGISTER_SENSOR(bms_1_cell_26_voltage)
  // REGISTER_SENSOR(bms_1_cell_27_voltage)
  // REGISTER_SENSOR(bms_1_cell_28_voltage)
  // REGISTER_SENSOR(bms_1_cell_29_voltage)
  // REGISTER_SENSOR(bms_1_cell_30_voltage)
  // REGISTER_SENSOR(bms_1_current)
  // REGISTER_SENSOR(bms_1_factory_capacity)
  // REGISTER_SENSOR(bms_1_full_cycles)
  // REGISTER_SENSOR(bms_1_soc)
  // REGISTER_SENSOR(bms_1_remaining_capacity)
  // REGISTER_SENSOR(bms_1_voltage)
  // REGISTER_SENSOR(bms_2_cell_1_voltage)
  // REGISTER_SENSOR(bms_2_cell_2_voltage)
  // REGISTER_SENSOR(bms_2_cell_3_voltage)
  // REGISTER_SENSOR(bms_2_cell_4_voltage)
  // REGISTER_SENSOR(bms_2_cell_5_voltage)
  // REGISTER_SENSOR(bms_2_cell_6_voltage)
  // REGISTER_SENSOR(bms_2_cell_7_voltage)
  // REGISTER_SENSOR(bms_2_cell_8_voltage)
  // REGISTER_SENSOR(bms_2_cell_9_voltage)
  // REGISTER_SENSOR(bms_2_cell_10_voltage)
  // REGISTER_SENSOR(bms_2_cell_11_voltage)
  // REGISTER_SENSOR(bms_2_cell_12_voltage)
  // REGISTER_SENSOR(bms_2_cell_13_voltage)
  // REGISTER_SENSOR(bms_2_cell_14_voltage)
  // REGISTER_SENSOR(bms_2_cell_15_voltage)
  // REGISTER_SENSOR(bms_2_cell_16_voltage)
  // REGISTER_SENSOR(bms_2_cell_17_voltage)
  // REGISTER_SENSOR(bms_2_cell_18_voltage)
  // REGISTER_SENSOR(bms_2_cell_19_voltage)
  // REGISTER_SENSOR(bms_2_cell_20_voltage)
  // REGISTER_SENSOR(bms_2_cell_21_voltage)
  // REGISTER_SENSOR(bms_2_cell_22_voltage)
  // REGISTER_SENSOR(bms_2_cell_23_voltage)
  // REGISTER_SENSOR(bms_2_cell_24_voltage)
  // REGISTER_SENSOR(bms_2_cell_25_voltage)
  // REGISTER_SENSOR(bms_2_cell_26_voltage)
  // REGISTER_SENSOR(bms_2_cell_27_voltage)
  // REGISTER_SENSOR(bms_2_cell_28_voltage)
  // REGISTER_SENSOR(bms_2_cell_29_voltage)
  // REGISTER_SENSOR(bms_2_cell_30_voltage)
  // REGISTER_SENSOR(bms_2_current)
  // REGISTER_SENSOR(bms_2_factory_capacity)
  // REGISTER_SENSOR(bms_2_full_cycles)
  // REGISTER_SENSOR(bms_2_soc)
  // REGISTER_SENSOR(bms_2_remaining_capacity)
  // REGISTER_SENSOR(bms_2_voltage)
  // REGISTER_SENSOR(cpu_rate)
  // REGISTER_SENSOR(error_code)
  // REGISTER_SENSOR(motor_hall)
  // REGISTER_SENSOR(motor_phase_line)
  // REGISTER_SENSOR(pwm)

 protected:
  std::vector<KingSongEUCBinarySensor *> binary_sensors_;
  std::vector<KingSongEUCButton *> buttons_;
  uint32_t last_update_ = 0;
  std::vector<KingSongEUCLock *> locks_;
  std::vector<KingSongEUCNumber *> numbers_;
  std::vector<KingSongEUCSelect *> selects_;
  std::vector<KingSongEUCSensor *> sensors_;
  std::vector<KingSongEUCSwitch *> switches_;
  std::vector<KingSongEUCTextSensor *> text_sensors_;

  void publish_debug_data_();
};

}  // namespace kingsong_euc
}  // namespace esphome

#endif
