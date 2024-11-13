#pragma once

#include <cstring>
#include <map>
#include <stdint.h>
#include "esphome/components/lock/lock.h"
#include "esphome/core/log.h"
#include "base.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define GETTER_FIELD(type, name) \
 private: \
  void set_##name(type value) { this->name##_ = value; } \
\
 protected: \
  type name##_{}; \
\
 public: \
  type get_##name() { return name##_; }

class KingSongEUCCodec {
 public:
  std::unique_ptr<KingSongEUCBuffer> get_beep_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_alarms_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_lock_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_model_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_serial_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_standby_delay_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_lift_sensor_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_magic_light_mode_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_music_bluetooth_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_spectrum_light_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_spectrum_light_mode_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_strobe_request();
  std::unique_ptr<KingSongEUCBuffer> get_get_voice_language_request();
  std::unique_ptr<KingSongEUCBuffer> get_horn_request();
  std::unique_ptr<KingSongEUCBuffer> get_lock_request();
  std::unique_ptr<KingSongEUCBuffer> get_power_off_request();
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_1_request(uint8_t alarm_1);
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_2_request(uint8_t alarm_2);
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_3_request(uint8_t alarm_3);
  std::unique_ptr<KingSongEUCBuffer> get_set_main_light_mode_request(uint8_t main_light_mode);
  std::unique_ptr<KingSongEUCBuffer> get_set_music_bluetooth_request(bool music_bluetooth);
  std::unique_ptr<KingSongEUCBuffer> get_set_ride_mode_request(uint8_t ride_mode);
  std::unique_ptr<KingSongEUCBuffer> get_set_standby_delay_request(uint16_t standby_delay);
  std::unique_ptr<KingSongEUCBuffer> get_set_lift_sensor_request(bool lift_sensor);
  std::unique_ptr<KingSongEUCBuffer> get_set_magic_light_mode_request(uint8_t magic_light_mode);
  std::unique_ptr<KingSongEUCBuffer> get_set_spectrum_light_request(bool spectrum_light);
  std::unique_ptr<KingSongEUCBuffer> get_set_spectrum_light_mode_request(uint8_t spectrum_light_mode);
  std::unique_ptr<KingSongEUCBuffer> get_set_strobe_request(bool strobe);
  std::unique_ptr<KingSongEUCBuffer> get_set_tilt_back_request(uint8_t tilt_back);
  std::unique_ptr<KingSongEUCBuffer> get_set_voice_request(bool voice);
  std::unique_ptr<KingSongEUCBuffer> get_set_voice_language_request(uint8_t voice_language);
  std::unique_ptr<KingSongEUCBuffer> get_unlock_request();
  void save_buffer(uint8_t *buffer);
  bool check_packet_type(KingSongEUCPacketTypes packet_type) { return this->buffer_.packet_type == packet_type; }
  KingSongEUCPacketTypes get_packet_type() { return this->buffer_.packet_type; }
  KingSongEUCBMSPacketTypes get_bms_packet_type() { return (KingSongEUCBMSPacketTypes) this->buffer_.tail[0]; }
  template<typename T> const T *get_packet() const { return (T *) &this->buffer_; }
  std::string get_string();
  void log_buffer();
  uint16_t get_value() { return this->get_word(2); }
  int8_t get_byte(uint8_t index) { return this->buffer_.data_byte[index - 2]; }
  int16_t get_word(uint8_t index) {
    return this->buffer_.data_byte[index - 1] << 8 | this->buffer_.data_byte[index - 2];
  }
  int32_t get_dword(uint8_t index) {
    return this->buffer_.data_byte[index - 1] << 24 | this->buffer_.data_byte[index - 2] << 16 |
           this->buffer_.data_byte[index + 1] << 8 | this->buffer_.data_byte[index];
  }

 protected:
  // KingSongEUCBuffer request_;
  KingSongEUCBuffer buffer_;
  struct lock_pin {
    uint8_t a;
    uint8_t b;
    uint8_t c;
  } lock_pin_{};
  GETTER_FIELD(uint16_t, alarm_1)
  GETTER_FIELD(uint16_t, alarm_2)
  GETTER_FIELD(uint16_t, alarm_3)
  GETTER_FIELD(bool, charging)
  GETTER_FIELD(bool, circle_light)
  GETTER_FIELD(float, current)
  GETTER_FIELD(bool, fan)
  GETTER_FIELD(bool, lift_sensor)
  GETTER_FIELD(lock::LockState, lock)
  GETTER_FIELD(uint8_t, magic_light_mode)
  GETTER_FIELD(uint8_t, main_light_mode)
  GETTER_FIELD(std::string, model)
  GETTER_FIELD(float, mosfet_temperature)
  GETTER_FIELD(float, motor_temperature)
  GETTER_FIELD(bool, music_bluetooth)
  GETTER_FIELD(float, odometer)
  GETTER_FIELD(float, power)
  GETTER_FIELD(uint8_t, ride_mode)
  GETTER_FIELD(std::string, serial)
  GETTER_FIELD(bool, spectrum_light)
  GETTER_FIELD(uint8_t, spectrum_light_mode)
  GETTER_FIELD(float, speed)
  GETTER_FIELD(uint16_t, tilt_back)
  GETTER_FIELD(float, trip_distance)
  GETTER_FIELD(float, trip_max_speed)
  GETTER_FIELD(uint16_t, standby_delay)
  GETTER_FIELD(bool, strobe)
  GETTER_FIELD(bool, voice)
  GETTER_FIELD(uint8_t, voice_language)
  GETTER_FIELD(uint16_t, uptime)
  GETTER_FIELD(float, voltage)

  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, uint16_t value);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, uint16_t value,
                                                  std::map<uint8_t, uint8_t> bytes);
};

}  // namespace kingsong_euc
}  // namespace esphome
