# Kenwood XS System Control Manual

This document collects the known timing details and command bytes for the
Kenwood XS System Control bus.

## Wiring

See [XS-Connection.jpg](images/XS-Connection.jpg) for the connector pinout.

| Signal | Description | Notes |
| --- | --- | --- |
| `CTRL` | Control line | Positive logic, TTL level. Goes high before a command frame. |
| `SDAT` | Serial data line | Carries the start bit and 8 data bits. |
| `GND` | Ground | Common ground between the controller and Kenwood device. |

## Protocol Summary

The protocol details below are based on reverse engineering by Olaf Such
from posts to `de.sci.electronics` in 1997.

| Item | Value / Behavior |
| --- | --- |
| Logic level | Positive TTL logic |
| Frame setup | `CTRL` goes high, then data starts about 2 ms later |
| Start bit | `SDAT` high for about 15 ms |
| Data length | 8 bits |
| Bit order | MSB first in this library |
| `0` bit | `SDAT` low for about 15 ms, then high for 7.5 ms |
| `1` bit | `SDAT` low for about 7.5 ms, then high for 7.5 ms |
| Frame end | `CTRL` returns low after all 8 bits are sent |

Olaf's original description, paraphrased:

1. `CTRL` goes high.
2. About 2 ms later, `SDAT` goes high for roughly 15 ms as the start bit.
3. Each bit is encoded as a low pulse followed by a 7.5 ms high frame signal.
4. The low pulse is about 15 ms for `0`, or about 7.5 ms for `1`.
5. After 8 data bits, `CTRL` returns low.

## KX-3050 Cassette Deck Commands

These commands were observed for the Kenwood KX-3050 cassette deck.

### Standby And Power-On Mode

| Decimal Code | Action | Notes |
| ---: | --- | --- |
| `121` | Play | Works in both standby and power-on mode. |
| `112` | Stop | Works in both standby and power-on mode. |
| `113` | Stop | Works in both standby and power-on mode. |
| `115` | Stop | Works in both standby and power-on mode. |
| `117` | Stop | Works in both standby and power-on mode. |
| `122` | Stop | Works in both standby and power-on mode. |
| `123` | Stop | Works in both standby and power-on mode. |
| `125` | Stop | Works in both standby and power-on mode. |

### Power-On Mode Only

| Decimal Code | Action |
| ---: | --- |
| `66` | Search next track |
| `68` | Stop |
| `70` | Play if stopped or paused; repeat current song if playing |
| `72` | Record |
| `74` | Search previous track |
| `76` | Pause |

## KR-A5030 Receiver / RC-5030 Remote Commands

These commands were transmitted from a Kenwood RC-5030 remote control unit
and observed from a Kenwood KR-A5030 audio/video receiver.

### Miscellaneous

| Enum | Decimal Code | Remote Button / Action | Notes |
| --- | ---: | --- | --- |
| `POWER_ON` | `181` | Power on | Same physical remote button as power off. |
| `POWER_OFF` | `185` | Power off | Same physical remote button as power on. |
| `BAND` | `123` | Tuner band | Same code as `TUNER`; behavior may depend on receiver state. |
| `PLUS_10` | `164` | Number pad `+10` | - |

### Input Selection

| Enum | Decimal Code | Input / Action | Notes |
| --- | ---: | --- | --- |
| `VIDEO_1` | `122` | Video 1 | - |
| `VIDEO_2` | `113` | Video 2 | - |
| `TAPE_1` | `121` | Tape 1 | - |
| `TAPE_2_ON` | `20` | Tape 2 on | Toggle on. |
| `TAPE_2_OFF` | `24` | Tape 2 off | Toggle off. |
| `PHONO` | `115` | Phono | - |
| `TUNER` | `123` | Tuner | Same code as `BAND`; behavior may depend on receiver state. |
| `CD` | `125` | CD | - |

### Tape A Transport

| Enum | Decimal Code | Button | Action |
| --- | ---: | --- | --- |
| `TA_FAST_FORWARD` | `67` | `>>` | Fast forward |
| `TA_STOP` | `69` | `[]` | Stop |
| `TA_PLAY` | `71` | `>` | Play |
| `TA_RECORD` | `73` | `*` | Record |
| `TA_REWIND` | `75` | `<<` | Rewind |
| `TA_PAUSE` | `77` | <code>&#124;&#124;</code> | Pause |
| `TA_REWIND_SLOW` | `79` | `<` | Slow rewind, not fully confirmed |

### Tape B Transport

| Enum | Decimal Code | Button | Action |
| --- | ---: | --- | --- |
| `TB_FAST_FORWARD` | `66` | `>>` | Fast forward |
| `TB_STOP` | `68` | `[]` | Stop |
| `TB_PLAY` | `70` | `>` | Play |
| `TB_RECORD` | `72` | `*` | Record |
| `TB_REWIND` | `74` | `<<` | Rewind |
| `TB_PAUSE` | `76` | <code>&#124;&#124;</code> | Pause |
| `TB_REWIND_SLOW` | `78` | `<` | Slow rewind, not fully confirmed |

### CD Controls

| Enum | Decimal Code | Button | Action | Notes |
| --- | ---: | --- | --- | --- |
| `CD_PLAY_PAUSE` | `194` | <code>&gt;/&#124;&#124;</code> | Toggle play / pause | - |
| `CD_STOP` | `198` | `[]` | Stop | - |
| `CD_DISC` | `174` | `DISC` | Change disc | - |
| `CD_START` | `222` | Start CD | Start playback | - |
| `CD_FAST_FORWARD` | `161` | `>>` | Fast forward / scrub | Sends `CD_SCRUB_STOP` on release. |
| `CD_REWIND` | `169` | `<<` | Rewind / scrub | Sends `CD_SCRUB_STOP` on release. |
| `CD_NEXT_TRACK` | `192` | <code>&gt;&gt;&#124;</code> | Next track / scrub | Sends `CD_SCRUB_STOP` on release. |
| `CD_PREV_TRACK` | `200` | <code>&#124;&lt;&lt;</code> | Previous track / scrub | Sends `CD_SCRUB_STOP` on release. |
| `CD_SCRUB_STOP` | `177` | Release | Stop scrubbing | Sent after releasing scrub or track-skip buttons. |

The CD scrub behavior still needs more testing. The current assumption is
that the receiver expects a press-and-hold command for `>>`, `<<`, `>>|`,
or `|<<`, followed by command `177` when the button is released.

## Notes And Caveats

| Topic | Detail |
| --- | --- |
| Duplicate command `123` | Both `BAND` and `TUNER` were observed as code `123`. The receiver may interpret the command differently depending on current mode or context. |
| Unconfirmed slow rewind | Tape A code `79` and Tape B code `78` appear to be slow rewind commands, but this needs confirmation. |
| CD devices | CD scrub behavior is inferred from receiver output and may need validation with a compatible Kenwood CD player. |
