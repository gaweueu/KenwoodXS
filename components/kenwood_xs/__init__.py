import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

CODEOWNERS = ["@gaweueu"]

CONF_CTRL_PIN = "ctrl_pin"
CONF_SDAT_PIN = "sdat_pin"

kenwood_xs_ns = cg.esphome_ns.namespace("kenwood_xs")
KenwoodXSComponent = kenwood_xs_ns.class_("KenwoodXSComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(KenwoodXSComponent),
        cv.Required(CONF_CTRL_PIN): cv.int_range(min=0, max=48),
        cv.Required(CONF_SDAT_PIN): cv.int_range(min=0, max=48),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_CTRL_PIN], config[CONF_SDAT_PIN])
    await cg.register_component(var, config)

    cg.add_library("https://github.com/gaweueu/KenwoodXS.git", None)
