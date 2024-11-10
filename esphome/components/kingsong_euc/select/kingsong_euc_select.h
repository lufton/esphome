#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/select/select.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_SELECT(name) \
 protected: \
  KingSongEUCSelect *name##_select_{nullptr}; \
\
 public: \
  void set_##name##_select(SelectType type, KingSongEUCSelect *select) { \
    this->name##_select_ = select; \
    select->set_parent(this); \
    select->set_type(type); \
  }

enum class SelectType {
  NONE = 0,
  MAIN_LIGHT_MODE,
  RIDE_MODE,
};

class KingSongEUCSelect : public select::Select, public KingSongEUCComponent {

 public:
  void dump_config() { LOG_SELECT("  ", this->get_type().c_str(), this); }

  void publish_state(const std::string &state) {
    if (!this->is_state_expired() && this->state == state) return;
    select::Select::publish_state(state);
    this->just_updated();
  }

  void set_type(SelectType type) {
    this->type_ = type;
  }

 protected:
  SelectType type_ = SelectType::NONE;

  void control(const std::string &value) override {
    if (!this->is_connected())
      return;
    auto index = this->index_of(value);
    if (!index.has_value())
      return;
    if (this->type_ == SelectType::MAIN_LIGHT_MODE)
      this->get_parent()->set_main_light_mode(index.value());
    else if (this->type_ == SelectType::RIDE_MODE)
      this->get_parent()->set_ride_mode(index.value());
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
