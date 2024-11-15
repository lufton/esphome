#ifdef USE_ESP32

#include "esphome/core/log.h"
#include "kingsong_euc.h"

namespace esphome {
namespace kingsong_euc {

enum MainLightModes { MAIN_LIGHT_MODE_ON = 0, MAIN_LIGHT_MODE_OFF, MAIN_LIGHT_MODE_AUTO };
enum MagicLightModes {
  MAGIC_LIGHT_MODE_ANNULAR_1 = 0,
  MAGIC_LIGHT_MODE_ANNULAR_2,
  MAGIC_LIGHT_MODE_ANNULAR_3,
  MAGIC_LIGHT_MODE_ANNULAR_4
};
enum RideModes { RIDE_MODE_HARD = 0, RIDE_MODE_MEDIUM, RIDE_MODE_SOFT };
enum SpectrumLightModes {
  SPECTRUM_LIGHT_MODE_AUTOMATIC = 0,
  SPECTRUM_LIGHT_MODE_BEATING,
  SPECTRUM_LIGHT_MODE_FLASHING,
  SPECTRUM_LIGHT_MODE_ALTERNATE
};
enum VoiceLanguages { VOICE_LANGUAGE_ENGLISH = 0, VOICE_LANGUAGE_CHINESE };
const char *magic_light_mode_options[] = {[MAGIC_LIGHT_MODE_ANNULAR_1] = "Annular 1",
                                          [MAGIC_LIGHT_MODE_ANNULAR_2] = "Annular 2",
                                          [MAGIC_LIGHT_MODE_ANNULAR_3] = "Annular 3",
                                          [MAGIC_LIGHT_MODE_ANNULAR_4] = "Annular 4"};
const char *main_light_mode_options[] = {
    [MAIN_LIGHT_MODE_ON] = "On", [MAIN_LIGHT_MODE_OFF] = "Off", [MAIN_LIGHT_MODE_AUTO] = "Auto"};
const char *ride_mode_options[] = {[RIDE_MODE_HARD] = "Hard", [RIDE_MODE_MEDIUM] = "Medium", [RIDE_MODE_SOFT] = "Soft"};
const char *spectrum_light_mode_options[] = {[SPECTRUM_LIGHT_MODE_AUTOMATIC] = "Automatic",
                                             [SPECTRUM_LIGHT_MODE_BEATING] = "Beating",
                                             [SPECTRUM_LIGHT_MODE_FLASHING] = "Flashing",
                                             [SPECTRUM_LIGHT_MODE_ALTERNATE] = "Alternate"};
const char *voice_language_options[] = {[VOICE_LANGUAGE_ENGLISH] = "English", [VOICE_LANGUAGE_CHINESE] = "Chinese"};

void KingSongEUC::dump_config() {
  ESP_LOGCONFIG(TAG, "KingSongEUC:");
  // for (const auto &pair : this->sensors_)
  //   pair.second->dump_config();  // LOG_SENSOR("  ", pair.second->get_type().c_str(), pair.second);
}

void KingSongEUC::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                      esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = esp32_ble_tracker::ClientState::IDLE;
      this->status_set_warning("Disconnected from EUC");
      for (KingSongEUCSensor *sensor : this->sensors_)
        PUBLISH_STATE(sensor, NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(SERVICE_UUID, CHARACTERISTIC_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not a KingSong EUC..?",
                 this->parent_->address_str().c_str());
        this->status_set_warning("No control service found at device, not a KingSong EUC..?");
        break;
      }
      this->char_handle_ = chr->handle;
      auto status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(),
                                                      chr->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
        this->status_set_warning("Failed to register for notifications, not a KingSong EUC..?");
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->status_clear_warning();
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      if (param->notify.conn_id != this->parent()->get_conn_id())
        break;
      if (param->notify.handle != this->char_handle_)
        break;
      if (param->notify.value_len != sizeof(KingSongEUCCommand))
        break;
      if (param->notify.value[0] != 0xAA || param->notify.value[1] != 0x55)
        break;
      KingSongEUCCodec *codec = this->get_codec();
      codec->save_buffer(param->notify.value);
      uint8_t bms_num = codec->get_packet() - PKT_BMS1 + 1;
      switch (codec->get_packet()) {
        case PKT_STANDBY_DELAY:  // 63
          PUBLISH_STATE(this->standby_delay_number_, codec->get_standby_delay());
          break;
        // case PKT_OLD_MODEL: // 72
        //   break;
        case PKT_SPECTRUM_LIGHT:  // 74
          PUBLISH_STATE(this->spectrum_light_switch_, codec->get_spectrum_light());
          break;
        case PKT_LIFT_SENSOR:  // 76
          PUBLISH_STATE(this->lift_sensor_switch_, codec->get_lift_sensor());
          break;
        case PKT_SPECTRUM_LIGHT_MODE:  // 77
          PUBLISH_STATE(this->spectrum_light_mode_select_,
                        spectrum_light_mode_options[codec->get_spectrum_light_mode()]);
          break;
        case PKT_MAGIC_LIGHT_MODE:  // 82
          PUBLISH_STATE(this->magic_light_mode_select_, magic_light_mode_options[codec->get_magic_light_mode()]);
          break;
        case PKT_STROBE:  // 85
          PUBLISH_STATE(this->strobe_switch_, codec->get_strobe());
          break;
        case PKT_MUSIC_BT:  // 88
          PUBLISH_STATE(this->music_bluetooth_switch_, codec->get_music_bluetooth());
          break;
        // case PKT_COLORS: // 92
        //   break;
        case PKT_LOCK:  // 95
          PUBLISH_STATE(this->lock_lock_, codec->get_lock());
          break;
        case PKT_VOICE_LANGUAGE:  // 107
          PUBLISH_STATE(this->voice_language_select_, voice_language_options[codec->get_voice_language()]);
          break;
        case PKT_CIRCLE_LIGHT:  // 110
          PUBLISH_STATE(this->circle_light_switch_, codec->get_circle_light());
          break;
        // case PKT_CALIBRATE_TILT: // 138
        //   break;
        case PKT_A9:  // 169
          PUBLISH_STATE(this->voltage_sensor_, codec->get_voltage());
          PUBLISH_STATE(this->speed_sensor_, codec->get_speed());
          PUBLISH_STATE(this->odometer_sensor_, codec->get_odometer());
          PUBLISH_STATE(this->current_sensor_, codec->get_current());
          PUBLISH_STATE(this->mosfet_temperature_sensor_, codec->get_mosfet_temperature());
          PUBLISH_STATE(this->ride_mode_select_, ride_mode_options[codec->get_ride_mode()]);
          PUBLISH_STATE(this->power_sensor_, codec->get_power());
          break;
        // case PKT_RIDE_PARAM_1: // 172
        //   break;
        // case PKT_RIDE_PARAM_2: // 173
        //   break;
        // case PKT_RIDE_PARAM_3: // 174
        //   break;
        // case PKT_FACTORY_RESET: // 177
        //   break;
        case PKT_SERIAL:  // 179
          PUBLISH_STATE(this->serial_text_sensor_, codec->get_serial());
          break;
        case PKT_ALARMS:  // 181
          PUBLISH_STATE(this->alarm_1_number_, codec->get_alarm_1());
          PUBLISH_STATE(this->alarm_2_number_, codec->get_alarm_2());
          PUBLISH_STATE(this->alarm_3_number_, codec->get_alarm_3());
          PUBLISH_STATE(this->tilt_back_number_, codec->get_tilt_back());
          break;
        case PKT_B9:  // 185
          PUBLISH_STATE(this->trip_distance_sensor_, codec->get_trip_distance());
          PUBLISH_STATE(this->uptime_sensor_, codec->get_uptime());
          PUBLISH_STATE(this->trip_max_speed_sensor_, codec->get_trip_max_speed());
          PUBLISH_STATE(this->main_light_mode_select_, main_light_mode_options[codec->get_main_light_mode() - 18]);
          PUBLISH_STATE(this->voice_switch_, codec->get_voice());
          PUBLISH_STATE(this->fan_binary_sensor_, codec->get_fan());
          PUBLISH_STATE(this->charging_binary_sensor_, codec->get_charging());
          PUBLISH_STATE(this->motor_temperature_sensor_, codec->get_motor_temperature());
          break;
        case PKT_MODEL:  // 187
          PUBLISH_STATE(this->model_text_sensor_, codec->get_model());
          break;
        // case PKT_C9: // 201
        //   break;
        case PKT_BMS1:  // 241
          switch (codec->get_bms_packet()) {
            case GENERAL:
              PUBLISH_STATE(this->bms_1_voltage_sensor_, codec->get_bms_1_voltage());
              PUBLISH_STATE(this->bms_1_current_sensor_, codec->get_bms_1_current());
              PUBLISH_STATE(this->bms_1_remaining_capacity_sensor_, codec->get_bms_1_remaining_capacity());
              PUBLISH_STATE(this->bms_1_factory_capacity_sensor_, codec->get_bms_1_factory_capacity());
              PUBLISH_STATE(this->bms_1_full_cycles_sensor_, codec->get_bms_1_full_cycles());
              PUBLISH_STATE(this->bms_1_soc_sensor_, codec->get_bms_1_soc());
              break;
            case TEMP:
              PUBLISH_STATE(this->bms_1_temperature_1_sensor_, codec->get_bms_1_temperature_1());
              PUBLISH_STATE(this->bms_1_temperature_2_sensor_, codec->get_bms_1_temperature_2());
              PUBLISH_STATE(this->bms_1_temperature_3_sensor_, codec->get_bms_1_temperature_3());
              PUBLISH_STATE(this->bms_1_temperature_4_sensor_, codec->get_bms_1_temperature_4());
              PUBLISH_STATE(this->bms_1_temperature_5_sensor_, codec->get_bms_1_temperature_5());
              PUBLISH_STATE(this->bms_1_temperature_6_sensor_, codec->get_bms_1_temperature_6());
              PUBLISH_STATE(this->bms_1_mosfet_temperature_sensor_, codec->get_bms_1_mosfet_temperature());
              break;
            case CELL_GROUP_1:
              PUBLISH_STATE(this->bms_1_cell_1_voltage_sensor_, codec->get_bms_1_cell_1_voltage());
              PUBLISH_STATE(this->bms_1_cell_2_voltage_sensor_, codec->get_bms_1_cell_2_voltage());
              PUBLISH_STATE(this->bms_1_cell_3_voltage_sensor_, codec->get_bms_1_cell_3_voltage());
              PUBLISH_STATE(this->bms_1_cell_4_voltage_sensor_, codec->get_bms_1_cell_4_voltage());
              PUBLISH_STATE(this->bms_1_cell_5_voltage_sensor_, codec->get_bms_1_cell_5_voltage());
              PUBLISH_STATE(this->bms_1_cell_6_voltage_sensor_, codec->get_bms_1_cell_6_voltage());
              PUBLISH_STATE(this->bms_1_cell_7_voltage_sensor_, codec->get_bms_1_cell_7_voltage());
              break;
            case CELL_GROUP_2:
              PUBLISH_STATE(this->bms_1_cell_8_voltage_sensor_, codec->get_bms_1_cell_8_voltage());
              PUBLISH_STATE(this->bms_1_cell_9_voltage_sensor_, codec->get_bms_1_cell_9_voltage());
              PUBLISH_STATE(this->bms_1_cell_10_voltage_sensor_, codec->get_bms_1_cell_10_voltage());
              PUBLISH_STATE(this->bms_1_cell_11_voltage_sensor_, codec->get_bms_1_cell_11_voltage());
              PUBLISH_STATE(this->bms_1_cell_12_voltage_sensor_, codec->get_bms_1_cell_12_voltage());
              PUBLISH_STATE(this->bms_1_cell_13_voltage_sensor_, codec->get_bms_1_cell_13_voltage());
              PUBLISH_STATE(this->bms_1_cell_14_voltage_sensor_, codec->get_bms_1_cell_14_voltage());
              break;
            case CELL_GROUP_3:
              PUBLISH_STATE(this->bms_1_cell_15_voltage_sensor_, codec->get_bms_1_cell_15_voltage());
              PUBLISH_STATE(this->bms_1_cell_16_voltage_sensor_, codec->get_bms_1_cell_16_voltage());
#if KINGSONG_EUC_CELL_COUNT > 16
              PUBLISH_STATE(this->bms_1_cell_17_voltage_sensor_, codec->get_bms_1_cell_17_voltage());
              PUBLISH_STATE(this->bms_1_cell_18_voltage_sensor_, codec->get_bms_1_cell_18_voltage());
              PUBLISH_STATE(this->bms_1_cell_19_voltage_sensor_, codec->get_bms_1_cell_19_voltage());
              PUBLISH_STATE(this->bms_1_cell_20_voltage_sensor_, codec->get_bms_1_cell_20_voltage());
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_1_cell_21_voltage_sensor_, codec->get_bms_1_cell_21_voltage());
#endif
              break;
            case CELL_GROUP_4:
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_1_cell_22_voltage_sensor_, codec->get_bms_1_cell_22_voltage());
              PUBLISH_STATE(this->bms_1_cell_23_voltage_sensor_, codec->get_bms_1_cell_23_voltage());
              PUBLISH_STATE(this->bms_1_cell_24_voltage_sensor_, codec->get_bms_1_cell_24_voltage());
              PUBLISH_STATE(this->bms_1_cell_25_voltage_sensor_, codec->get_bms_1_cell_25_voltage());
              PUBLISH_STATE(this->bms_1_cell_26_voltage_sensor_, codec->get_bms_1_cell_26_voltage());
              PUBLISH_STATE(this->bms_1_cell_27_voltage_sensor_, codec->get_bms_1_cell_27_voltage());
              PUBLISH_STATE(this->bms_1_cell_28_voltage_sensor_, codec->get_bms_1_cell_28_voltage());
#endif
              break;
            case CELL_GROUP_5:
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_1_cell_29_voltage_sensor_, codec->get_bms_1_cell_29_voltage());
              PUBLISH_STATE(this->bms_1_cell_30_voltage_sensor_, codec->get_bms_1_cell_30_voltage());
#endif
              break;
          }
          break;
        case PKT_BMS2:  // 242
          switch (codec->get_bms_packet()) {
            case GENERAL:
              PUBLISH_STATE(this->bms_2_voltage_sensor_, codec->get_bms_2_voltage());
              PUBLISH_STATE(this->bms_2_current_sensor_, codec->get_bms_2_current());
              PUBLISH_STATE(this->bms_2_remaining_capacity_sensor_, codec->get_bms_2_remaining_capacity());
              PUBLISH_STATE(this->bms_2_factory_capacity_sensor_, codec->get_bms_2_factory_capacity());
              PUBLISH_STATE(this->bms_2_full_cycles_sensor_, codec->get_bms_2_full_cycles());
              PUBLISH_STATE(this->bms_2_soc_sensor_, codec->get_bms_2_soc());
              break;
            case TEMP:
              PUBLISH_STATE(this->bms_2_temperature_1_sensor_, codec->get_bms_2_temperature_1());
              PUBLISH_STATE(this->bms_2_temperature_2_sensor_, codec->get_bms_2_temperature_2());
              PUBLISH_STATE(this->bms_2_temperature_3_sensor_, codec->get_bms_2_temperature_3());
              PUBLISH_STATE(this->bms_2_temperature_4_sensor_, codec->get_bms_2_temperature_4());
              PUBLISH_STATE(this->bms_2_temperature_5_sensor_, codec->get_bms_2_temperature_5());
              PUBLISH_STATE(this->bms_2_temperature_6_sensor_, codec->get_bms_2_temperature_6());
              PUBLISH_STATE(this->bms_2_mosfet_temperature_sensor_, codec->get_bms_2_mosfet_temperature());
              break;
            case CELL_GROUP_1:
              PUBLISH_STATE(this->bms_2_cell_1_voltage_sensor_, codec->get_bms_2_cell_1_voltage());
              PUBLISH_STATE(this->bms_2_cell_2_voltage_sensor_, codec->get_bms_2_cell_2_voltage());
              PUBLISH_STATE(this->bms_2_cell_3_voltage_sensor_, codec->get_bms_2_cell_3_voltage());
              PUBLISH_STATE(this->bms_2_cell_4_voltage_sensor_, codec->get_bms_2_cell_4_voltage());
              PUBLISH_STATE(this->bms_2_cell_5_voltage_sensor_, codec->get_bms_2_cell_5_voltage());
              PUBLISH_STATE(this->bms_2_cell_6_voltage_sensor_, codec->get_bms_2_cell_6_voltage());
              PUBLISH_STATE(this->bms_2_cell_7_voltage_sensor_, codec->get_bms_2_cell_7_voltage());
              break;
            case CELL_GROUP_2:
              PUBLISH_STATE(this->bms_2_cell_8_voltage_sensor_, codec->get_bms_2_cell_8_voltage());
              PUBLISH_STATE(this->bms_2_cell_9_voltage_sensor_, codec->get_bms_2_cell_9_voltage());
              PUBLISH_STATE(this->bms_2_cell_10_voltage_sensor_, codec->get_bms_2_cell_10_voltage());
              PUBLISH_STATE(this->bms_2_cell_11_voltage_sensor_, codec->get_bms_2_cell_11_voltage());
              PUBLISH_STATE(this->bms_2_cell_12_voltage_sensor_, codec->get_bms_2_cell_12_voltage());
              PUBLISH_STATE(this->bms_2_cell_13_voltage_sensor_, codec->get_bms_2_cell_13_voltage());
              PUBLISH_STATE(this->bms_2_cell_14_voltage_sensor_, codec->get_bms_2_cell_14_voltage());
              break;
            case CELL_GROUP_3:
              PUBLISH_STATE(this->bms_2_cell_15_voltage_sensor_, codec->get_bms_2_cell_15_voltage());
              PUBLISH_STATE(this->bms_2_cell_16_voltage_sensor_, codec->get_bms_2_cell_16_voltage());
#if KINGSONG_EUC_CELL_COUNT > 16
              PUBLISH_STATE(this->bms_2_cell_17_voltage_sensor_, codec->get_bms_2_cell_17_voltage());
              PUBLISH_STATE(this->bms_2_cell_18_voltage_sensor_, codec->get_bms_2_cell_18_voltage());
              PUBLISH_STATE(this->bms_2_cell_19_voltage_sensor_, codec->get_bms_2_cell_19_voltage());
              PUBLISH_STATE(this->bms_2_cell_20_voltage_sensor_, codec->get_bms_2_cell_20_voltage());
#endif
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_2_cell_21_voltage_sensor_, codec->get_bms_2_cell_21_voltage());
#endif
              break;
            case CELL_GROUP_4:
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_2_cell_22_voltage_sensor_, codec->get_bms_2_cell_22_voltage());
              PUBLISH_STATE(this->bms_2_cell_23_voltage_sensor_, codec->get_bms_2_cell_23_voltage());
              PUBLISH_STATE(this->bms_2_cell_24_voltage_sensor_, codec->get_bms_2_cell_24_voltage());
              PUBLISH_STATE(this->bms_2_cell_25_voltage_sensor_, codec->get_bms_2_cell_25_voltage());
              PUBLISH_STATE(this->bms_2_cell_26_voltage_sensor_, codec->get_bms_2_cell_26_voltage());
              PUBLISH_STATE(this->bms_2_cell_27_voltage_sensor_, codec->get_bms_2_cell_27_voltage());
              PUBLISH_STATE(this->bms_2_cell_28_voltage_sensor_, codec->get_bms_2_cell_28_voltage());
#endif
              break;
            case CELL_GROUP_5:
#if KINGSONG_EUC_CELL_COUNT > 20
              PUBLISH_STATE(this->bms_2_cell_29_voltage_sensor_, codec->get_bms_2_cell_29_voltage());
              PUBLISH_STATE(this->bms_2_cell_30_voltage_sensor_, codec->get_bms_2_cell_30_voltage());
#endif
              break;
          }
          break;
        // case PKT_F3: // 243
        //   break;
        // case PKT_F4: // 244
        //   break;
        case PKT_F5:  // 245
          PUBLISH_STATE(this->phase_short_circuit_binary_sensor_, codec->get_phase_short_circuit());
          PUBLISH_STATE(this->gyroscope_error_binary_sensor_, codec->get_gyroscope_error());
          PUBLISH_STATE(this->hall_sensor_error_binary_sensor_, codec->get_hall_sensor_error());
          PUBLISH_STATE(this->cpu_load_sensor_, codec->get_cpu_load());
          PUBLISH_STATE(this->pwm_sensor_, codec->get_pwm());
          break;
        case PKT_F6:  // 246
          PUBLISH_STATE(this->speed_limit_sensor_, codec->get_speed_limit());
          PUBLISH_STATE(this->ride_time_sensor_, codec->get_ride_time());
          PUBLISH_STATE(this->error_code_sensor_, codec->get_error_code());
          PUBLISH_STATE(this->error_description_text_sensor_, codec->get_error_description());
          break;
      }
      // } else if (packet_type >= PKT_BMS1 && packet_type <= PKT_BMS2) {
      //   //   uint8_t bms_num = packet_type - PKT_BMS1 + 1;
      //   //   if (this->get_codec()->get_bms_packet_type() == GENERAL) {
      //   //     auto packet = this->get_codec()->get_packet<KingSongEUCBMSGeneralPacket>();
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_current"),
      //   //                          packet->current / 100.0f, 0.1f);
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_factory_capacity"),
      //   //                          packet->factory_capacity * 10);
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_full_cycles"),
      //   //                          packet->full_cycles);
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_soc"),
      //   //                          (float) packet->remaining_capacity / packet->factory_capacity * 100.0f);
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_remaining_capacity"),
      //   //                          packet->remaining_capacity * 10);
      //   //     this->publish_state_(this->get_sensor_("bms_" + std::to_string(bms_num) + "_voltage"),
      //   //                          packet->voltage / 100.0f, 0.1f);
      //   //     this->assert_word(12, packet->factory_capacity);
      //   //     // this->assert_byte(14, bms_num == 1 ? 106 : 119); // 111 → 109 → 108 → 106 125 → 124 → 122 → 119
      //   //     if (bms_num <= 2)
      //   //       this->assert_byte(15, 15);
      //   //   } else if (this->get_codec()->get_bms_packet_type() >= CELL_GROUP_1 &&
      //   //              this->get_codec()->get_bms_packet_type() <= CELL_GROUP_5) {
      //   //     auto packet = this->get_codec()->get_packet<KingSongEUCBMSCellGroupPacket>();
      //   //     uint8_t group_num = this->get_codec()->get_bms_packet_type() - CELL_GROUP_1;
      //   //     uint8_t cell_num = group_num * 7 + 1;
      //   //     for (int i = 0; i < 7 && cell_num <= 30; i++, cell_num++)
      //   //       this->publish_state_(
      //   //           this->get_sensor_("bms_" + std::to_string(bms_num) + "_cell_" + std::to_string(cell_num) +
      //   //           "_voltage"), packet->cells[i] / 1000.0f, 0.001f);
      //   //     if (this->get_codec()->get_bms_packet_type() == CELL_GROUP_5) {
      //   //       this->assert_byte({6, 7, 8, 9}, 0);
      //   //     }
      //   //   }
      // } else if (packet_type == PKT_F5) {
      //   //   auto packet = this->get_codec()->get_packet<KingSongEUCPacketMotorLineGyroMotorHolzerCPUPWM>();
      //   //   this->publish_state_(this->cpu_rate_sensor_, packet->cpu_rate);
      //   //   this->publish_state_(this->motor_hall_sensor_, packet->motor_hall);
      //   //   this->publish_state_(this->motor_phase_line_sensor_, packet->motor_phase_line);
      //   //   this->publish_state_(this->pwm_sensor_, packet->pwm);
      //   //   this->assert_byte({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 0);
      //   //   // this->publish_debug_data_();
      //   // } else if (packet_type == PKT_COLORS) {
      //   //   auto packet = this->get_codec()->get_packet<KingSongEUCPacketColors>();
      // } else if (packet_type == PKT_C9 || packet_type == PKT_F3 || packet_type == PKT_F4) {
      //   //   this->assert_byte({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 0);
      //   // } else {
      //   //   this->get_codec()->log_buffer();
      //   // }
      // } else {
      //   ESP_LOGE(TAG, "We received packet 0x%2x (%d)", this->get_codec()->get_packet_type(),
      //            this->get_codec()->get_packet_type());
      //   this->get_codec()->log_buffer();
      // }
      break;
    }
    default:
      break;
  }
}

}  // namespace kingsong_euc
}  // namespace esphome

#endif
