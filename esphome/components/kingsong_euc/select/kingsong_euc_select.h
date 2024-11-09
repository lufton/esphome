#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/select/select.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

#define REGISTER_SELECT(name) \
 protected: \
  KingSongEUCSelect *name##_select_{nullptr}; \
\
 public: \
  void set_##name##_select(KingSongEUCSelect *select) { \
    this->name##_select_ = select; \
    this->selects_[#name] = select; \
    select->set_parent(this); \
    select->set_type(#name); \
  }

namespace esphome {
namespace kingsong_euc {

class KingSongEUCSelect : public select::Select, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_SELECT("  ", this->get_type().c_str(), this); }
  void publish_state(const std::string &state) {
    select::Select::publish_state(state);
    this->just_updated();
  }

 protected:
  void control(const std::string &value) override {
    if (!this->is_connected())
      return;
    auto index = this->index_of(value);
    if (!index.has_value())
      return;
    if (this->type_ == "light_mode")
      this->get_parent()->send_request(CMD_SET_LIGHT_MODE, index.value() + 0x12);
    else if (this->type_ == "ride_mode")
      this->get_parent()->send_request(CMD_SET_RIDE_MODE, index.value(), {{3, 0xE0}});
    // this->parent_->set_select_value(this->type_, index.value());
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
