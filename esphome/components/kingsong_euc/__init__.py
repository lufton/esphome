import esphome.codegen as cg
from esphome.components import binary_sensor, ble_client, select, sensor, text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@lufton"]
DEPENDENCIES = ["ble_client"]
AUTO_LOAD = ["binary_sensor", "select", "sensor", "text_sensor"]
CONF_KINGSONG_EUC_ID = "kingsong_euc_id"

kingsong_euc_ns = cg.esphome_ns.namespace("kingsong_euc")
KingSongEUC = kingsong_euc_ns.class_(
    "KingSongEUC", ble_client.BLEClientNode, cg.Component
)
KingSongEUCBinarySensor = kingsong_euc_ns.class_(
    "KingSongEUCBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent
)
KingSongEUCSelect = kingsong_euc_ns.class_(
    "KingSongEUCSelect", select.Select, cg.PollingComponent
)
KingSongEUCSensor = kingsong_euc_ns.class_(
    "KingSongEUCSensor", sensor.Sensor, cg.PollingComponent
)
KingSongEUCTextSensor = kingsong_euc_ns.class_(
    "KingSongEUCTextSensor", text_sensor.TextSensor, cg.PollingComponent
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(KingSongEUC),
    }
).extend(ble_client.BLE_CLIENT_SCHEMA)

KINGSONG_EUC_COMPONENT_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KINGSONG_EUC_ID): cv.use_id(KingSongEUC),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
