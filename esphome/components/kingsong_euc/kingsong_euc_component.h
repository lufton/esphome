#pragma once

// #include "esphome/core/component.h"
// #include "esphome/core/helpers.h"
#include "base.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCComponent : public Parented<ISendable> {
 public:
  void dump_config() {}
  void set_type(const std::string type) { this->type_ = type; }
  std::string get_type() { return this->type_; }
  void set_last_publish(const uint32_t last_publish) { this->last_publish_ = last_publish; }
  uint32_t get_last_publish() { return this->last_publish_; }

 protected:
  std::string type_;
  uint32_t last_publish_;
};

}  // namespace kingsong_euc
}  // namespace esphome
