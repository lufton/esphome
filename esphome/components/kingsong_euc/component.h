#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "client.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCComponent : public Parented<KingSongEUCClient>, public Component {

 public:
  KingSongEUCComponent() = default;
  KingSongEUCComponent(uint32_t report_interval) { this->report_interval_ = report_interval; }

  void dump_config() override {}
  uint32_t get_last_updated() { return this->last_updated_; }
  bool is_connected() { return this->get_parent()->node_state == esp32_ble_tracker::ClientState::ESTABLISHED; }
  bool is_state_expired() { return this->last_reported_ == 0 || (this->report_interval_ != SCHEDULER_DONT_RUN && millis() - this->last_reported_ > this->report_interval_); }
  void just_reported() { this->last_reported_ = millis(); }
  void just_updated() { this->last_updated_ = millis(); }
  void loop() override { if (is_state_expired()) this->report_state(); }
  virtual void report_state() {}
  virtual void update() {}

 protected:
  uint32_t last_reported_ = 0;
  uint32_t last_updated_ = 0;
  uint32_t report_interval_ = 0;

};

}  // namespace kingsong_euc
}  // namespace esphome
