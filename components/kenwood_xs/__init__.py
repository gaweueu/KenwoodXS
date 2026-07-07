import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor
from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

CODEOWNERS = ["@gaweueu"]
AUTO_LOAD = ["sensor", "text_sensor"]

CONF_BIDIRECTIONAL = "bidirectional"
CONF_CTRL_PIN = "ctrl_pin"
CONF_LAST_COMMAND = "last_command"
CONF_LAST_COMMAND_CODE = "last_command_code"
CONF_SDAT_PIN = "sdat_pin"

kenwood_xs_ns = cg.esphome_ns.namespace("kenwood_xs")
KenwoodXSComponent = kenwood_xs_ns.class_("KenwoodXSComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(KenwoodXSComponent),
        cv.Required(CONF_CTRL_PIN): cv.int_range(min=0, max=48),
        cv.Required(CONF_SDAT_PIN): cv.int_range(min=0, max=48),
        cv.Optional(CONF_BIDIRECTIONAL, default=True): cv.boolean,
        cv.Optional(CONF_LAST_COMMAND): text_sensor.text_sensor_schema(
            icon="mdi:remote",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_LAST_COMMAND_CODE): sensor.sensor_schema(
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_CTRL_PIN], config[CONF_SDAT_PIN])
    await cg.register_component(var, config)

    cg.add(var.set_bidirectional(config[CONF_BIDIRECTIONAL]))

    if CONF_LAST_COMMAND in config:
        sens = await text_sensor.new_text_sensor(config[CONF_LAST_COMMAND])
        cg.add(var.set_last_command_text_sensor(sens))

    if CONF_LAST_COMMAND_CODE in config:
        sens = await sensor.new_sensor(config[CONF_LAST_COMMAND_CODE])
        cg.add(var.set_last_command_sensor(sens))

    cg.add_library("https://github.com/gaweueu/KenwoodXS.git", None)
