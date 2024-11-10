import esphome.codegen as cg
from esphome.components import ble_client, sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CURRENT,
    CONF_HYSTERESIS,
    CONF_ID,
    CONF_POWER,
    CONF_SPEED,
    CONF_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_DISTANCE,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_SPEED,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_BATTERY,
    ICON_THERMOMETER,
    ICON_TIMER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_KILOMETER,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_PERCENT,
    UNIT_SECOND,
    UNIT_VOLT,
    UNIT_WATT,
)

from .. import (
    CONF_KINGSONG_EUC_ID,
    KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA,
    KingSongEUCSensor,
    kingsong_euc_ns,
)

CONF_CPU_RATE = "cpu_rate"
CONF_ERROR_CODE = "error_code"
CONF_ODOMETER = "odometer"
CONF_MAGIC_LIGHT_MDOE = "magic_light_mode"
CONF_MOSFET_TEMPERATURE = "mosfet_temperature"
CONF_MOTOR_HALL = "motor_hall"
CONF_MOTOR_PHASE_LINE = "motor_phase_line"
CONF_MOTOR_TEMPERATURE = "motor_temperature"
CONF_PWM = "pwm"
CONF_SPECTRUM_LIGHT_MODE = "spectrum_light_mode"
CONF_STANDBY_DELAY = "standby_delay"
CONF_TILT_BACK = "tilt_back"
CONF_TRIP_DISTANCE = "trip_distance"
CONF_TRIP_MAX_SPEED = "trip_max_speed"
CONF_UPTIME = "uptime"
ICON_BATTERY_ARROW_DOWN = "mdi:battery-arrow-down"
ICON_BATTERY_CHECK = "mdi:battery-check"
ICON_BATTERY_SYNC = "mdi:battery-sync"
ICON_CAR_SPEED_LIMITER = "mdi:car-speed-limiter"
ICON_CURRENT_DC = "mdi:current-dc"
ICON_MAP_MARKER_DISTANCE = "mdi:map-marker-distance"
ICON_SPEEDOMETER = "mdi:speedometer"
UNIT_AMPS_HOURS = "Ah"

SensorTypeEnum = kingsong_euc_ns.enum("SensorType", True)

SENSOR_TYPES = {
    # debug
    # "packet_type": sensor.sensor_schema(
    #     KingSongEUCSensor,
    #     entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    # ),
    # dubug
    # CONF_MAGIC_LIGHT_MDOE: sensor.sensor_schema(
    #     KingSongEUCSensor,
    # ),
    # CONF_SPECTRUM_LIGHT_MODE: sensor.sensor_schema(
    #     KingSongEUCSensor,
    # ),
    CONF_CPU_RATE: sensor.sensor_schema(
        KingSongEUCSensor,
    ),
    CONF_ERROR_CODE: sensor.sensor_schema(
        KingSongEUCSensor,
    ),
    CONF_CURRENT: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_DC,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_MOSFET_TEMPERATURE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_MOTOR_HALL: sensor.sensor_schema(
        KingSongEUCSensor,
    ),
    CONF_MOTOR_PHASE_LINE: sensor.sensor_schema(
        KingSongEUCSensor,
    ),
    CONF_MOTOR_TEMPERATURE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_ODOMETER: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER,
        icon=ICON_MAP_MARKER_DISTANCE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    CONF_POWER: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_PWM: sensor.sensor_schema(
        KingSongEUCSensor,
    ),
    CONF_SPEED: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        icon=ICON_SPEEDOMETER,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_SPEED,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # CONF_TILT_BACK: sensor.sensor_schema(
    #     KingSongEUCSensor,
    #     unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
    #     icon=ICON_CAR_SPEED_LIMITER,
    #     accuracy_decimals=0,
    #     device_class=DEVICE_CLASS_SPEED,
    # ),
    CONF_TRIP_DISTANCE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER,
        icon=ICON_MAP_MARKER_DISTANCE,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    CONF_TRIP_MAX_SPEED: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        icon=ICON_SPEEDOMETER,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_SPEED,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    CONF_UPTIME: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_SECOND,
        icon=ICON_TIMER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_VOLTAGE: sensor.sensor_schema(
        KingSongEUCSensor,
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_BATTERY,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}

SENSOR_TYPES.update(
    {
        f"alarm_{num + 1}": sensor.sensor_schema(
            KingSongEUCSensor,
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            icon=ICON_CAR_SPEED_LIMITER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_SPEED,
        )
        for num in range(3)
    }
)

SENSOR_TYPES.update(
    {
        f"byte{num + 2}": sensor.sensor_schema(
            KingSongEUCSensor,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        )
        for num in range(14)
    }
)

SENSOR_TYPES.update(
    {
        f"word{num + 2}_{num + 3}": sensor.sensor_schema(
            KingSongEUCSensor,
            accuracy_decimals=2,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        )
        for num in range(13)
    }
)

SENSOR_TYPES.update(
    {
        f"dword{num + 2}_{num + 5}": sensor.sensor_schema(
            KingSongEUCSensor,
            accuracy_decimals=3,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        )
        for num in range(11)
    }
)

for bms in range(2):
    SENSOR_TYPES.update(
        {
            f"bms_{bms + 1}_current": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_DC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            f"bms_{bms + 1}_factory_capacity": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_AMPS_HOURS,
                icon=ICON_BATTERY_CHECK,
                accuracy_decimals=0,
                state_class=STATE_CLASS_TOTAL,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            f"bms_{bms + 1}_full_cycles": sensor.sensor_schema(
                KingSongEUCSensor,
                icon=ICON_BATTERY_SYNC,
                accuracy_decimals=0,
                state_class=STATE_CLASS_TOTAL_INCREASING,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            f"bms_{bms + 1}_soc": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_BATTERY,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            f"bms_{bms + 1}_remaining_capacity": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_AMPS_HOURS,
                icon=ICON_BATTERY_ARROW_DOWN,
                accuracy_decimals=0,
                state_class=STATE_CLASS_TOTAL,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            f"bms_{bms + 1}_voltage": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_BATTERY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    SENSOR_TYPES.update(
        {
            f"bms_{bms + 1}_cell_{cell + 1}_voltage": sensor.sensor_schema(
                KingSongEUCSensor,
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_BATTERY,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            )
            for cell in range(30)
        }
    )

CONFIG_SCHEMA = KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA.extend(
    {
        cv.Optional(sensor_type): schema.extend({cv.Optional(CONF_HYSTERESIS, 0xFFFFFFFF): cv.positive_not_null_float}).extend(cv.polling_component_schema("10s"))
        for sensor_type, schema in SENSOR_TYPES.items()
    }
)


async def to_code(config):
    kingsong_euc_id = config[CONF_KINGSONG_EUC_ID]
    kingsong_euc_hub = await cg.get_variable(kingsong_euc_id)

    for sensor_type, _ in SENSOR_TYPES.items():
        if conf := config.get(sensor_type):
            sens = cg.new_Pvariable(conf[CONF_ID])
            await sensor.register_sensor(sens, conf)
            await cg.register_component(sens, conf)
            cg.add(getattr(kingsong_euc_hub, f"set_{sensor_type}_sensor")(getattr(SensorTypeEnum, sensor_type.upper()), sens, conf.get(CONF_HYSTERESIS)))
