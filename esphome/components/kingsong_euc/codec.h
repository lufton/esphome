#pragma once

#include <cstring>
#include <map>
#include <stdint.h>
#include "const.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCCodec {
 public:
  void save_buffer(const uint8_t *data, uint16_t length) {
    memcpy(&this->buf_, data, sizeof(KingSongEUCBuffer));
    if (this->get_packet_type() == PKT_LOCK)
      this->save_lock_pin();
  }
  KingSongEUCBuffer *get_request(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes);
  KingSongEUCBuffer *get_request(KingSongEUCPacketTypes type, uint16_t value, std::map<uint8_t, uint8_t> bytes);
  KingSongEUCBuffer *get_request(KingSongEUCPacketTypes type, uint16_t value) { return get_request(type, value, {}); }
  KingSongEUCBuffer *get_lock_request() { return this->get_request(CMD_LOCK, 0x0001); }
  KingSongEUCBuffer *get_unlock_request();
  bool check_packet_type(KingSongEUCPacketTypes packet_type) { return this->buf_.packet_type == packet_type; }
  KingSongEUCPacketTypes get_packet_type() { return this->buf_.packet_type; }
  KingSongEUCBMSPacketTypes get_bms_packet_type() { return (KingSongEUCBMSPacketTypes) this->buf_.tail[0]; }
  template<typename T> const T *get_packet() const { return (T *) &this->buf_; }
  std::string get_string();
  void save_lock_pin() {
    if (this->get_word(4) + this->get_word(6) + this->get_word(8) > 0) {
      this->lock_pin_a_ = this->get_byte(4) - 48;
      this->lock_pin_b_ = this->get_byte(6) - 48;
      this->lock_pin_c_ = this->get_byte(8) - 48;
    }
  }
  void log_buffer();
  uint16_t get_value() { return this->get_word(2); }
  uint8_t get_byte(uint8_t index) { return this->buf_.data_byte[index - 2]; }
  uint16_t get_word(uint8_t index) { return this->buf_.data_byte[index - 1] << 8 | this->buf_.data_byte[index - 2]; }
  uint32_t get_dword(uint8_t index) {
    return this->buf_.data_byte[index - 1] << 24 | this->buf_.data_byte[index - 2] << 16 |
           this->buf_.data_byte[index + 1] << 8 | this->buf_.data_byte[index];
  }

 protected:
  KingSongEUCBuffer request_;
  KingSongEUCBuffer buf_;
  uint8_t lock_pin_a_;
  uint8_t lock_pin_b_;
  uint8_t lock_pin_c_;
  void clear_request_() { std::memcpy(&this->request_, &EMPTY_PACKET, sizeof(KingSongEUCBuffer)); }
};
}  // namespace kingsong_euc
}  // namespace esphome
