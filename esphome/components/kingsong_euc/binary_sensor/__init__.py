import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
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
CONF_CIRCLE_LIGHT = "circle_light"
CONF_FAN = "fan"
CONF_LIFT_SENSOR = "lift_sensor"
CONF_LOCK = "lock"
CONF_MUSIC_BLUETOOTH = "music_bluetooth"
CONF_OLD_MODEL = "old_model"
CONF_VOICE = "voice"
ICON_CAR_LIGHT_HIGH = "mdi:car-light-high"

BINARY_SENSOR_TYPES = {
    CONF_CHARGING: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon="mdi:battery-charging",
        device_class=DEVICE_CLASS_BATTERY_CHARGING,
    ),
    CONF_CIRCLE_LIGHT: binary_sensor.binary_sensor_schema(
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
    CONF_LIFT_SENSOR: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_CAR_LIGHT_HIGH,
        device_class=DEVICE_CLASS_LIGHT,
    ),
    CONF_LOCK: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_CAR_LIGHT_HIGH,
        device_class=DEVICE_CLASS_LIGHT,
    ),
    CONF_MUSIC_BLUETOOTH: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_CAR_LIGHT_HIGH,
        device_class=DEVICE_CLASS_LIGHT,
    ),
    CONF_OLD_MODEL: binary_sensor.binary_sensor_schema(
        KingSongEUCBinarySensor,
        icon=ICON_CAR_LIGHT_HIGH,
        device_class=DEVICE_CLASS_LIGHT,
    ),
    CONF_VOICE: binary_sensor.binary_sensor_schema(
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
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for binary_sensor_type, _ in BINARY_SENSOR_TYPES.items():
        if conf := config.get(binary_sensor_type):
            binary_sens = cg.new_Pvariable(conf[CONF_ID])
            await binary_sensor.register_binary_sensor(binary_sens, conf)
            await cg.register_component(binary_sens, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{binary_sensor_type}_binary_sensor")(binary_sens))
