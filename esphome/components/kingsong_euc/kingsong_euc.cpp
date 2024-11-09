#ifdef USE_ESP32

#include "esphome/core/log.h"
#include "error_codes.h"
#include "kingsong_euc.h"

namespace esphome {
namespace kingsong_euc {

enum LightModes { LIGHT_MODE_CITY = 0, LIGHT_MODE_ON = 1, LIGHT_MODE_OFF = 2, LIGHT_MODE_AUTO = 3 };
const char *light_mode_options[] = {
    [LIGHT_MODE_CITY] = "City", [LIGHT_MODE_ON] = "On", [LIGHT_MODE_OFF] = "Off", [LIGHT_MODE_AUTO] = "Auto"};
enum RideModes { RIDE_MODE_HARD = 0, RIDE_MODE_MEDIUM = 1, RIDE_MODE_SOFT = 2 };
const char *ride_mode_options[] = {[RIDE_MODE_HARD] = "Hard", [RIDE_MODE_MEDIUM] = "Medium", [RIDE_MODE_SOFT] = "Soft"};

void KingSongEUC::dump_config() {
  ESP_LOGCONFIG(TAG, "KingSongEUC:");
  // for (const auto &pair : this->sensors_)
  //   pair.second->dump_config();  // LOG_SENSOR("  ", pair.second->get_type().c_str(), pair.second);
}

// void KingSongEUC::update() {
// for (const auto &pair : this->binary_sensors_)
//   if (pair.second->has_state())
//     pair.second->publish_state(pair.second->state);
// for (const auto &pair : this->selects_)
//   if (pair.second->has_state())
//     pair.second->publish_state(pair.second->state);
// for (const auto &pair : this->sensors_)
//   if (pair.second->has_state())
//     pair.second->publish_state(pair.second->state);
// for (const auto &pair : this->text_sensors_)
//   if (pair.second->has_state())
//     pair.second->publish_state(pair.second->state);
// }

void KingSongEUC::publish_debug_data_() {
  auto buffer = this->codec_->get_packet<KingSongEUCBuffer>();
  // ESP_LOGE(TAG, "Start of a buffer debug, type %d (0x%02x)", buffer->packet_type, buffer->packet_type);
  char hex[10];
  sprintf(hex, "0x%X", buffer->packet_type);
  // this->publish_state_(this->packet_type_sensor_, buffer->packet_type);
  // this->publish_state_(this->packet_type_hex_text_sensor_, std::string(hex));
  // for (int i = 0; i < 14; i++)
  //   this->publish_state_(this->get_sensor_("byte" + std::to_string(i + 2)), buffer->data_byte[i] / 1.0f);
  // for (int i = 0; i < 13; i++)
  //   this->publish_state_(this->get_sensor_("word" + std::to_string(i + 2) + "_" + std::to_string(i + 3)),
  //                        (uint16_t) (buffer->data_byte[i + 1] << 8 | buffer->data_byte[i]) / 100.0f);
  // for (int i = 0; i < 11; i++)
  //   this->publish_state_(this->get_sensor_("dword" + std::to_string(i + 2) + "_" + std::to_string(i + 5)),
  //                        (uint32_t) (buffer->data_byte[i + 1] << 24 | buffer->data_byte[i] << 16 |
  //                                    buffer->data_byte[i + 3] << 8 | buffer->data_byte[i + 2]) /
  //                            1000.0f);
  // ESP_LOGE(TAG, "End of a buffer debug, type %d (0x%02X)", buffer->packet_type, buffer->packet_type);
}

void KingSongEUC::assert_byte(uint8_t index, uint8_t expected) {
  auto buffer = this->codec_->get_packet<KingSongEUCBuffer>();
  uint8_t actual = buffer->data_byte[index - 2];
  if (actual != expected)
    ESP_LOGE(TAG, "[%d (0x%02X) packet] byte %d suppose to be %d, but it is %d", this->codec_->get_packet_type(),
             this->codec_->get_packet_type(), index, expected, actual);
}

void KingSongEUC::assert_byte(std::vector<uint8_t> indices, uint8_t expected) {
  for (uint8_t index : indices)
    this->assert_byte(index, expected);
}

void KingSongEUC::assert_word(uint8_t index, uint16_t expected) {
  auto buffer = this->codec_->get_packet<KingSongEUCBuffer>();
  uint16_t actual = buffer->data_byte[index - 1] << 8 | buffer->data_byte[index - 2];
  if (actual != expected)
    ESP_LOGE(TAG, "[%d (0x%02X) packet] word %d-%d suppose to be %d, but it is %d", this->codec_->get_packet_type(),
             this->codec_->get_packet_type(), index, index + 1, expected, actual);
}

void KingSongEUC::assert_word(std::vector<uint8_t> indices, uint16_t expected) {
  for (uint8_t index : indices)
    this->assert_word(index, expected);
}

void KingSongEUC::assert_dword(uint8_t index, uint32_t expected) {
  auto buffer = this->codec_->get_packet<KingSongEUCBuffer>();
  uint32_t actual = buffer->data_byte[index - 1] << 24 | buffer->data_byte[index - 2] << 16 |
                    buffer->data_byte[index + 1] << 8 | buffer->data_byte[index];
  if (actual != expected)
    ESP_LOGE(TAG, "[%d (0x%02X) packet] dword %d-%d suppose to be %d, but it is %d", this->codec_->get_packet_type(),
             this->codec_->get_packet_type(), index, index + 3, expected, actual);
}

void KingSongEUC::assert_dword(std::vector<uint8_t> indices, uint32_t expected) {
  for (uint8_t index : indices)
    this->assert_dword(index, expected);
}

void KingSongEUC::send_request(KingSongEUCBuffer *request) {
  ESP_LOGE(
      TAG, "Request is: %04X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
      request->header, request->data_byte[0], request->data_byte[1], request->data_byte[2], request->data_byte[3],
      request->data_byte[4], request->data_byte[5], request->data_byte[6], request->data_byte[7], request->data_byte[8],
      request->data_byte[9], request->data_byte[10], request->data_byte[11], request->data_byte[12],
      request->data_byte[13], request->packet_type, request->tail[0], request->tail[1], request->tail[2]);
  ESP_LOGE(TAG, "     Index: 0001 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
  auto status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                         this->char_handle_, sizeof(KingSongEUCBuffer), (uint8_t *) request,
                                         ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status)
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
}

void KingSongEUC::send_request(KingSongEUCPacketTypes type) { this->send_request(type, 0x0000, {}); }

void KingSongEUC::send_request(KingSongEUCPacketTypes type, uint16_t value) { this->send_request(type, value, {}); }

void KingSongEUC::send_request(KingSongEUCPacketTypes type, uint16_t value, std::map<uint8_t, uint8_t> bytes) {
  bytes[2] = value & 0xFF;
  bytes[3] = (value >> 8) & 0xFF;
  auto request = this->codec_->get_request(type, bytes);
  this->send_request(request);
}

void KingSongEUC::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                      esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = esp32_ble_tracker::ClientState::IDLE;
      this->status_set_warning();
      // for (const auto &pair : this->sensors_)
      //   this->publish_state_(pair.second, NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(SERVICE_UUID, CHARACTERISTIC_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an KingSong EUC..?",
                 this->parent_->address_str().c_str());
        this->status_set_warning();
        break;
      }
      this->char_handle_ = chr->handle;
      auto status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(),
                                                      chr->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
        this->status_set_warning();
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      // this->send_request(CMD_GET_LOCK);
      // this->send_request(CMD_GET_MODEL);
      // this->send_request(CMD_GET_SERIAL);
      // this->send_request(CMD_GET_STANDBY_DELAY);
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      // ESP_LOGW(TAG, "We received ESP_GATTC_NOTIFY_EVT event");
      if (param->notify.handle != this->char_handle_)
        break;
      if (param->notify.value[0] != 0xAA || param->notify.value[1] != 0x55 || param->notify.value_len < 20)
        break;
      this->codec_->save_buffer(param->notify.value, param->notify.value_len);
      auto packet_type = this->codec_->get_packet_type();
      // uint32_t now = millis();
      // auto pair = this->latest_updates_.find(packet_type);
      // if (pair != this->latest_updates_.end() && now - pair->second < 1000) break;
      // this->latest_updates_[packet_type] = now;
      uint16_t value = this->codec_->get_value();
      if (packet_type == PKT_LOCK)
        this->lock_lock_->publish_state(value == 1 ? lock::LOCK_STATE_LOCKED : lock::LOCK_STATE_UNLOCKED);
      else if (packet_type == PKT_LIFT_SENSOR)
        this->lift_sensor_switch_->publish_state(value == 1);
      // else if (packet_type == PKT_MUSIC_BT) this->music_bluetooth_binary_sensor_->publish_state(value1 == 1);
      // else if (packet_type == PKT_VOICE_LANGUAGE) this->voice_language_text_sensor_->publish_state(value1 == 0 ?
      // "English" : "Chinese");
      else if (packet_type == PKT_STROBE)
        this->strobe_switch_->publish_state(value == 1);
      // else if (packet_type == PKT_SPECTRUM_LIGHT_MODE) this->spectrum_light_mode_sensor_->publish_state(value1);
      // else if (packet_type == PKT_CIRCLE_LIGHT) this->circle_light_binary_sensor_->publish_state(value1 == 1);
      // else if (packet_type == PKT_MAGIC_LIGHT) this->magic_light_mode_sensor_->publish_state(value1);
      else if (packet_type == PKT_SPECTRUM_LIGHT)
        this->spectrum_light_switch_->publish_state(value == 1);
      // // else if (packet_type == PKT_FACTORY_RESET) this->factory_reset_binary_sensor_->publish_state(value1);
      // else if (packet_type == PKT_OLD_MODEL) this->old_model_binary_sensor_->publish_state(value1);
      else if (packet_type == PKT_STANDBY_DELAY)
        this->standby_delay_number_->publish_state(this->codec_->get_word(4));
      else if (packet_type == PKT_VOL_SPD_ODO_CUR_MOSTEMP_RMODE) {
        //   auto packet = this->codec_->get_packet<KingSongEUCPacketVoltageSpeedOdoCurrentMosTempRideMode>();
        //   this->publish_state_(this->current_sensor_, packet->current / 100.0f, 0.1f);
        //   this->publish_state_(this->odometer_sensor_, (packet->odometer_high << 16 | packet->odometer_low) /
        //   1000.0f); this->publish_state_(this->power_sensor_, packet->current * packet->voltage / 10000.0f);
        //   this->publish_state_(this->ride_mode_select_, ride_mode_options[packet->ride_mode]);
        //   this->publish_state_(this->speed_sensor_, packet->speed / 100.0f);
        //   this->publish_state_(this->mosfet_temperature_sensor_, packet->mosfet_temperature / 100.0f);
        //   this->publish_state_(this->voltage_sensor_, packet->voltage / 100.0f, 0.1f);
        //   this->assert_byte(15, 0xE0);
      } else if (packet_type == PKT_TDIST_UPT_TSPD_LMODE_FAN_CHRG_MOTTEMP) {
        //   auto packet =
        //   this->codec_->get_packet<KingSongEUCPacketTripDistUptimeTripSpeedLightModeFanChargingMotorTemp>();
        //   this->publish_state_(this->charging_binary_sensor_, packet->charging_status == 1);
        //   this->publish_state_(this->fan_binary_sensor_, packet->fan_status == 1);
        //   this->publish_state_(this->light_mode_select_, light_mode_options[packet->light_mode - 18]);
        //   this->publish_state_(this->voice_binary_sensor_, packet->voice_status == 1);
        //   this->publish_state_(this->trip_distance_sensor_,
        //                        (packet->trip_distance_high << 16 | packet->trip_distance_low) / 1000.0f);
        //   this->publish_state_(this->trip_max_speed_sensor_, packet->trip_max_speed / 100.0f);
        //   this->publish_state_(this->motor_temperature_sensor_, packet->motor_temperature / 100.0f);
        //   this->publish_state_(this->uptime_sensor_, packet->uptime, 10);
        //   this->assert_byte(11, 1);  // light status
        //   this->assert_byte(12, 0);
      } else if (packet_type == PKT_ALARMS) {
        auto packet = this->codec_->get_packet<KingSongEUCPacketAlarms>();
        this->alarm_1_number_->publish_state(packet->alarm_1);
        this->alarm_2_number_->publish_state(packet->alarm_2);
        this->alarm_3_number_->publish_state(packet->alarm_3);
        this->tilt_back_number_->publish_state(packet->tilt_back);
        //   this->assert_byte({2, 3, 5, 7, 9, 11, 12, 13, 14, 15}, 0);
      } else if (packet_type == PKT_MODEL)
        this->model_text_sensor_->publish_state(this->codec_->get_string());
      else if (packet_type == PKT_SERIAL)
        this->serial_text_sensor_->publish_state(this->codec_->get_string());
      else if (packet_type == PKT_WARNINGS) {
        //   auto packet = this->codec_->get_packet<KingSongEUCPacketWarnings>();
        //   if (packet->error_code != 0) {
        //     this->publish_state_(this->error_code_sensor_, packet->error_code);
        //     this->publish_state_(this->error_description_text_sensor_, get_error_description(packet->error_code));
        //   }
        //   this->assert_byte({4, 5, 6, 7}, 0);
        //   this->assert_byte(8, 28);
        //   this->assert_byte(9, 58);
        //   this->assert_byte(10, 0);  // speed warning? 45 → 0
        //   this->assert_byte(11, 0);  // flag? 1 → 0 after restarting
        //   // this->assert_word(12, 179); // tRdT? 176 → 177 → 178 → 179 → 180
      } else if (packet_type >= PKT_BMS1 && packet_type <= PKT_BMS2) {
        //   uint8_t bms_num = packet_type - PKT_BMS1 + 1;
        //   if (this->codec_->get_bms_packet_type() == GENERAL) {
        //     auto packet = this->codec_->get_packet<KingSongEUCBMSGeneralPacket>();
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_current"),
        //                          packet->current / 100.0f, 0.1f);
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_factory_capacity"),
        //                          packet->factory_capacity * 10);
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_full_cycles"),
        //                          packet->full_cycles);
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_soc"),
        //                          (float) packet->remaining_capacity / packet->factory_capacity * 100.0f);
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_remaining_capacity"),
        //                          packet->remaining_capacity * 10);
        //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_voltage"),
        //                          packet->voltage / 100.0f, 0.1f);
        //     this->assert_word(12, packet->factory_capacity);
        //     // this->assert_byte(14, bms_num == 1 ? 106 : 119); // 111 → 109 → 108 → 106 125 → 124 → 122 → 119
        //     if (bms_num <= 2)
        //       this->assert_byte(15, 15);
        //   } else if (this->codec_->get_bms_packet_type() >= CELL_GROUP_1 &&
        //              this->codec_->get_bms_packet_type() <= CELL_GROUP_5) {
        //     auto packet = this->codec_->get_packet<KingSongEUCBMSCellGroupPacket>();
        //     uint8_t group_num = this->codec_->get_bms_packet_type() - CELL_GROUP_1;
        //     uint8_t cell_num = group_num * 7 + 1;
        //     for (int i = 0; i < 7 && cell_num <= 30; i++, cell_num++)
        //       this->publish_state_(
        //           this->get_sensor_("bms_" + std::to_string(bms_num) + "_cell_" + std::to_string(cell_num) +
        //           "_voltage"), packet->cells[i] / 1000.0f, 0.001f);
        //     if (this->codec_->get_bms_packet_type() == CELL_GROUP_5) {
        //       this->assert_byte({6, 7, 8, 9}, 0);
        //     }
        //   }
      } else if (packet_type == PKT_MOTLN_GYRO_MOTHOL_CPU_PWM) {
        //   auto packet = this->codec_->get_packet<KingSongEUCPacketMotorLineGyroMotorHolzerCPUPWM>();
        //   this->publish_state_(this->cpu_rate_sensor_, packet->cpu_rate);
        //   this->publish_state_(this->motor_hall_sensor_, packet->motor_hall);
        //   this->publish_state_(this->motor_phase_line_sensor_, packet->motor_phase_line);
        //   this->publish_state_(this->pwm_sensor_, packet->pwm);
        //   this->assert_byte({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 0);
        //   // this->publish_debug_data_();
        // } else if (packet_type == PKT_COLORS) {
        //   auto packet = this->codec_->get_packet<KingSongEUCPacketColors>();
      } else if (packet_type == PKT_UNKNOWN1 || packet_type == PKT_UNKNOWN2 || packet_type == PKT_UNKNOWN3) {
        //   this->assert_byte({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 0);
        // } else {
        //   this->codec_->log_buffer();
        // }
      } else {
        ESP_LOGE(TAG, "We received packet 0x%2x (%d)", this->codec_->get_packet_type(),
                 this->codec_->get_packet_type());
        this->codec_->log_buffer();
      }
      break;
    }
    default:
      break;
  }
}

}  // namespace kingsong_euc
}  // namespace esphome

#endif
