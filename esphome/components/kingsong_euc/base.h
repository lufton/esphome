#pragma once

#include <map>
#include <stdint.h>
#include "esphome/components/ble_client/ble_client.h"

namespace esphome {
namespace kingsong_euc {

enum KingSongEUCCmd : uint8_t {
  CMD_NONE = 0x00,
  CMD_GET_STANDBY_DELAY = 0x3F,        // 63
  CMD_SET_STANDBY_DELAY = 0x3F,        // 63
  CMD_POWER_OFF = 0x40,                // 64
  CMD_SEND_PASS = 0x41,                // 65
  CMD_CLEAR_PASS = 0x42,               // 66
  CMD_SET_PASS = 0x44,                 // 68
  CMD_GET_PASS = 0x45,                 // 69
  CMD_LOCK_ONCE = 0x47,                // 71
  CMD_GET_LOCK_ONCE = 0x48,            // 72
  CMD_GET_VOICE = 0x4A,                // 74
  CMD_SET_MAGIC_LIGHT_MODE = 0x50,     // 80
  CMD_GET_MAGIC_LIGHT_MODE = 0x51,     // 81
  CMD_SET_STROBE = 0x53,               // 83
  CMD_GET_STROBE = 0x54,               // 84
  CMD_SET_MUSIC_BT = 0x56,             // 86
  CMD_GET_MUSIC_BLUETOOTH = 0x57,      // 87
  CMD_LOCK = 0x5D,                     // 93
  CMD_GET_LOCK = 0x5E,                 // 94
  CMD_GET_SERIAL = 0x63,               // 99
  CMD_SET_VOICE_LANGUAGE = 0x69,       // 105
  CMD_GET_VOICE_LANGUAGE = 0x6A,       // 106
  CMD_SET_CIRCLE_LIGHT = 0x6C,         // 108
  CMD_GET_CIRCLE_LIGHT = 0x6D,         // 109
  CMD_SET_MAIN_LIGHT_MODE = 0x73,      // 115
  CMD_BEEP = 0x7C,                     // 124
  CMD_SET_SPECTRUM_LIGHT = 0x7D,       // 125
  CMD_SET_LIFT_SENSOR = 0x7E,          // 126
  CMD_GET_SPECTRUM_LIGHT = 0x80,       // 128
  CMD_GET_LIFT_SENSOR = 0x81,          // 129
  CMD_SET_ALARMS = 0x85,               // 133
  CMD_SET_RIDE_MODE = 0x87,            // 135
  CMD_HORN = 0x88,                     // 136
  CMD_CALIBRATE = 0x89,                // 137
  CMD_GET_CALIBRATE_TILT = 0x8A,       // 138
  CMD_SET_CALIBRATE_TILT = 0x8A,       // 138
  CMD_GET_RIDE_PARAM_A = 0x92,         // 146
  CMD_GET_RIDE_PARAM_B = 0x93,         // 147
  CMD_GET_RIDE_PARAM_C = 0x94,         // 148
  CMD_VOICE_VOLUME = 0x95,             // 149
  CMD_GET_SPECTRUM_LIGHT_MODE = 0x96,  // 150
  CMD_SET_SPECTRUM_LIGHT_MODE = 0x97,  // 151
  CMD_GET_ALARMS = 0x98,               // 152
  CMD_GET_MODEL = 0x9B,                // 155
};

enum KingSongEUCPkt : uint8_t {
  PKT_STANDBY_DELAY = 0x3F,        // 63
  PKT_OLD_MODEL = 0x48,            // 72
  PKT_SPECTRUM_LIGHT = 0x4A,       // 74
  PKT_LIFT_SENSOR = 0x4C,          // 76
  PKT_SPECTRUM_LIGHT_MODE = 0x4D,  // 77
  PKT_MAGIC_LIGHT_MODE = 0x52,     // 82
  PKT_STROBE = 0x55,               // 85
  PKT_MUSIC_BT = 0x58,             // 88
  PKT_COLORS = 0x5C,               // 92
  PKT_LOCK = 0x5F,                 // 95
  PKT_VOICE_LANGUAGE = 0x6B,       // 107
  PKT_CIRCLE_LIGHT = 0x6E,         // 110
  PKT_CALIBRATE_TILT = 0x8A,       // 138
  PKT_A9 = 0xA9,                   // 169
  PKT_RIDE_PARAM_1 = 0xAC,         // 172
  PKT_RIDE_PARAM_2 = 0xAD,         // 173
  PKT_RIDE_PARAM_3 = 0xAE,         // 174
  PKT_FACTORY_RESET = 0xB1,        // 177
  PKT_SERIAL = 0xB3,               // 179
  PKT_ALARMS = 0xB5,               // 181
  PKT_B9 = 0xB9,                   // 185
  PKT_MODEL = 0xBB,                // 187
  PKT_C9 = 0xC9,                   // 201
  PKT_BMS1 = 0xF1,                 // 241
  PKT_BMS2 = 0xF2,                 // 242
  PKT_F3 = 0xF3,                   // 243
  PKT_F4 = 0xF4,                   // 244
  PKT_F5 = 0xF5,                   // 245
  PKT_F6 = 0xF6,                   // 246
};

enum KingSongEUCBMSPkt : uint8_t {
  GENERAL = 0x00,
  TEMP = 0x01,
  CELL_GROUP_1 = 0x02,
  CELL_GROUP_2 = 0x03,
  CELL_GROUP_3 = 0x04,
  CELL_GROUP_4 = 0x05,
  CELL_GROUP_5 = 0x06,
};

struct KingSongEUCCommand {
  uint16_t header = 0x55AA;
  uint8_t data_byte[14] = {0};
  KingSongEUCCmd command = CMD_NONE;
  uint8_t tail[3] = {0x14, 0x5A, 0x5A};
} __attribute__((packed));

struct KingSongEUCPacket {
  uint16_t header;
  uint8_t data_byte[14];
  KingSongEUCPkt packet;
  uint8_t tail[3];
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
  KingSongEUCPkt packet;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCBMSTempPacket {
  uint16_t header;
  uint16_t temperature[7];  // (x-2730)/10.0f
  KingSongEUCPkt packet;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

struct KingSongEUCBMSCellGroupPacket {
  uint16_t header;
  uint16_t cells[7];  // x/1000.0f ((group5[4]-2730)/10.0f = tempMosEnv)
  KingSongEUCPkt packet;
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
  KingSongEUCPkt packet;
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
  KingSongEUCPkt packet;
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
  KingSongEUCPkt packet;
  uint8_t ind17_;
  uint8_t ind18_;
  uint8_t ind19_;
} __attribute__((packed));

}  // namespace kingsong_euc
}  // namespace esphome
