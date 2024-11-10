#pragma once

#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "codec.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCClient : public ble_client::BLEClientNode {
 public:
  KingSongEUCCodec* get_codec();
  void beep();
  void get_alarms();
  void get_lift_sensor();
  void get_lock();
  void get_magic_light();
  void get_model();
  void get_serial();
  void get_spectrum_light();
  void get_standby_delay();
  void get_strobe();
  void horn();
  void lock();
  void power_off();
  void set_alarms(uint8_t alarm_1, uint8_t alarm_2, uint8_t alarm_3, uint8_t tilt_back);
  void set_lift_sensor(bool state);
  void set_magic_light(bool state);
  void set_main_light_mode(uint8_t main_light_mode);
  void set_ride_mode(uint8_t ride_mode);
  void set_spectrum_light(bool state);
  void set_standby_delay(uint16_t standby_delay);
  void set_strobe(bool state);
  void unlock();

 protected:
  uint16_t char_handle_;
  std::unique_ptr<KingSongEUCCodec> codec_;

  void send_request_(std::unique_ptr<KingSongEUCBuffer> request);
};

}  // namespace kingsong_euc
}  // namespace esphome
