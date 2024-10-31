#include "esphome/core/log.h"
#include "codec.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

// uint8_t *KingSongEUCCodec::get_request(KingSongEUCPacketTypes type) {
//   this->clear_request_();
//   this->request_.packet_type = type;
//   return (uint8_t *) &this->request_;
// }
KingSongEUCBuffer *KingSongEUCCodec::get_request(KingSongEUCPacketTypes type, std::map<uint8_t, uint8_t> bytes) {
  this->clear_request_();
  this->request_.packet_type = type;
  for (const auto &pair : bytes)
    this->request_.data_byte[pair.first - 2] = pair.second;
  return &this->request_;
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

std::string KingSongEUCCodec::get_model() {
  uint8_t *start = (uint8_t *) &this->buf_ + 2;
  uint8_t length = 0;
  while (start[length] != 0)
    length++;
  std::string result(reinterpret_cast<char *>(start), length);
  return result;
}

}  // namespace kingsong_euc
}  // namespace esphome
