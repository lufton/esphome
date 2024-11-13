#pragma once

#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "codec.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCClient : public ble_client::BLEClientNode {
 public:
  KingSongEUCCodec *get_codec();
  void beep();
  void get_alarms();
  void get_lift_sensor();
  uint32_t get_last_requested();
  void get_lock();
  void get_magic_light_mode();
  void get_model();
  void get_music_bluetooth();
  void get_serial();
  void get_spectrum_light();
  void get_spectrum_light_mode();
  void get_standby_delay();
  void get_strobe();
  void get_voice_language();
  void horn();
  void lock();
  void power_off();
  void set_alarm_1(uint8_t alarm_1);
  void set_alarm_2(uint8_t alarm_2);
  void set_alarm_3(uint8_t alarm_3);
  void set_last_requested(uint32_t last_requested);
  void set_lift_sensor(bool lift_sensor);
  void set_magic_light_mode(uint8_t magic_light_mode);
  void set_main_light_mode(uint8_t main_light_mode);
  void set_music_bluetooth(bool music_bluetooth);
  void set_ride_mode(uint8_t ride_mode);
  void set_spectrum_light(bool spectrum_light);
  void set_spectrum_light_mode(uint8_t spectrum_light_mode);
  void set_standby_delay(uint16_t standby_delay);
  void set_strobe(bool strobe);
  void set_tilt_back(uint8_t tilt_back);
  void set_voice(bool voice);
  void set_voice_language(uint8_t voice_language);
  void unlock();

 protected:
  uint16_t char_handle_;
  std::unique_ptr<KingSongEUCCodec> codec_;
  uint32_t last_requested_ = 0;

  void send_request_(std::unique_ptr<KingSongEUCBuffer> request);
};

}  // namespace kingsong_euc
}  // namespace esphome
