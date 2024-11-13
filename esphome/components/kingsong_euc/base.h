#pragma once

#include <map>
#include <stdint.h>
#include "esphome/components/ble_client/ble_client.h"

namespace esphome {
namespace kingsong_euc {

enum KingSongEUCPacketTypes : uint8_t {
  CMD_NONE = 0x00,
  CMD_GET_STANDBY_DELAY = 0x3F,  // 63
  CMD_SET_STANDBY_DELAY = 0x3F,  // 63
  PKT_STANDBY_DELAY = 0x3F,      // 63
  CMD_POWER_OFF = 0x40,          // 64
  CMD_SEND_PASS = 0x41,          // 65
  CMD_CLEAR_PASS = 0x42,         // 66
  CMD_SET_PASS = 0x44,           // 68
  CMD_GET_PASS = 0x45,           // 69
  CMD_LOCK_ONCE = 0x47,          // 71
  CMD_GET_LOCK_ONCE = 0x48,      // 72
  PKT_OLD_MODEL = 0x48,          // 72
  CMD_GET_VOICE = 0x4A,          // 74
  PKT_SPECTRUM_LIGHT = 0x4A,     // 74
  PKT_LIFT_SENSOR = 0x4C,        // 76
  PKT_SPECTRUM_LIGHT_MODE = 0x4D,
  CMD_SET_MAGIC_LIGHT_MODE = 0x50,
  CMD_GET_MAGIC_LIGHT_MODE = 0x51,
  PKT_MAGIC_LIGHT_MODE = 0x52,  // 82
  CMD_SET_STROBE = 0x53,        // 83
  CMD_GET_STROBE = 0x54,
  PKT_STROBE = 0x55,  // 85
  CMD_SET_MUSIC_BT = 0x56,
  CMD_GET_MUSIC_BT = 0x57,
  PKT_MUSIC_BT = 0x58,  // 88
  PKT_COLORS = 0x5C,    // 92
  CMD_LOCK = 0x5D,      // 93
  CMD_GET_LOCK = 0x5E,  // 94
  PKT_LOCK = 0x5F,
  CMD_GET_SERIAL = 0x63,          // 99
  CMD_SET_VOICE_LANGUAGE = 0x69,  // 105
  CMD_GET_VOICE_LANGUAGE = 0x6A,  // 106
  PKT_VOICE_LANGUAGE = 0x6B,      // 107
  CMD_SET_CIRCLE_LIGHT = 0x6C,
  CMD_GET_CIRCLE_LIGHT = 0x6D,
  PKT_CIRCLE_LIGHT = 0x6E,         // 110
  CMD_SET_MAIN_LIGHT_MODE = 0x73,  // 115
  CMD_BEEP = 0x7C,
  CMD_SET_SPECTRUM_LIGHT = 0x7D,
  CMD_SET_LIFT_SENSOR = 0x7E,     // 126
  CMD_GET_SPECTRUM_LIGHT = 0x80,  // 128
  CMD_GET_LIFT_SENSOR = 0x81,     // 129
  CMD_SET_ALARMS = 0x85,
  CMD_SET_RIDE_MODE = 0x87,
  CMD_HORN = 0x88,
  CMD_CALIBRATE = 0x89,
  CMD_GET_CALIBRATE_TILT = 0x8A,
  CMD_SET_CALIBRATE_TILT = 0x8A,
  PKT_CALIBRATE_TILT = 0x8A,  // 138
  CMD_GET_RIDE_PARAM_A = 0x92,
  CMD_GET_RIDE_PARAM_B = 0x93,
  CMD_GET_RIDE_PARAM_C = 0x94,
  CMD_VOICE_VOLUME = 0x95,
  CMD_GET_SPECTRUM_LIGHT_MODE = 0x96,        // 150
  CMD_SET_SPECTRUM_LIGHT_MODE = 0x97,        // 151
  CMD_GET_ALARMS = 0x98,                     // 152
  CMD_GET_MODEL = 0x9B,                      // 155
  PKT_VOL_SPD_ODO_CUR_MOSTEMP_RMODE = 0xA9,  // 169
  PKT_RIDE_PARAM_1 = 0xAC,                   // 172
  PKT_RIDE_PARAM_2 = 0xAD,                   // 173
  PKT_RIDE_PARAM_3 = 0xAE,                   // 174
  PKT_FACTORY_RESET = 0xB1,                  // 177
  PKT_SERIAL = 0xB3,                         // 179
  PKT_ALARMS = 0xB5,
  PKT_TDIST_UPT_TSPD_LMODE_VOI_FAN_CHRG_MOTTEMP = 0xB9,  // 185
  PKT_MODEL = 0xBB,
  PKT_UNKNOWN1 = 0xC9,
  PKT_BMS1 = 0xF1,
  PKT_BMS2 = 0xF2,
  PKT_UNKNOWN2 = 0xF3,
  PKT_UNKNOWN3 = 0xF4,
  PKT_MOTLN_GYRO_MOTHOL_CPU_PWM = 0xF5,  // 245
  PKT_WARNINGS = 0xF6,
};

enum KingSongEUCBMSPacketTypes : uint8_t {
  GENERAL = 0x00,
  TEMP = 0x01,
  CELL_GROUP_1 = 0x02,
  CELL_GROUP_2 = 0x03,
  CELL_GROUP_3 = 0x04,
  CELL_GROUP_4 = 0x05,
  CELL_GROUP_5 = 0x06,
};

struct KingSongEUCBuffer {
  uint16_t header = 0x55AA;
  uint8_t data_byte[14] = {0};
  KingSongEUCPacketTypes packet_type = CMD_NONE;
  uint8_t tail[3] = {0x14, 0x5A, 0x5A};
} __attribute__((packed));

struct KingSongEUCPacketVoltageSpeedOdoCurrentMosTempRideMode {
  uint16_t header;
  uint16_t voltage;             // x/100.0f
  uint16_t speed;               // x/100.0f
  uint16_t odometer_high;       // x/1000.0f
  uint16_t odometer_low;        // x/1000.0f
  int16_t current;              // x/100.0f
  uint16_t mosfet_temperature;  // x/100.0f
  uint8_t ride_mode;            // x in ["HARD","MEDIUM","SOFT"]
  uint8_t ride_mode_flag;       // x = 224 (0xE0)
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketTripDistUptimeTripSpeedLightModeFanChargingMotorTemp {
  uint16_t header;
  uint16_t trip_distance_high;  // x/1000.0f
  uint16_t trip_distance_low;   // x/1000.0f
  uint16_t uptime;              // x
  uint16_t trip_max_speed;      // x/100.0f
  uint8_t main_light_mode;      // x-18 in ["ON","OFF","AUTO"]
  uint8_t voice_status;         // !x in 0/1
  uint8_t fan_status;           // x in 0/1
  uint8_t charging_status;      // x in 0/1
  uint16_t motor_temperature;   // x/100.0f
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketAlarms {
  uint16_t header;
  uint8_t ind02_;      // ?
  uint8_t ind03_;      // ?
  uint16_t alarm_1;    // x
  uint16_t alarm_2;    // x
  uint16_t alarm_3;    // x
  uint16_t tilt_back;  // x
  uint8_t ind12_;      // ?
  uint8_t ind13_;      // ?
  uint8_t ind14_;      // ?
  uint8_t ind15_;      // ?
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCBMSGeneralPacket {
  uint16_t header;
  uint16_t voltage;               // x/100.0f
  uint16_t current;               // x/100.0f
  uint16_t remaining_capacity;    // x*10
  uint16_t factory_capacity;      // x*10
  uint16_t full_cycles;           // x
  uint16_t factory_capacity_too;  // factory capacity too?
  uint8_t ind14_;                 // ?
  uint8_t ind15_;                 // ?
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCBMSTempPacket {
  uint16_t header;
  uint16_t temperature[7];  // (x-2730)/10.0f
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCBMSCellGroupPacket {
  uint16_t header;
  uint16_t cells[7];  // x/1000.0f ((group5[4]-2730)/10.0f = tempMosEnv)
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketWarnings {
  uint16_t header;
  uint16_t curr_speed_limit;  // x/100.0f
  uint8_t ind04_11[8];
  uint16_t total_ride_time;  // x
  uint16_t error_code;       // x in error_codes
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketMotorLineGyroMotorHolzerCPUPWM {
  uint16_t header;
  uint8_t ind02_05[4];       // ?
  uint8_t motor_phase_line;  // x
  uint8_t gyro;              // x
  uint8_t motor_hall;        // x
  uint8_t ind09_13[5];       // looks like some motor stats: key, temp, voltage, current, iic, time, serial, param2,
                             // chargeProtect
  uint8_t cpu_rate;          // ?
  uint8_t pwm;               // ?
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketColors {
  uint16_t header;
  uint8_t color_a_r;
  uint8_t color_a_g;
  uint8_t color_a_b;
  uint8_t color_b_r;
  uint8_t color_b_g;
  uint8_t color_b_b;
  uint8_t color_c_r;
  uint8_t color_c_g;
  uint8_t color_c_b;
  uint8_t color_d_r;
  uint8_t color_d_g;
  uint8_t color_d_b;
  uint8_t ind14_;
  uint8_t ind15_;
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketRideParams {
  uint16_t header;
  uint8_t ind02_;
  uint8_t ind03_;
  uint16_t param_a;
  uint16_t param_b;
  uint16_t param_c;
  uint16_t param_d;
  uint16_t param_e;
  uint16_t param_f;
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

class ISendable : public ble_client::BLEClientNode {
 public:
  virtual ~ISendable() {}

  virtual void send_alarms() = 0;
  virtual void lock() = 0;
  virtual void unlock() = 0;

 protected:
  virtual void send_request_(KingSongEUCBuffer *packet) = 0;
  virtual void send_request_(KingSongEUCPacketTypes type) = 0;
  virtual void send_request_(KingSongEUCPacketTypes type, uint16_t value) = 0;
  virtual void send_request_(KingSongEUCPacketTypes type, uint16_t value, std::map<uint8_t, uint8_t> bytes) = 0;
};

}  // namespace kingsong_euc
}  // namespace esphome
