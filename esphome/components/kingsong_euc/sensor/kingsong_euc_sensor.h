#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "../const.h"
#include "../kingsong_euc_component.h"

namespace esphome {
namespace kingsong_euc {

class KingSongEUCSensor : public sensor::Sensor, public KingSongEUCComponent {
 public:
  void dump_config() { LOG_SENSOR("  ", this->get_type().c_str(), this); }
  void publish_state(float state) {
    sensor::Sensor::publish_state(state));
    this->just_updated();
  }
};

}  // namespace kingsong_euc
}  // namespace esphome
