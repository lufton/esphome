#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/lock/lock.h"
#include "const.h"
#include "component.h"

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
  }

enum class KingSongEUCLockType {
  LOCK,
};

class KingSongEUCLock : public lock::Lock, public KingSongEUCComponent {

 public:
  KingSongEUCLock(KingSongEUCLockType lock_type, uint32_t report_interval) : KingSongEUCComponent(report_interval) {
    this->lock_type_ = lock_type;
  }

  void dump_config() {
    // LOG_LOCK("  ", this->get_type().c_str(), this);
  }

  void publish_state(lock::LockState state) {
    lock::LockState prev_state = this->state;
    this->state = state;
    this->just_updated();
    if (state != prev_state) this->report_state();
  }

  void report_state() override {
    if (this->last_updated_ == 0) return;
    lock::Lock::publish_state(this->state);
    this->just_reported();
  }

  void update() override {
    if (!this->is_connected()) return;
    if (this->get_last_updated() > 0) return;
    switch (this->lock_type_) {
      case KingSongEUCLockType::LOCK: return this->get_parent()->get_lock();
      default: break;
    }
  }

 protected:
  KingSongEUCLockType lock_type_;

  void control(const lock::LockCall &call) {
    if (!this->is_connected()) return;
    lock::LockState state = *call.get_state();
    if (state == lock::LOCK_STATE_LOCKED) {
      switch (this->lock_type_) {
        case KingSongEUCLockType::LOCK: return this->get_parent()->lock();
        default: break;
      }
    } else if (state == lock::LOCK_STATE_UNLOCKED) {
      switch (this->lock_type_) {
        case KingSongEUCLockType::LOCK: return this->get_parent()->unlock();
        default: break;
      }
    }
    this->publish_state(state);
  }

};

}  // namespace kingsong_euc
}  // namespace esphome
