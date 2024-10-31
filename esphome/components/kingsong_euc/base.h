#pragma once

#include <map>
#include <stdint.h>

namespace esphome {
namespace kingsong_euc {

enum KingSongEUCPacketTypes : uint8_t {
  CMD_NONE = 0x00,
  CMD_GET_POWER_OFF = 0x3F,
  CMD_SET_POWER_OFF = 0x3F,
  CMD_POWER_OFF = 0x40,
  CMD_SET_PASS = 0x41,
  CMD_CLEAR_PASS = 0x42,
  CMD_SEND_PASS = 0x44,
  CMD_GET_PASS = 0x45,
  CMD_LOCK_ONCE = 0x47,
  CMD_GET_LOCK_ONCE = 0x48,
  CMD_GET_VOICE = 0x4A,
  CMD_SET_MAGIC_LIGHT = 0x50,
  CMD_GET_MAGIC_LIGHT = 0x51,
  CMD_SET_STROBE = 0x53,
  CMD_GET_STROBE = 0x54,
  CMD_SET_BT_MUSIC = 0x56,
  CMD_GET_BT_MUSIC = 0x57,
  CMD_LOCK = 0x5D,
  CMD_GET_LOCK = 0x5E,
  CMD_GET_SERIAL = 0x63,
  CMD_SET_RIDE_LIGHT = 0x6C,
  CMD_GET_RIDE_LIGHT = 0x6D,
  CMD_SET_LIGHT_MODE = 0x73,
  CMD_SET_VOICE = 0x73,
  CMD_BEEP = 0x7C,
  CMD_SET_SPECTRUM_LIGHT = 0x7D,
  CMD_SET_LIFT_SENSOR = 0x7E,
  CMD_GET_SPECTRUM_LIGHT = 0x80,
  CMD_SET_ALARMS = 0x85,
  CMD_SET_RIDE_MODE = 0x87,
  CMD_HORN = 0x88,
  CMD_CALIBRATE = 0x89,
  CMD_GET_CALIBRATE_TILT = 0x8A,
  CMD_SET_CALIBRATE_TILT = 0x8A,
  CMD_GET_RIDE_PARAM_A = 0x92,
  CMD_GET_RIDE_PARAM_B = 0x93,
  CMD_GET_RIDE_PARAM_C = 0x94,
  CMD_VOICE_VOLUME = 0x95,
  CMD_GET_SPECTRUM_MODE = 0x96,
  CMD_SET_SPECTRUM_MODE = 0x97,
  CMD_GET_ALARMS = 0x98,
  CMD_GET_MODEL = 0x9B,
  PKT_VOL_SPD_ODO_CUR_MOSTEMP_RMODE = 0xA9,
  PKT_ALARMS = 0xB5,
  PKT_TDIST_UPT_TSPD_LMODE_FAN_CHRG_MOTTEMP = 0xB9,
  PKT_MODEL = 0xBB,
  PKT_UNKNOWN1 = 0xC9,
  PKT_BMS1 = 0xF1,
  PKT_BMS2 = 0xF2,
  PKT_BMS3 = 0xF3,
  PKT_BMS4 = 0xF4,
  PKT_MOTLN_GYRO_MOTHOL_CPU_PWM = 0xF5,
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
  uint16_t header;
  uint8_t data_byte[14];
  KingSongEUCPacketTypes packet_type;
  uint8_t tail[3];
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
  uint8_t light_mode;           // x-17 in ["CITY","ON","OFF","AUTO"]
  uint8_t light_status;         // x in 0/1
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
  uint8_t ind02_;     // ?
  uint8_t ind03_;     // ?
  uint8_t alarm_1;    // x
  uint8_t ind05_;     // ?
  uint8_t alarm_2;    // x
  uint8_t ind07_;     // ?
  uint8_t alarm_3;    // x
  uint8_t ind09_;     // ?
  uint8_t tilt_back;  // x
  uint8_t ind11_;     // ?
  uint8_t ind12_;     // ?
  uint8_t ind13_;     // ?
  uint8_t ind14_;     // ?
  uint8_t ind15_;     // ?
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
  uint16_t max_speed;  // x/100.0f
  uint8_t ind04_;
  uint8_t ind05_;
  uint8_t ind06_;
  uint8_t ind07_;
  uint8_t ind08_;
  uint8_t ind09_;
  uint8_t ind10_;
  uint8_t ind11_;
  uint16_t tRdT;        // x
  uint16_t error_code;  // x in error_codes
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCPacketMotorLineGyroMotorHolzerCPUPWM {
  uint16_t header;
  uint8_t ind02_;        // ?
  uint8_t ind03_;        // ?
  uint8_t ind04_;        // x
  uint8_t ind05_;        // ?
  uint8_t motor_line;    // x
  uint8_t gyro;          // x
  uint8_t motor_holzer;  // x
  uint8_t ind09_;        // ?
  uint8_t ind10_;        // x
  uint8_t ind11_;        // ?
  uint8_t ind12_;        // ?
  uint8_t ind13_;        // ?
  uint8_t cpu_rate;      // ?
  uint8_t pwm;           // ?
  KingSongEUCPacketTypes packet_type;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

class ISendable {
 public:
  virtual ~ISendable() {}

  virtual void send_request(KingSongEUCPacketTypes type, uint8_t value) = 0;
  virtual void send_request(KingSongEUCPacketTypes type, uint8_t value, std::map<uint8_t, uint8_t> bytes) = 0;
};

}  // namespace kingsong_euc
}  // namespace esphome
