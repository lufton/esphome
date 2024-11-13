#pragma once

#include "esphome/core/component.h"
#include "esphome/core/entity_base.h"
#include "esphome/core/helpers.h"
#include "client.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCBaseEntity : public Parented<KingSongEUCClient>, public PollingComponent {
  static const uint32_t REQUEST_TIMEOUT = 100;
  static const uint32_t RESPONSE_TIMEOUT = 3000;

 public:
  KingSongEUCBaseEntity() = default;
  KingSongEUCBaseEntity(uint32_t report_interval) { this->report_interval_ = report_interval; }

  void dump_config() override {}
  virtual bool has_state() { return this->last_updated_ > 0; }
  bool is_connected() { return this->get_parent()->node_state == esp32_ble_tracker::ClientState::ESTABLISHED; }
  bool is_request_expired() { return millis() - this->last_requested_ > RESPONSE_TIMEOUT; }
  bool is_request_possible() { return millis() - this->parent_->get_last_requested() > REQUEST_TIMEOUT; }
  bool is_report_expired() {
    return this->last_reported_ == 0 ||
           (this->report_interval_ != SCHEDULER_DONT_RUN && millis() - this->last_reported_ > this->report_interval_);
  }
  bool is_update_expired() {
    return this->last_updated_ == 0 || (this->update_interval_ != SCHEDULER_DONT_RUN && this->update_interval_ != 0 &&
                                        millis() - this->last_updated_ > this->update_interval_);
  }
  void just_reported() { this->last_reported_ = millis(); }
  uint32_t just_requested() {
    this->last_requested_ = millis();
    return this->last_requested_;
  }
  void just_updated() { this->last_updated_ = millis(); }
  void loop() override {
    if (this->is_report_expired() && this->has_state()) {
      this->just_reported();
      this->report_state();
    }
    if (this->is_update_expired() && this->is_request_expired() && this->is_request_possible() &&
        this->is_connected()) {
      this->parent_->set_last_requested(this->just_requested());
      this->request_state();
    }
  }
  virtual void report_state() {}
  virtual void request_state() {}
  void update() override {}

 protected:
  uint32_t last_reported_ = 0;
  uint32_t last_requested_ = 0;
  uint32_t last_updated_ = 0;
  uint32_t report_interval_ = 0;
};

}  // namespace kingsong_euc
}  // namespace esphome
