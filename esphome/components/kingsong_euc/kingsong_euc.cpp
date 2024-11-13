#ifdef USE_ESP32

#include "esphome/core/log.h"
#include "error_codes.h"
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

KingSongEUCCodec *KingSongEUC::get_codec() { return codec_.get(); }

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
      // for (const auto &pair : this->sensors_)
      //   this->publish_state_(pair.second, NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(SERVICE_UUID, CHARACTERISTIC_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an KingSong EUC..?",
                 this->parent_->address_str().c_str());
        this->status_set_warning("No control service found at device, not an KingSong EUC..?");
        break;
      }
      this->char_handle_ = chr->handle;
      auto status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(),
                                                      chr->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
        this->status_set_warning("Failed to register for notifications, not an KingSong EUC..?");
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
      if (param->notify.value_len != sizeof(KingSongEUCBuffer))
        break;
      if (param->notify.value[0] != 0xAA || param->notify.value[1] != 0x55)
        break;
      KingSongEUCCodec *codec = this->get_codec();
      codec->save_buffer(param->notify.value);
      switch (codec->get_packet_type()) {
        case PKT_ALARMS:
          PUBLISH_STATE(alarm_1_number_, codec->get_alarm_1());
          PUBLISH_STATE(alarm_2_number_, codec->get_alarm_2());
          PUBLISH_STATE(alarm_3_number_, codec->get_alarm_3());
          PUBLISH_STATE(tilt_back_number_, codec->get_tilt_back());
          break;
        case PKT_CIRCLE_LIGHT:
          PUBLISH_STATE(circle_light_switch_, codec->get_circle_light());
          break;
        case PKT_LOCK:
          PUBLISH_STATE(lock_lock_, codec->get_lock());
          break;
        case PKT_LIFT_SENSOR:
          PUBLISH_STATE(lift_sensor_switch_, codec->get_lift_sensor());
          break;
        case PKT_MAGIC_LIGHT_MODE:
          PUBLISH_STATE(magic_light_mode_select_, magic_light_mode_options[codec->get_magic_light_mode()]);
          break;
        case PKT_MODEL:
          PUBLISH_STATE(model_text_sensor_, codec->get_model());
          break;
        case PKT_MUSIC_BT:
          PUBLISH_STATE(music_bluetooth_switch_, codec->get_music_bluetooth());
          break;
        case PKT_SERIAL:
          PUBLISH_STATE(serial_text_sensor_, codec->get_serial());
          break;
        case PKT_SPECTRUM_LIGHT:
          PUBLISH_STATE(spectrum_light_switch_, codec->get_spectrum_light());
          break;
        case PKT_SPECTRUM_LIGHT_MODE:
          PUBLISH_STATE(spectrum_light_mode_select_, spectrum_light_mode_options[codec->get_spectrum_light_mode()]);
          break;
        case PKT_STANDBY_DELAY:
          PUBLISH_STATE(standby_delay_number_, codec->get_standby_delay());
          break;
        case PKT_STROBE:
          PUBLISH_STATE(strobe_switch_, codec->get_strobe());
          break;
        case PKT_TDIST_UPT_TSPD_LMODE_VOI_FAN_CHRG_MOTTEMP:
          PUBLISH_STATE(trip_distance_sensor_, codec->get_trip_distance());
          PUBLISH_STATE(uptime_sensor_, codec->get_uptime());
          PUBLISH_STATE(trip_max_speed_sensor_, codec->get_trip_max_speed());
          PUBLISH_STATE(main_light_mode_select_, main_light_mode_options[codec->get_main_light_mode() - 18]);
          PUBLISH_STATE(voice_switch_, codec->get_voice());
          PUBLISH_STATE(fan_binary_sensor_, codec->get_fan());
          PUBLISH_STATE(charging_binary_sensor_, codec->get_charging());
          PUBLISH_STATE(motor_temperature_sensor_, codec->get_motor_temperature());
          break;
        case PKT_VOICE_LANGUAGE:
          PUBLISH_STATE(voice_language_select_, voice_language_options[codec->get_voice_language()]);
          break;
        case PKT_VOL_SPD_ODO_CUR_MOSTEMP_RMODE:
          PUBLISH_STATE(voltage_sensor_, codec->get_voltage());
          PUBLISH_STATE(speed_sensor_, codec->get_speed());
          PUBLISH_STATE(odometer_sensor_, codec->get_odometer());
          PUBLISH_STATE(current_sensor_, codec->get_current());
          PUBLISH_STATE(mosfet_temperature_sensor_, codec->get_mosfet_temperature());
          PUBLISH_STATE(ride_mode_select_, ride_mode_options[codec->get_ride_mode()]);
          PUBLISH_STATE(power_sensor_, codec->get_power());
          break;
        default:
          break;
      }
      // else if (packet_type == PKT_WARNINGS) {
      //   //   auto packet = this->get_codec()->get_packet<KingSongEUCPacketWarnings>();
      //   //   if (packet->error_code != 0) {
      //   //     this->publish_state_(this->error_code_sensor_, packet->error_code);
      //   //     this->publish_state_(this->error_description_text_sensor_, get_error_description(packet->error_code));
      //   //   }
      //   //   this->assert_byte({4, 5, 6, 7}, 0);
      //   //   this->assert_byte(8, 28);
      //   //   this->assert_byte(9, 58);
      //   //   this->assert_byte(10, 0);  // speed warning? 45 → 0
      //   //   this->assert_byte(11, 0);  // flag? 1 → 0 after restarting
      //   //   // this->assert_word(12, 179); // tRdT? 176 → 177 → 178 → 179 → 180
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
      // } else if (packet_type == PKT_MOTLN_GYRO_MOTHOL_CPU_PWM) {
      //   //   auto packet = this->get_codec()->get_packet<KingSongEUCPacketMotorLineGyroMotorHolzerCPUPWM>();
      //   //   this->publish_state_(this->cpu_rate_sensor_, packet->cpu_rate);
      //   //   this->publish_state_(this->motor_hall_sensor_, packet->motor_hall);
      //   //   this->publish_state_(this->motor_phase_line_sensor_, packet->motor_phase_line);
      //   //   this->publish_state_(this->pwm_sensor_, packet->pwm);
      //   //   this->assert_byte({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 0);
      //   //   // this->publish_debug_data_();
      //   // } else if (packet_type == PKT_COLORS) {
      //   //   auto packet = this->get_codec()->get_packet<KingSongEUCPacketColors>();
      // } else if (packet_type == PKT_UNKNOWN1 || packet_type == PKT_UNKNOWN2 || packet_type == PKT_UNKNOWN3) {
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
