#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "base.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCComponent : public Parented<ISendable>, public PollingComponent {
 public:
  void dump_config() override {}
  void update() override {}
  bool is_connected() { return this->get_parent()->node_state == esp32_ble_tracker::ClientState::ESTABLISHED; }
  void set_type(const std::string type) { this->type_ = type; }
  std::string get_type() { return this->type_; }
  void just_updated() { this->last_updated_ = millis(); }
  uint32_t get_last_updated() { return this->last_updated_; }

 protected:
  std::string type_;
  uint32_t last_updated_ = 0;
};

}  // namespace kingsong_euc
}  // namespace esphome
