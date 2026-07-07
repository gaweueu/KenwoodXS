import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button

from . import KenwoodXSComponent, kenwood_xs_ns

CONF_COMMAND = "command"
CONF_KENWOOD_XS_ID = "kenwood_xs_id"

KenwoodXSButton = kenwood_xs_ns.class_("KenwoodXSButton", button.Button)

COMMANDS = {
    "POWER_ON": 181,
    "POWER_OFF": 185,
    "BAND": 123,
    "PLUS_10": 164,
    "TAPE_2_ON": 20,
    "TAPE_2_OFF": 24,
    "VIDEO_2": 113,
    "PHONO": 115,
    "TAPE_1": 121,
    "VIDEO_1": 122,
    "TUNER": 123,
    "CD": 125,
    "TA_FAST_FORWARD": 67,
    "TA_STOP": 69,
    "TA_PLAY": 71,
    "TA_RECORD": 73,
    "TA_REWIND": 75,
    "TA_PAUSE": 77,
    "TA_REWIND_SLOW": 79,
    "TB_FAST_FORWARD": 66,
    "TB_STOP": 68,
    "TB_PLAY": 70,
    "TB_RECORD": 72,
    "TB_REWIND": 74,
    "TB_PAUSE": 76,
    "TB_REWIND_SLOW": 78,
    "CD_PLAY_PAUSE": 194,
    "CD_STOP": 198,
    "CD_DISC": 174,
    "CD_FAST_FORWARD": 161,
    "CD_REWIND": 169,
    "CD_NEXT_TRACK": 192,
    "CD_PREV_TRACK": 200,
    "CD_SCRUB_STOP": 177,
    "CD_START": 222,
}


def validate_command(value):
    if isinstance(value, str):
        normalized = value.upper()
        if normalized in COMMANDS:
            return COMMANDS[normalized]
    return cv.int_range(min=0, max=255)(value)


CONFIG_SCHEMA = button.button_schema(KenwoodXSButton, icon="mdi:remote").extend(
    {
        cv.GenerateID(CONF_KENWOOD_XS_ID): cv.use_id(KenwoodXSComponent),
        cv.Required(CONF_COMMAND): validate_command,
    }
)


async def to_code(config):
    var = await button.new_button(config)
    parent = await cg.get_variable(config[CONF_KENWOOD_XS_ID])

    cg.add(var.set_parent(parent))
    cg.add(var.set_command(config[CONF_COMMAND]))
