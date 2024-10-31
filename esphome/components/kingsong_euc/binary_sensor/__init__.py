import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_LIGHT,
    DEVICE_CLASS_BATTERY_CHARGING,
    DEVICE_CLASS_LIGHT,
    DEVICE_CLASS_RUNNING,
    ICON_FAN,
)

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCBinarySensor,
)

CONF_CHARGING = "charging"
CONF_FAN = "fan"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"

BINARY_SENSOR_TYPES = {
    CONF_CHARGING: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon="mdi:battery-charging",
        device_class=DEVICE_CLASS_BATTERY_CHARGING,
    ),
    CONF_FAN: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_FAN,
        device_class=DEVICE_CLASS_RUNNING,
    ),
    CONF_LIGHT: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_CAR_LIGHT_HIGH,
        device_class=DEVICE_CLASS_LIGHT,
    ),
}

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(binary_sensor_type): schema.extend(
            cv.polling_component_schema("10s")
        )
        for binary_sensor_type, schema in BINARY_SENSOR_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_hub = await cg.get_variable(config[CONF_KINGSONG_EUC_ID])

    for binary_sensor_type, _ in BINARY_SENSOR_TYPES.items():
        if conf := config.get(binary_sensor_type):
            binary_sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(
                getattr(kingsong_euc_hub, f"set_{binary_sensor_type}_binary_sensor")(
                    binary_sens
                )
            )
