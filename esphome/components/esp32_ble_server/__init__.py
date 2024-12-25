from esphome import automation
import esphome.codegen as cg
from esphome.components import esp32_ble, esp32_ble_tracker
from esphome.components.esp32 import add_idf_sdkconfig_option
import esphome.config_validation as cv
from esphome.const import (
    CONF_CHARACTERISTIC_UUID,
    CONF_ID,
    CONF_MODEL,
    CONF_SERVICE_UUID,
    CONF_SERVICES,
    CONF_TRIGGER_ID,
    CONF_VALUE,
)
from esphome.core import CORE

AUTO_LOAD = ["esp32_ble"]
CODEOWNERS = ["@jesserockz", "@clydebarrow", "@Rapsssito"]
DEPENDENCIES = ["esp32"]

CONF_ADVERTISE = "advertise"
CONF_BROADCAST = "broadcast"
CONF_CHARACTERISTICS = "characteristics"
CONF_HANDLES = "handles"
CONF_INDICATE = "indicate"
CONF_INSTANCE_ID = "instance_id"
CONF_MANUFACTURER = "manufacturer"
CONF_MANUFACTURER_DATA = "manufacturer_data"
CONF_NOTIFY = "notify"
CONF_ON_WRITE = "on_write"
CONF_PROPERTIES = "properties"
CONF_READ = "read"
CONF_WRITE = "write"
CONF_WRITE_NR = "write_without_response"

esp32_ble_server_ns = cg.esphome_ns.namespace("esp32_ble_server")
BLEServer = esp32_ble_server_ns.class_(
    "BLEServer",
    cg.Component,
    esp32_ble.GATTsEventHandler,
    cg.Parented.template(esp32_ble.ESP32BLE),
)
BLEService = esp32_ble_server_ns.class_("BLEService")
BLECharacteristic = esp32_ble_server_ns.class_("BLECharacteristic")
BLEServiceComponent = esp32_ble_server_ns.class_("BLEServiceComponent")

BLECharacteristicEnum = esp32_ble_server_ns.enum("BLECharacteristic", True)
PROPERTIES = {
    CONF_READ: BLECharacteristicEnum.PROPERTY_READ,
    CONF_WRITE: BLECharacteristicEnum.PROPERTY_WRITE,
    CONF_NOTIFY: BLECharacteristicEnum.PROPERTY_NOTIFY,
    CONF_BROADCAST: BLECharacteristicEnum.PROPERTY_BROADCAST,
    CONF_INDICATE: BLECharacteristicEnum.PROPERTY_INDICATE,
    CONF_WRITE_NR: BLECharacteristicEnum.PROPERTY_WRITE_NR,
}

BLECharacteristicWriteTrigger = esp32_ble_server_ns.class_(
    "BLECharacteristicWriteTrigger",
    automation.Trigger.template(cg.std_vector.template(cg.uint8)),
)
BLECharacteristicSetValueAction = esp32_ble_server_ns.class_(
    "BLECharacteristicSetValueAction", automation.Action
)


def validate_write_property(config):
    properties = config[CONF_PROPERTIES]
    if (
        CONF_WRITE in properties or CONF_WRITE_NR in properties
    ) and CONF_ON_WRITE not in config:
        raise cv.Invalid(
            f"'{CONF_ON_WRITE}' automation should be defined to declare '{CONF_WRITE}' and '{CONF_WRITE_NR}' characteristic property."
        )
    if (
        CONF_WRITE not in properties
        and CONF_WRITE_NR not in properties
        and CONF_ON_WRITE in config
    ):
        raise cv.Invalid(
            f"'{CONF_ON_WRITE}' automation shouldn't be defined if neither '{CONF_WRITE}' nor '{CONF_WRITE_NR}' characteristic property is declared."
        )
    return config


CHARACTERISTIC_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BLECharacteristic),
            cv.Required(CONF_CHARACTERISTIC_UUID): esp32_ble_tracker.bt_uuid,
            cv.Required(CONF_PROPERTIES): cv.ensure_list(
                cv.enum(PROPERTIES, lower=True)
            ),
            cv.Optional(CONF_ON_WRITE): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        BLECharacteristicWriteTrigger
                    ),
                }
            ),
        }
    ),
    validate_write_property,
)

SERVICE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BLEService),
        cv.Optional(CONF_HANDLES, default=0): cv.positive_int,
        cv.Optional(CONF_INSTANCE_ID, default=0): cv.positive_int,
        cv.Required(CONF_SERVICE_UUID): esp32_ble_tracker.bt_uuid,
        cv.Optional(CONF_ADVERTISE, default=False): cv.boolean,
        cv.Required(CONF_CHARACTERISTICS): cv.All(
            cv.ensure_list(CHARACTERISTIC_SCHEMA), cv.Length(min=1)
        ),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BLEServer),
        cv.GenerateID(esp32_ble.CONF_BLE_ID): cv.use_id(esp32_ble.ESP32BLE),
        cv.Optional(CONF_MANUFACTURER, default="ESPHome"): cv.string,
        cv.Optional(CONF_MANUFACTURER_DATA): cv.Schema([cv.hex_uint8_t]),
        cv.Optional(CONF_MODEL): cv.string,
        cv.Optional(CONF_SERVICES): cv.ensure_list(SERVICE_SCHEMA),
    }
).extend(cv.COMPONENT_SCHEMA)

CHARACTERISTIC_OPERATION_BASE_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.use_id(BLECharacteristic),
    }
)


@automation.register_action(
    "ble_characteristic.set_value",
    BLECharacteristicSetValueAction,
    CHARACTERISTIC_OPERATION_BASE_SCHEMA.extend(
        {
            cv.Required(CONF_VALUE): cv.templatable(
                cv.Any(cv.ensure_list(cv.hex_uint8_t), cv.string)
            ),
            cv.Optional(CONF_NOTIFY, default=False): cv.boolean,
        }
    ),
)
async def ble_characteristic_set_value_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren, config[CONF_NOTIFY])
    value = config[CONF_VALUE]
    if cg.is_template(value):
        templ = await cg.templatable(value, args, cg.std_vector.template(cg.uint8))
        cg.add(var.set_value_template(templ))
    elif isinstance(value, list):
        cg.add(var.set_value_vector(value))
    else:
        cg.add(var.set_value_string(value))
    return var


async def characteristics_to_code(var, service, service_config):
    for characteristic_config in service_config[CONF_CHARACTERISTICS]:
        uuid = characteristic_config[CONF_CHARACTERISTIC_UUID]

        properties = 0
        for property in characteristic_config[CONF_PROPERTIES]:
            properties = properties | PROPERTIES.get(property)

        if len(uuid) == len(esp32_ble_tracker.bt_uuid16_format):
            characteristic = cg.Pvariable(
                characteristic_config[CONF_ID],
                var.new_characteristic_uuid16(
                    service, esp32_ble_tracker.as_hex(uuid), properties
                ),
            )
        elif len(uuid) == len(esp32_ble_tracker.bt_uuid32_format):
            characteristic = cg.Pvariable(
                characteristic_config[CONF_ID],
                var.new_characteristic_uuid32(
                    service, esp32_ble_tracker.as_hex(uuid), properties
                ),
            )
        elif len(uuid) == len(esp32_ble_tracker.bt_uuid128_format):
            characteristic = cg.Pvariable(
                characteristic_config[CONF_ID],
                var.new_characteristic_uuid128(
                    service, esp32_ble_tracker.as_reversed_hex_array(uuid), properties
                ),
            )
        else:
            continue

        if (
            CONF_WRITE in characteristic_config[CONF_PROPERTIES]
            or CONF_WRITE_NR in characteristic_config[CONF_PROPERTIES]
        ):
            for action_config in characteristic_config[CONF_ON_WRITE]:
                trigger = cg.new_Pvariable(
                    action_config[CONF_TRIGGER_ID], characteristic
                )
                await automation.build_automation(
                    trigger, [(cg.std_vector.template(cg.uint8), "data")], action_config
                )


async def services_to_code(var, config):
    for service_config in config[CONF_SERVICES]:
        uuid = service_config[CONF_SERVICE_UUID]
        handles = (
            service_config[CONF_HANDLES]
            or 1 + len(service_config[CONF_CHARACTERISTICS]) * 2
        )
        instance_id = service_config[CONF_INSTANCE_ID]
        advertise = service_config[CONF_ADVERTISE]

        if len(uuid) == len(esp32_ble_tracker.bt_uuid16_format):
            service = cg.Pvariable(
                service_config[CONF_ID],
                var.new_service_uuid16(
                    esp32_ble_tracker.as_hex(uuid), advertise, handles, instance_id
                ),
            )
        elif len(uuid) == len(esp32_ble_tracker.bt_uuid32_format):
            service = cg.Pvariable(
                service_config[CONF_ID],
                var.new_service_uuid32(
                    esp32_ble_tracker.as_hex(uuid), advertise, handles, instance_id
                ),
            )
        elif len(uuid) == len(esp32_ble_tracker.bt_uuid128_format):
            service = cg.Pvariable(
                service_config[CONF_ID],
                var.new_service_uuid128(
                    esp32_ble_tracker.as_reversed_hex_array(uuid),
                    advertise,
                    handles,
                    instance_id,
                ),
            )
        else:
            continue

        await characteristics_to_code(var, service, service_config)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)

    parent = await cg.get_variable(config[esp32_ble.CONF_BLE_ID])
    cg.add(parent.register_gatts_event_handler(var))
    cg.add(parent.register_ble_status_event_handler(var))
    cg.add(var.set_parent(parent))

    cg.add(var.set_manufacturer(config[CONF_MANUFACTURER]))
    if CONF_MANUFACTURER_DATA in config:
        cg.add(var.set_manufacturer_data(config[CONF_MANUFACTURER_DATA]))
    if CONF_MODEL in config:
        cg.add(var.set_model(config[CONF_MODEL]))
    await services_to_code(var, config)

    cg.add_define("USE_ESP32_BLE_SERVER")

    if CORE.using_esp_idf:
        add_idf_sdkconfig_option("CONFIG_BT_ENABLED", True)
