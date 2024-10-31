#pragma once

#include <cstring>
#include <map>
#include <stdint.h>
#include "const.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCCodec {
 public:
  void save_buffer(const uint8_t *data, uint16_t length) { memcpy(&this->buf_, data, sizeof(KingSongEUCBuffer)); }
  // uint8_t *get_request(KingSongEUCPacketTypes type);
  KingSongEUCBuffer *get_request(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes);
  bool check_packet_type(KingSongEUCPacketTypes packet_type) { return this->buf_.packet_type == packet_type; }
  KingSongEUCPacketTypes get_packet_type() { return this->buf_.packet_type; }
  KingSongEUCBMSPacketTypes get_bms_packet_type() { return (KingSongEUCBMSPacketTypes) this->buf_.tail[0]; }
  template<typename T> const T *get_packet() const { return (T *) &this->buf_; }
  std::string get_model();
  void log_buffer();

 protected:
  KingSongEUCBuffer request_;
  KingSongEUCBuffer buf_;
  void clear_request_() { std::memcpy(&this->request_, &EMPTY_PACKET, sizeof(KingSongEUCBuffer)); }
};
}  // namespace kingsong_euc
}  // namespace esphome
