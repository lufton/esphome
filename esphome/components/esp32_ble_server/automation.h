#pragma once

#ifdef USE_ESP32
#include "ble_characteristic.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace esp32_ble_server {

static const char *const TAG = "esp32_ble_server.automation";

class BLECharacteristicWriteTrigger : public Trigger<std::vector<uint8_t>> {
 public:
  explicit BLECharacteristicWriteTrigger(BLECharacteristic *characteristic) {
    characteristic->add_on_write_callback([this](std::vector<uint8_t> data) { this->trigger(data); });
  }
};

template<typename... Ts> class BLECharacteristicSetValueAction : public Action<Ts...> {
 public:
  explicit BLECharacteristicSetValueAction(BLECharacteristic *characteristic, bool notify = false)
      : characteristic_(characteristic), notify_(notify) {}

  void play(Ts... x) override {
    std::vector<uint8_t> value = this->has_simple_value_ ? this->value_simple_ : this->value_template_(x...);
    this->characteristic_->set_value(value.data(), value.size());
    if (this->notify_) {
      if (this->characteristic_->is_notify_property_set())
        this->characteristic_->notify();
      else
        ESP_LOGW(TAG, "'notify' feature can't be used if 'notify' characteristic property is not declared.");
    }
  }

  void set_value_template(std::function<std::vector<uint8_t>(Ts...)> func) {
    this->value_template_ = std::move(func);
    has_simple_value_ = false;
  }

  void set_value_string(const std::string value) {
    this->value_simple_.assign(value.begin(), value.end());
    has_simple_value_ = true;
  }

  void set_value_vector(const std::vector<uint8_t> &value) {
    this->value_simple_ = value;
    has_simple_value_ = true;
  }

 protected:
  BLECharacteristic *characteristic_;
  bool has_simple_value_ = true;
  bool notify_ = false;
  std::vector<uint8_t> value_simple_{0};
  std::function<std::vector<uint8_t>(Ts...)> value_template_{};
};

}  // namespace esp32_ble_server
}  // namespace esphome

#endif
