import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_MODEL, ENTITY_CATEGORY_DIAGNOSTIC

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCTextSensor,
)

CONF_ERROR_DESCRIPTION = "error_description"
ICON_ALERT = "mdi:alert"
ICON_NUMERIC = "mdi:numeric"

TEXT_SENSOR_TYPES = {
    CONF_ERROR_DESCRIPTION: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_ALERT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_MODEL: text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        icon=ICON_NUMERIC,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    "packet_type_hex": text_sensor.text_sensor_schema(
        KingSongEUCTextSensor,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(text_sensor_type): schema.extend(cv.polling_component_schema("10s"))
        for text_sensor_type, schema in TEXT_SENSOR_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_hub = await cg.get_variable(config[CONF_KINGSONG_EUC_ID])

    for text_sensor_type, _ in TEXT_SENSOR_TYPES.items():
        if conf := config.get(text_sensor_type):
            text_sens = await text_sensor.new_text_sensor(conf)
            cg.add(
                getattr(kingsong_euc_hub, f"set_{text_sensor_type}_text_sensor")(
                    text_sens
                )
            )
