#pragma once

#include <cstring>
#include <map>
#include <stdint.h>
#include "esphome/components/lock/lock.h"
#include "esphome/core/log.h"
#include "base.h"
#include "const.h"
#include "error_codes.h"

namespace esphome {
namespace kingsong_euc {

#define CMD_REQUEST(name, command) \
 public: \
  std::unique_ptr<KingSongEUCBuffer> get_##name##_request() { return this->get_request_(command); }

#define CMD_REQUEST_BOOL_PARAM(name, command) \
 public: \
  std::unique_ptr<KingSongEUCBuffer> get_##name##_request(bool value) { \
    return this->get_request_(command, value ? 1 : 0); \
  }

#define CMD_REQUEST_INT_PARAM(name, command) \
 public: \
  std::unique_ptr<KingSongEUCBuffer> get_##name##_request(uint16_t value) { return this->get_request_(command, value); }

#define CMD_REQUEST_INT_VALUE(name, command, value) \
 public: \
  std::unique_ptr<KingSongEUCBuffer> get_##name##_request() { return this->get_request_(command, value); }

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
  CMD_REQUEST(beep, CMD_BEEP)
  CMD_REQUEST(get_alarms, CMD_GET_ALARMS)
  CMD_REQUEST(get_lift_sensor, CMD_GET_LIFT_SENSOR)
  CMD_REQUEST(get_lock, CMD_GET_LOCK)
  CMD_REQUEST(get_model, CMD_GET_MODEL)
  CMD_REQUEST(get_serial, CMD_GET_SERIAL)
  CMD_REQUEST(get_standby_delay, CMD_GET_STANDBY_DELAY)
  CMD_REQUEST(get_magic_light_mode, CMD_GET_MAGIC_LIGHT_MODE)
  CMD_REQUEST(get_music_bluetooth, CMD_GET_MUSIC_BLUETOOTH)
  CMD_REQUEST(get_spectrum_light, CMD_GET_SPECTRUM_LIGHT)
  CMD_REQUEST(get_spectrum_light_mode, CMD_GET_SPECTRUM_LIGHT_MODE)
  CMD_REQUEST(get_strobe, CMD_GET_STROBE)
  CMD_REQUEST(get_voice_language, CMD_GET_VOICE_LANGUAGE)
  CMD_REQUEST(horn, CMD_HORN)
  CMD_REQUEST_INT_VALUE(lock, CMD_LOCK, 0x0001)
  CMD_REQUEST(power_off, CMD_POWER_OFF)
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_1_request(uint8_t alarm_1);
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_2_request(uint8_t alarm_2);
  std::unique_ptr<KingSongEUCBuffer> get_set_alarm_3_request(uint8_t alarm_3);
  std::unique_ptr<KingSongEUCBuffer> get_set_main_light_mode_request(uint8_t main_light_mode);
  CMD_REQUEST_BOOL_PARAM(set_music_bluetooth, CMD_SET_MUSIC_BT)
  std::unique_ptr<KingSongEUCBuffer> get_set_ride_mode_request(uint8_t ride_mode);
  std::unique_ptr<KingSongEUCBuffer> get_set_standby_delay_request(uint16_t standby_delay);
  CMD_REQUEST_BOOL_PARAM(set_lift_sensor, CMD_SET_LIFT_SENSOR)
  CMD_REQUEST_INT_PARAM(set_magic_light_mode, CMD_SET_MAGIC_LIGHT_MODE)
  CMD_REQUEST_BOOL_PARAM(set_spectrum_light, CMD_SET_SPECTRUM_LIGHT)
  CMD_REQUEST_INT_PARAM(set_spectrum_light_mode, CMD_SET_SPECTRUM_LIGHT_MODE)
  CMD_REQUEST_BOOL_PARAM(set_strobe, CMD_SET_STROBE)
  std::unique_ptr<KingSongEUCBuffer> get_set_tilt_back_request(uint8_t tilt_back);
  std::unique_ptr<KingSongEUCBuffer> get_set_voice_request(bool voice);
  CMD_REQUEST_INT_PARAM(set_voice_language, CMD_SET_VOICE_LANGUAGE)

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
  GETTER_FIELD(uint16_t, error_code)
  GETTER_FIELD(std::string, error_description)
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
  GETTER_FIELD(uint16_t, ride_time)
  GETTER_FIELD(std::string, serial)
  GETTER_FIELD(bool, spectrum_light)
  GETTER_FIELD(uint8_t, spectrum_light_mode)
  GETTER_FIELD(float, speed)
  GETTER_FIELD(float, speed_limit)
  GETTER_FIELD(uint16_t, tilt_back)
  GETTER_FIELD(float, trip_distance)
  GETTER_FIELD(float, trip_max_speed)
  GETTER_FIELD(uint16_t, standby_delay)
  GETTER_FIELD(bool, strobe)
  GETTER_FIELD(bool, voice)
  GETTER_FIELD(uint8_t, voice_language)
  GETTER_FIELD(uint16_t, uptime)
  GETTER_FIELD(float, voltage)

  inline std::string get_error_description_(uint16_t error_code);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, uint16_t value);
  std::unique_ptr<KingSongEUCBuffer> get_request_(KingSongEUCPacketTypes type, uint16_t value,
                                                  std::map<uint8_t, uint8_t> bytes);
};

}  // namespace kingsong_euc
}  // namespace esphome
