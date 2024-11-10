#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/button/button.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_BUTTON(name) \
 protected: \
  KingSongEUCButton *name##_button_{nullptr}; \
\
 public: \
  void set_##name##_button(KingSongEUCButton *button) { \
    this->name##_button_ = button; \
    button->set_parent(this); \
    button->set_type(#name); \
  }

class KingSongEUCButton : public button::Button, public KingSongEUCComponent {

 public:
  void dump_config() { LOG_BUTTON("  ", this->get_type().c_str(), this); }

 protected:
  void press_action() {
    if (!this->is_connected())
      return;
    if (this->get_type() == "beep")
      this->get_parent()->beep();
    else if (this->get_type() == "horn")
      this->get_parent()->horn();
    else if (this->get_type() == "power_off")
      this->get_parent()->power_off();
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
