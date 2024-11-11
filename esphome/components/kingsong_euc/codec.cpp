#include "codec.h"

namespace esphome {
namespace kingsong_euc {

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_beep_request() {
  return this->get_request_(CMD_BEEP);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_alarms_request() {
  return this->get_request_(CMD_GET_ALARMS);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_lift_sensor_request() {
  return this->get_request_(CMD_GET_LIFT_SENSOR);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_lock_request() {
  return this->get_request_(CMD_GET_LOCK);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_magic_light_mode_request() {
  return this->get_request_(CMD_GET_MAGIC_LIGHT_MODE);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_model_request() {
  return this->get_request_(CMD_GET_MODEL);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_music_bluetooth_request() {
  return this->get_request_(CMD_GET_MUSIC_BT);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_serial_request() {
  return this->get_request_(CMD_GET_SERIAL);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_spectrum_light_request() {
  return this->get_request_(CMD_GET_SPECTRUM_LIGHT);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_standby_delay_request() {
  return this->get_request_(CMD_GET_STANDBY_DELAY);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_get_strobe_request() {
  return this->get_request_(CMD_GET_STROBE);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_horn_request() {
  return this->get_request_(CMD_HORN);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_lock_request() {
  return this->get_request_(CMD_LOCK, 0x0001);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_power_off_request() {
  return this->get_request_(CMD_POWER_OFF);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_alarms_request(uint8_t alarm_1, uint8_t alarm_2, uint8_t alarm_3, uint8_t tilt_back) {
  return this->get_request_(CMD_SET_ALARMS, {{2, alarm_1}, {4, alarm_2}, {6, alarm_3}, {8, tilt_back}, {10, '1'}, {11, '2'}, {12, '3'}, {13, '4'}, {14, '5'}, {15, '6'}});
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_lift_sensor_request(bool state) {
  return this->get_request_(CMD_SET_LIFT_SENSOR, state ? 1 : 0);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_magic_light_mode_request(uint8_t magic_light_mode) {
  return this->get_request_(CMD_SET_MAGIC_LIGHT_MODE, magic_light_mode);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_main_light_mode_request(uint8_t main_light_mode) {
  return this->get_request_(CMD_SET_MAIN_LIGHT_MODE, main_light_mode + 18);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_music_bluetooth_request(bool music_bluetooth) {
  return this->get_request_(CMD_SET_MUSIC_BT, music_bluetooth);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_ride_mode_request(uint8_t ride_mode) {
  return this->get_request_(CMD_SET_RIDE_MODE, 0xE000 | ride_mode);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_spectrum_light_request(bool state) {
  return this->get_request_(CMD_SET_SPECTRUM_LIGHT, state ? 1 : 0);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_spectrum_light_mode_request(uint8_t spectrum_light_mode) {
  return this->get_request_(CMD_SET_SPECTRUM_LIGHT_MODE, spectrum_light_mode);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_standby_delay_request(uint16_t standby_delay) {
  return this->get_request_(CMD_SET_STANDBY_DELAY, 0x0001, {{4, standby_delay & 0xFF}, {5, (standby_delay >> 8) & 0xFF}});
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_set_strobe_request(bool state) {
  return this->get_request_(CMD_SET_STROBE, state ? 1 : 0);
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_unlock_request() {
  uint8_t x = random_uint32() % 10;
  uint8_t y = random_uint32() % 10;
  uint8_t z = random_uint32() % 10;
  uint8_t a = (x + y + z + this->lock_pin_.a + this->lock_pin_.b + this->lock_pin_.c) % 10;
  uint8_t b = (x + y + z + a + this->lock_pin_.b + this->lock_pin_.c) % 10;
  uint8_t c = (x + y + z + a + b + this->lock_pin_.c) % 10;
  return this->get_request_(CMD_LOCK, {{10, '0' + a}, {11, '0' + x}, {12, '0' + b}, {13, '0' + y}, {14, '0' + c}, {15, '0' + z}});
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_request_(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes) {
  // KingSongEUCBuffer* request = new KingSongEUCBuffer();
  std::unique_ptr<KingSongEUCBuffer> request_ptr = make_unique<KingSongEUCBuffer>();
  auto request = request_ptr.get();
  request->packet_type = type;
  for (const auto &pair : bytes)
    request->data_byte[pair.first - 2] = pair.second;
  return request_ptr;
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_request_(KingSongEUCPacketTypes type) {
  return this->get_request_(type, {});
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_request_(KingSongEUCPacketTypes type, uint16_t value) {
  return this->get_request_(type, value, {});
}

std::unique_ptr<KingSongEUCBuffer> KingSongEUCCodec::get_request_(KingSongEUCPacketTypes type, uint16_t value, std::map<uint8_t, uint8_t> bytes) {
  bytes[2] = value & 0xFF;
  bytes[3] = (value >> 8) & 0xFF;
  return this->get_request_(type, bytes);
}

void KingSongEUCCodec::log_buffer() {
  ESP_LOGE(TAG,
           "Buffer is: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
           this->buffer_.header & 0xFF, (this->buffer_.header >> 8) & 0xFF, this->buffer_.data_byte[0], this->buffer_.data_byte[1],
           this->buffer_.data_byte[2], this->buffer_.data_byte[3], this->buffer_.data_byte[4], this->buffer_.data_byte[5],
           this->buffer_.data_byte[6], this->buffer_.data_byte[7], this->buffer_.data_byte[8], this->buffer_.data_byte[9],
           this->buffer_.data_byte[10], this->buffer_.data_byte[11], this->buffer_.data_byte[12], this->buffer_.data_byte[13],
           this->buffer_.packet_type, this->buffer_.tail[0], this->buffer_.tail[1], this->buffer_.tail[2]);
  ESP_LOGE(TAG, "    Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
}

bool KingSongEUCCodec::save_buffer(uint8_t *buffer, uint16_t length) {
  if (length != sizeof(KingSongEUCBuffer)) {
    ESP_LOGW(TAG, "Notification of size '%d' received, but '%d' expected", length, sizeof(KingSongEUCBuffer));
    return false;
  }
  if (buffer[0] != 0xAA || buffer[1] != 0x55) {
    ESP_LOGW(TAG, "Notification has unexpected header '%2x %2x' received", buffer[0], buffer[1]);
    return false;
  }
  memcpy(&this->buffer_, buffer, sizeof(KingSongEUCBuffer));
  if (this->get_packet_type() == PKT_LOCK) this->update_lock_pin_();
  return true;
}

std::string KingSongEUCCodec::get_string() {
  uint8_t *buffer_ = (uint8_t *) &this->buffer_;
  std::string result;

  for (size_t i = 2; i < sizeof(KingSongEUCBuffer); ++i) {
    if (i == 16)
      continue;
    if (buffer_[i] == 0)
      break;
    result += static_cast<char>(buffer_[i]);
  }

  return result;
}

// void KingSongEUCCodec::clear_request_() {
//   memcpy(&this->buffer_, &EMPTY_PACKET, sizeof(KingSongEUCBuffer));
//   // this->request_.header = 0x55AA;
//   // for (uint8_t i = 0; i < 14; ++i) this->request_.data_byte[i] = 0;
//   // this->request_.packet_type = CMD_NONE;
//   // this->request_.tail[0] = 0x14;
//   // this->request_.tail[1] = 0x5A;
//   // this->request_.tail[2] = 0x5A;
// }

void KingSongEUCCodec::update_lock_pin_() {
  if (this->get_word(4) + this->get_word(6) + this->get_word(8) > 0) {
    this->lock_pin_.a = this->get_byte(4) - 48;
    this->lock_pin_.b = this->get_byte(6) - 48;
    this->lock_pin_.c = this->get_byte(8) - 48;
  }
}

}  // namespace kingsong_euc
}  // namespace esphome
