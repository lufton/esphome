import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import CONF_ID, ENTITY_CATEGORY_CONFIG

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCSelect,
    kingsong_euc_ns,
)

CONF_MAIN_LIGHT_MODE = "main_light_mode"
CONF_RIDE_MODE = "ride_mode"
ICON_BIKE_FAST = "mdi:bike-fast"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"

SelectTypeEnum = kingsong_euc_ns.enum("SelectType", True)

SELECT_TYPES = {
    CONF_MAIN_LIGHT_MODE: (
        select.select_schema(
            KingSongEUCSelect,
            icon=ICON_CAR_LIGHT_HIGH,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        ["On", "Off", "Auto"],
    ),
    CONF_RIDE_MODE: (
        select.select_schema(
            KingSongEUCSelect,
            icon=ICON_BIKE_FAST,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        ["Hard", "Medium", "Soft"]
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(select_type): schema.extend(cv.polling_component_schema("10s"))
        for select_type, (schema, _) in SELECT_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for select_type, (_, options) in SELECT_TYPES.items():
        if conf := config.get(select_type):
            sel = cg.new_Pvariable(conf[CONF_ID])
            await select.register_select(sel, conf, options=options)
            await cg.register_component(sel, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{select_type}_select")(getattr(SelectTypeEnum, select_type.upper()), sel))
