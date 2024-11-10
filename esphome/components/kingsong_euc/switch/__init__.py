import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_FLASH

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCSwitch,
)

CONF_LIFT_SENSOR = "lift_sensor"
CONF_SPECTRUM_LIGHT = "spectrum_light"
CONF_STROBE = "strobe"
ICON_BIKE_FAST = "mdi:bike-fast"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"

SWITCH_TYPES = {
    CONF_LIFT_SENSOR: switch.switch_schema(
        KingSongEUCSwitch,
    ),
    CONF_SPECTRUM_LIGHT: switch.switch_schema(
        KingSongEUCSwitch,
    ),
    CONF_STROBE: switch.switch_schema(
        KingSongEUCSwitch,
        icon=ICON_FLASH,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(switch_type): schema.extend(cv.polling_component_schema("10s"))
        for switch_type, schema in SWITCH_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for switch_type, _ in SWITCH_TYPES.items():
        if conf := config.get(switch_type):
            sw = cg.new_Pvariable(conf[CONF_ID])
            await switch.register_switch(sw, conf)
            await cg.register_component(sw, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{switch_type}_switch")(sw))
