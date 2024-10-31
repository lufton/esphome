#pragma once

#include "base.h"

namespace esphome {
namespace kingsong_euc {

static const char *const TAG = "kingsong_euc";
static const uint16_t SERVICE_UUID = 0xFFE0;
static const uint16_t CHARACTERISTIC_UUID = 0xFFE1;
static const KingSongEUCBuffer EMPTY_PACKET = {0x55AA, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
                                               0x00,   0x00, 0x00, 0x00, 0x00, CMD_NONE, 0x14, 0x5A, 0x5A};

}  // namespace kingsong_euc
}  // namespace esphome
