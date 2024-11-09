#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/lock/lock.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

#define REGISTER_LOCK(name) \
 protected: \
  KingSongEUCLock *name##_lock_{nullptr}; \
\
 public: \
  void set_##name##_lock(KingSongEUCLock *lock) { \
    this->name##_lock_ = lock; \
    lock->set_parent(this); \
    lock->set_type(#name); \
  }

class KingSongEUCLock : public lock::Lock, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_LOCK("  ", this->get_type().c_str(), this); }
  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    if (this->get_type() == "lock") this->get_parent()->send_request(CMD_GET_LOCK);
  }
  void publish_state(lock::LockState state) {
    lock::Lock::publish_state(state);
    this->just_updated();
  }

 protected:
  void control(const lock::LockCall &call) {
    if (!this->is_connected()) return;
    lock::LockState state = *call.get_state();
    if (this->get_type() == "lock") {
      if (state == lock::LOCK_STATE_LOCKED) this->get_parent()->lock();
      else this->get_parent()->unlock();
    }
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
