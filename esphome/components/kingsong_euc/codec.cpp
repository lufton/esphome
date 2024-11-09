#include "esphome/core/log.h"
#include "codec.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

KingSongEUCBuffer *KingSongEUCCodec::get_request(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes) {
  this->clear_request_();
  this->request_.packet_type = type;
  for (const auto &pair : bytes)
    this->request_.data_byte[pair.first - 2] = pair.second;
  return &this->request_;
}

KingSongEUCBuffer *KingSongEUCCodec::get_request(KingSongEUCPacketTypes type, uint16_t value,
                                                 std::map<uint8_t, uint8_t> bytes) {
  this->clear_request_();
  this->request_.packet_type = type;
  this->request_.data_byte[0] = value & 0xFF;
  this->request_.data_byte[1] = (value >> 8) & 0xFF;
  for (const auto &pair : bytes)
    this->request_.data_byte[pair.first - 2] = pair.second;
  return &this->request_;
}

KingSongEUCBuffer *KingSongEUCCodec::get_unlock_request() {
  ESP_LOGE(TAG, "a = %d, b = %d, c = %d", this->lock_pin_a_, this->lock_pin_b_, this->lock_pin_c_);
  uint8_t x = random_uint32() % 10;
  uint8_t y = random_uint32() % 10;
  uint8_t z = random_uint32() % 10;
  uint8_t a = (x + y + z + this->lock_pin_a_ + this->lock_pin_b_ + this->lock_pin_c_) % 10;
  uint8_t b = (x + y + z + a + this->lock_pin_b_ + this->lock_pin_c_) % 10;
  uint8_t c = (x + y + z + a + b + this->lock_pin_c_) % 10;
  return this->get_request(CMD_LOCK,
                           {{10, '0' + a}, {11, '0' + x}, {12, '0' + b}, {13, '0' + y}, {14, '0' + c}, {15, '0' + z}});
}

void KingSongEUCCodec::log_buffer() {
  ESP_LOGE(TAG,
           "Buffer is: %04X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
           this->buf_.header, this->buf_.data_byte[0], this->buf_.data_byte[1], this->buf_.data_byte[2],
           this->buf_.data_byte[3], this->buf_.data_byte[4], this->buf_.data_byte[5], this->buf_.data_byte[6],
           this->buf_.data_byte[7], this->buf_.data_byte[8], this->buf_.data_byte[9], this->buf_.data_byte[10],
           this->buf_.data_byte[11], this->buf_.data_byte[12], this->buf_.data_byte[13], this->buf_.packet_type,
           this->buf_.tail[0], this->buf_.tail[1], this->buf_.tail[2]);
  ESP_LOGE(TAG, "    Index: 0001 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
}

std::string KingSongEUCCodec::get_string() {
  // uint8_t *start = (uint8_t *) &this->buf_ + 2;
  // uint8_t length = 0;
  // while (start[length] != 0)
  //   length++;
  // std::string result(reinterpret_cast<char *>(start), length);
  // return result;
  uint8_t *buf_ = (uint8_t *) &this->buf_;
  std::string result;

  for (size_t i = 2; i < sizeof(KingSongEUCBuffer); ++i) {
    if (i == 16)
      continue;
    if (buf_[i] == 0)
      break;
    result += static_cast<char>(buf_[i]);
  }

  return result;
}

}  // namespace kingsong_euc
}  // namespace esphome
