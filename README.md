# KenwoodXS

KenwoodXS is an Arduino library for controlling compatible Kenwood audio
equipment over the proprietary Kenwood XS System Control bus.

The library can send raw command bytes and includes named helpers for common
receiver, tape deck, and CD player commands discovered from Kenwood
KR-A5030 / RC-5030 and KX-3050 hardware.

## Features

- Sends Kenwood XS command bytes over `CTRL` and `SDAT`.
- Provides named command constants in `KenwoodXSCommand`.
- Includes helpers for power, input selection, CD transport, and Tape A/B
  transport.
- Supports optional Serial debug output.
- Supports an optional callback after each command is sent.
- Includes command tables and timing notes in [manual.md](manual.md).

## Hardware

The Kenwood XS bus uses three connections.

| Signal | Typical Wire | Arduino Direction | Description |
| --- | --- | --- | --- |
| `CTRL` | White | Output | Starts and gates command transmission. |
| `SDAT` | Red | Output | Serial command data. |
| `GND` | Black | Ground | Common ground. |

See [images/XS-Connection.jpg](images/XS-Connection.jpg) for the connector
pinout.

## Installation

### Arduino IDE

1. Download this repository as a ZIP file.
2. In Arduino IDE, select **Sketch > Include Library > Add .ZIP Library...**.
3. Select the downloaded ZIP file.
4. Open **File > Examples > KenwoodXS** and choose an example.

### PlatformIO

Copy this repository into your project's `lib/` directory, or add it as a
Git dependency after publishing:

```ini
lib_deps =
  https://github.com/gaweueu/KenwoodXS.git
```

## Basic Usage

```cpp
#include <KenwoodXS.h>

const int CTRL_PIN = 3;
const int SDAT_PIN = 2;

KenwoodXS kenwood(CTRL_PIN, SDAT_PIN);

void setup() {
    kenwood.begin();

    kenwood.powerOn();
    delay(1000);

    kenwood.selectInput(CD);
    delay(1000);

    kenwood.startCD();
}

void loop() {
}
```

## Examples

| Example | Description |
| --- | --- |
| `Basic` | Interactive Serial monitor command sender. Useful for testing and discovery. |
| `SimplePlayback` | Powers on the receiver, selects CD, and starts playback. |

## API Reference

### Setup

| Method | Description |
| --- | --- |
| `KenwoodXS(int ctrlPin, int sdatPin)` | Creates a controller using the selected Arduino pins. |
| `begin()` | Initializes pins and sets the bus idle state. |
| `setDebugOutput(bool enabled)` | Enables or disables Serial debug output. |
| `onCommandSentCallback(callback)` | Registers a callback called after each command is sent. |

### Sending Commands

| Method | Description |
| --- | --- |
| `sendCommand(byte command)` | Sends a raw command byte. |
| `sendCommand(KenwoodXSCommand command)` | Sends a named command from the enum. |
| `tryAllCommands(unsigned long delayMs = 1000)` | Sends command bytes for discovery and debugging. Use with care. |

### Convenience Helpers

| Method | Sent Command |
| --- | --- |
| `powerOn()` | `POWER_ON` |
| `powerOff()` | `POWER_OFF` |
| `selectInput(CD)` | Selected input command |
| `playTapeA()` | `TA_PLAY` |
| `stopTapeA()` | `TA_STOP` |
| `playTapeB()` | `TB_PLAY` |
| `stopTapeB()` | `TB_STOP` |
| `playCD()` | `CD_PLAY_PAUSE` |
| `startCD()` | `CD_START` |
| `stopCD()` | `CD_STOP` |

## Command Reference

The full command reference lives in [manual.md](manual.md). The most commonly
used values are:

| Command | Decimal Code | Description |
| --- | ---: | --- |
| `POWER_ON` | `181` | Receiver power on |
| `POWER_OFF` | `185` | Receiver power off |
| `CD` | `125` | Select CD input |
| `TAPE_1` | `121` | Select Tape 1 input |
| `VIDEO_1` | `122` | Select Video 1 input |
| `CD_START` | `222` | Start CD playback |
| `CD_PLAY_PAUSE` | `194` | Toggle CD play/pause |
| `CD_STOP` | `198` | Stop CD playback |

## Standalone Repository Layout

This directory is intended to become the root of the standalone repository.

```text
KenwoodXS/
  KenwoodXS.cpp
  KenwoodXS.h
  README.md
  manual.md
  LICENSE
  NOTICE
  library.properties
  library.json
  keywords.txt
  examples/
  images/
```

## License

KenwoodXS is licensed under the Apache License, Version 2.0. See
[LICENSE](LICENSE) and [NOTICE](NOTICE).

## Credits

Protocol timing notes are based on reverse engineering by Olaf Such, originally
posted to `de.sci.electronics` in 1997.

This project was also informed by prior Kenwood XS exploration in
`pyKenwoodXS` by Joseph Madera and saproj.
