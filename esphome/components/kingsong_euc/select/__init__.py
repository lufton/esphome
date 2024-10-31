import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_CONFIG

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCSelect,
)

CONF_LIGHT_MODE = "light_mode"
CONF_RIDE_MODE = "ride_mode"
ICON_BIKE_FAST = "mdi:bike-fast"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"

SELECT_TYPES = {
    CONF_LIGHT_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_CAR_LIGHT_HIGH,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    CONF_RIDE_MODE: select.select_schema(
        KingSongEUCSelect,
        icon=ICON_BIKE_FAST,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
}

OPTIONS = {
    CONF_LIGHT_MODE: ["City", "On", "Off", "Auto"],
    CONF_RIDE_MODE: ["Hard", "Medium", "Soft"],
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(select_type): schema.extend(cv.polling_component_schema("10s"))
        for select_type, schema in SELECT_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_hub = await cg.get_variable(config[CONF_KINGSONG_EUC_ID])

    for select_type, _ in SELECT_TYPES.items():
        if (conf := config.get(select_type)) and (options := OPTIONS.get(select_type)):
            sel = await select.new_select(conf, options=options)
            cg.add(getattr(kingsong_euc_hub, f"set_{select_type}_select")(sel))
