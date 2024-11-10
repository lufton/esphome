#include "kingsong_euc_client.h"

namespace esphome {
namespace kingsong_euc {

  void KingSongEUCClient::beep() {
    this->send_request_(this->get_codec()->get_beep_request());
  }

  void KingSongEUCClient::get_alarms() {
    this->send_request_(this->get_codec()->get_get_alarms_request());
  }

  void KingSongEUCClient::get_lift_sensor() {
    this->send_request_(this->get_codec()->get_get_lift_sensor_request());
  }

  void KingSongEUCClient::get_lock() {
    this->send_request_(this->get_codec()->get_get_lock_request());
  }

  void KingSongEUCClient::get_magic_light() {
    this->send_request_(this->get_codec()->get_get_magic_light_request());
  }

  void KingSongEUCClient::get_model() {
    this->send_request_(this->get_codec()->get_get_model_request());
  }

  void KingSongEUCClient::get_serial() {
    this->send_request_(this->get_codec()->get_get_serial_request());
  }

  void KingSongEUCClient::get_spectrum_light() {
    this->send_request_(this->get_codec()->get_get_spectrum_light_request());
  }

  void KingSongEUCClient::get_standby_delay() {
    this->send_request_(this->get_codec()->get_get_standby_delay_request());
  }

  void KingSongEUCClient::get_strobe() {
    this->send_request_(this->get_codec()->get_get_strobe_request());
  }

  void KingSongEUCClient::horn() {
    this->send_request_(this->get_codec()->get_horn_request());
  }

  KingSongEUCCodec* KingSongEUCClient::get_codec() {
    return codec_.get();
  }

  void KingSongEUCClient::lock() {
    this->send_request_(this->get_codec()->get_lock_request());
  }

  void KingSongEUCClient::power_off() {
    this->send_request_(this->get_codec()->get_power_off_request());
  }

  void KingSongEUCClient::set_alarms(uint8_t alarm_1, uint8_t alarm_2, uint8_t alarm_3, uint8_t tilt_back) {
    // uint8_t alarm_1 = (uint8_t) this->alarm_1_number_->state;
    // uint8_t alarm_2 = (uint8_t) this->alarm_2_number_->state;
    // uint8_t alarm_3 = (uint8_t) this->alarm_3_number_->state;
    // uint8_t tilt_back = (uint8_t) this->tilt_back_number_->state;
    this->send_request_(this->get_codec()->get_set_alarms_request(alarm_1, alarm_2, alarm_3, tilt_back));
  }

  void KingSongEUCClient::set_lift_sensor(bool state) {
    this->send_request_(this->get_codec()->get_set_lift_sensor_request(state));
  }

  void KingSongEUCClient::set_magic_light(bool state) {
    this->send_request_(this->get_codec()->get_set_magic_light_request(state));
  }

  void KingSongEUCClient::set_main_light_mode(uint8_t main_light_mode) {
    this->send_request_(this->get_codec()->get_set_main_light_mode_request(main_light_mode));
  }

  void KingSongEUCClient::set_ride_mode(uint8_t ride_mode) {
    this->send_request_(this->get_codec()->get_set_ride_mode_request(ride_mode));
  }

  void KingSongEUCClient::set_spectrum_light(bool state) {
    this->send_request_(this->get_codec()->get_set_spectrum_light_request(state));
  }

  void KingSongEUCClient::set_standby_delay(uint16_t standby_delay) {
    this->send_request_(this->get_codec()->get_set_standby_delay_request(standby_delay));
  }

  void KingSongEUCClient::set_strobe(bool state) {
    this->send_request_(this->get_codec()->get_set_strobe_request(state));
  }

  void KingSongEUCClient::unlock() {
    this->send_request_(this->get_codec()->get_unlock_request());
  }

  void KingSongEUCClient::send_request_(std::unique_ptr<KingSongEUCBuffer> request_ptr) {
    auto request = request_ptr.get();
    ESP_LOGE(
        TAG, "Request is: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
        request->header & 0xFF, (request->header >> 8) & 0xFF, request->data_byte[0], request->data_byte[1], request->data_byte[2],
        request->data_byte[3], request->data_byte[4], request->data_byte[5], request->data_byte[6], request->data_byte[7],
        request->data_byte[8], request->data_byte[9], request->data_byte[10], request->data_byte[11], request->data_byte[12],
        request->data_byte[13], request->packet_type, request->tail[0], request->tail[1], request->tail[2]);
    ESP_LOGE(TAG, "     Index: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19");
    auto status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                          this->char_handle_, sizeof(KingSongEUCBuffer), (uint8_t *) request,
                                          ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
    if (status)
      ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
  }

}  // namespace kingsong_euc
}  // namespace esphome
