#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/button/button.h"
#include "base_entity.h"
#include "const.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_BUTTON(name) \
 protected: \
  KingSongEUCButton *name##_button_{nullptr}; \
\
 public: \
  void set_##name##_button(KingSongEUCButton *button) { \
    this->buttons_.push_back(button); \
    this->name##_button_ = button; \
    button->set_parent(this); \
  }

enum class KingSongEUCButtonType {
  BEEP,
  HORN,
  POWER_OFF,
};

class KingSongEUCButton : public button::Button, public KingSongEUCBaseEntity {
 public:
  KingSongEUCButton(KingSongEUCButtonType button_type) { this->button_type_ = button_type; }

  void dump_config() {
    // LOG_BUTTON("  ", this->get_type().c_str(), this);
  }

 protected:
  KingSongEUCButtonType button_type_;

  void press_action() {
    if (!this->is_connected())
      return;
    switch (this->button_type_) {
      case KingSongEUCButtonType::BEEP:
        return this->get_parent()->beep();
      case KingSongEUCButtonType::HORN:
        return this->get_parent()->horn();
      case KingSongEUCButtonType::POWER_OFF:
        return this->get_parent()->power_off();
      default:
        break;
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
