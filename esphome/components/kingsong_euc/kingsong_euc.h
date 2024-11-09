#pragma once

#ifdef USE_ESP32

#include "esphome/core/component.h"
#include "codec.h"
#include "const.h"
// #include "binary_sensor/kingsong_euc_binary_sensor.h"
#include "button/kingsong_euc_button.h"
#include "lock/kingsong_euc_lock.h"
#include "number/kingsong_euc_number.h"
#include "switch/kingsong_euc_switch.h"
#include "text_sensor/kingsong_euc_text_sensor.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUC : public ISendable, public Component {
 public:
  void setup() override { this->codec_ = make_unique<KingSongEUCCodec>(); }
  // void update() override;
  void dump_config() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void send_request(KingSongEUCBuffer *packet) override;
  void send_request(KingSongEUCPacketTypes type) override;
  void send_request(KingSongEUCPacketTypes type, uint16_t value) override;
  void send_request(KingSongEUCPacketTypes type, uint16_t value, std::map<uint8_t, uint8_t> bytes) override;
  void send_alarms() override {
    uint8_t alarm_1 = (uint8_t) this->alarm_1_number_->state;
    uint8_t alarm_2 = (uint8_t) this->alarm_2_number_->state;
    uint8_t alarm_3 = (uint8_t) this->alarm_3_number_->state;
    uint8_t tilt_back = (uint8_t) this->tilt_back_number_->state;
    this->send_request(CMD_SET_ALARMS, alarm_1, {{4, alarm_2}, {6, alarm_3}, {8, tilt_back}, {10, '1'}, {11, '2'}, {12, '3'}, {13, '4'}, {14, '5'}, {15, '6'}});
  }
  void lock() override {
    this->send_request(this->codec_->get_lock_request());
  }
  void unlock() override {
    this->send_request(this->codec_->get_unlock_request());
  }
  // REGISTER_BINARY_SENSOR(charging)
  // REGISTER_BINARY_SENSOR(circle_light)
  // REGISTER_BINARY_SENSOR(fan)

  REGISTER_BUTTON(beep)
  REGISTER_BUTTON(horn)
  REGISTER_BUTTON(power_off)
  REGISTER_LOCK(lock)
  REGISTER_NUMBER(alarm_1)
  REGISTER_NUMBER(alarm_2)
  REGISTER_NUMBER(alarm_3)
  REGISTER_NUMBER(standby_delay)
  REGISTER_NUMBER(tilt_back)
  REGISTER_SWITCH(lift_sensor)
  REGISTER_SWITCH(spectrum_light)
  REGISTER_SWITCH(strobe)
  REGISTER_TEXT_SENSOR(model)
  REGISTER_TEXT_SENSOR(serial)

  // REGISTER_BINARY_SENSOR(music_bluetooth)
  // REGISTER_BINARY_SENSOR(old_model)
  // REGISTER_BINARY_SENSOR(spectrum_light)
  // REGISTER_BINARY_SENSOR(voice)
  // std::map<std::string, KingSongEUCBinarySensor *> binary_sensors_;

  // REGISTER_SELECT(light_mode)
  // REGISTER_SELECT(ride_mode)
  // std::map<std::string, KingSongEUCSelect *> selects_;

  // REGISTER_SENSOR(alarm_1)
  // REGISTER_SENSOR(alarm_2)
  // REGISTER_SENSOR(alarm_3)
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
  // REGISTER_SENSOR(current)
  // REGISTER_SENSOR(error_code)
  // REGISTER_SENSOR(magic_light_mode)
  // REGISTER_SENSOR(mosfet_temperature)
  // REGISTER_SENSOR(motor_hall)
  // REGISTER_SENSOR(motor_phase_line)
  // REGISTER_SENSOR(motor_temperature)
  // REGISTER_SENSOR(odometer)
  // REGISTER_SENSOR(power)
  // REGISTER_SENSOR(pwm)
  // REGISTER_SENSOR(spectrum_light_mode)
  // REGISTER_SENSOR(speed)
  // REGISTER_SENSOR(standby_delay)
  // REGISTER_SENSOR(tilt_back)
  // REGISTER_SENSOR(trip_distance)
  // REGISTER_SENSOR(trip_max_speed)
  // REGISTER_SENSOR(uptime)
  // REGISTER_SENSOR(voltage)

  // DEBUG
  void assert_byte(uint8_t index, uint8_t value);
  void assert_byte(std::vector<uint8_t> indices, uint8_t expected);
  void assert_word(uint8_t index, uint16_t value);
  void assert_word(std::vector<uint8_t> indices, uint16_t expected);
  void assert_dword(uint8_t index, uint32_t value);
  void assert_dword(std::vector<uint8_t> indices, uint32_t expected);
  // REGISTER_SENSOR(packet_type)
  // REGISTER_TEXT_SENSOR(packet_type_hex)
  // REGISTER_SENSOR(byte2)
  // REGISTER_SENSOR(byte3)
  // REGISTER_SENSOR(byte4)
  // REGISTER_SENSOR(byte5)
  // REGISTER_SENSOR(byte6)
  // REGISTER_SENSOR(byte7)
  // REGISTER_SENSOR(byte8)
  // REGISTER_SENSOR(byte9)
  // REGISTER_SENSOR(byte10)
  // REGISTER_SENSOR(byte11)
  // REGISTER_SENSOR(byte12)
  // REGISTER_SENSOR(byte13)
  // REGISTER_SENSOR(byte14)
  // REGISTER_SENSOR(byte15)
  // REGISTER_SENSOR(word2_3)
  // REGISTER_SENSOR(word3_4)
  // REGISTER_SENSOR(word4_5)
  // REGISTER_SENSOR(word5_6)
  // REGISTER_SENSOR(word6_7)
  // REGISTER_SENSOR(word7_8)
  // REGISTER_SENSOR(word8_9)
  // REGISTER_SENSOR(word9_10)
  // REGISTER_SENSOR(word10_11)
  // REGISTER_SENSOR(word11_12)
  // REGISTER_SENSOR(word12_13)
  // REGISTER_SENSOR(word13_14)
  // REGISTER_SENSOR(word14_15)
  // REGISTER_SENSOR(dword2_5)
  // REGISTER_SENSOR(dword3_6)
  // REGISTER_SENSOR(dword4_7)
  // REGISTER_SENSOR(dword5_8)
  // REGISTER_SENSOR(dword6_9)
  // REGISTER_SENSOR(dword7_10)
  // REGISTER_SENSOR(dword8_11)
  // REGISTER_SENSOR(dword9_12)
  // REGISTER_SENSOR(dword10_13)
  // REGISTER_SENSOR(dword11_14)
  // REGISTER_SENSOR(dword12_15)
  // DEBUG

  // std::map<std::string, KingSongEUCSensor *> sensors_;
  // KingSongEUCSensor *get_sensor_(const std::string &name) {
  //   auto pair = this->sensors_.find(name);
  //   return pair == this->sensors_.end() ? nullptr : pair->second;
  // }

  // SUB_SWITCH(lift_sensor)
  // SUB_SWITCH(spectrum_light)
  // SUB_SWITCH(strobe)
  // std::map<std::string, KingSongEUCTextSwitch *> switches_;

  // REGISTER_TEXT_SENSOR(error_description)
  // REGISTER_TEXT_SENSOR(model)
  // REGISTER_TEXT_SENSOR(voice_language)
  // std::map<std::string, KingSongEUCTextSensor *> text_sensors_;

 protected:
  uint16_t char_handle_;
  std::unique_ptr<KingSongEUCCodec> codec_;
  // std::map<std::string, uint32_t> latest_updates_;
  // void publish_state_(KingSongEUCBinarySensor *binary_sensor, bool state) {
  //   if (binary_sensor != nullptr && binary_sensor->state != state)
  //     binary_sensor->publish_state(state);
  // }
  // void publish_state_(KingSongEUCSelect *select, std::string state) {
  //   if (select != nullptr && select->state != state)
  //     select->publish_state(state);
  // }
  // void publish_state_(KingSongEUCSensor *sensor, float state) {
  //   if (sensor != nullptr && sensor->state != state)
  //     sensor->publish_state(state);
  // }
  // void publish_state_(KingSongEUCSensor *sensor, float state, float delta) {
  //   if (sensor != nullptr && (!sensor->has_state() || state == NAN || std::abs(sensor->state - state) >= delta))
  //     sensor->publish_state(state);
  // }
  // void publish_state_(KingSongEUCSwitch *switch_, bool state) {
  //   if (switch_ != nullptr && switch_->state != state)
  //     switch_->publish_state(state);
  // }
  // void publish_state_(KingSongEUCTextSensor *text_sensor, std::string state) {
  //   if (text_sensor != nullptr && text_sensor->state != state)
  //     text_sensor->publish_state(state);
  // }
  void publish_debug_data_();
};

}  // namespace kingsong_euc
}  // namespace esphome

#endif
