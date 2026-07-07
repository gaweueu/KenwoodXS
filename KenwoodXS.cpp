// SPDX-License-Identifier: Apache-2.0

#include "KenwoodXS.h"

// Create a new KenwoodXS instance
KenwoodXS::KenwoodXS(int ctrlPin, int sdatPin) : 
    _ctrlPin(ctrlPin), _sdatPin(sdatPin), _mode(KENWOOD_XS_TRANSMIT_ONLY),
    _debugOutput(false), _hasReceivedCommand(false), _lastReceivedCommand(0),
    _rxState(RX_IDLE), _rxCommand(0), _rxBitCount(0), _rxStateStartedAt(0),
    _rxLowStartedAt(0), _onCommandSentCallback(nullptr),
    _onCommandReceivedCallback(nullptr) {
}

// Initialize the KenwoodXS pins
void KenwoodXS::begin(KenwoodXSMode mode) {
    _mode = mode;
    resetReceiveState();

    if (_mode == KENWOOD_XS_BIDIRECTIONAL) {
        releaseBus();
    } else {
        driveIdle();
    }
}

void KenwoodXS::begin(bool bidirectional) {
    begin(bidirectional ? KENWOOD_XS_BIDIRECTIONAL : KENWOOD_XS_TRANSMIT_ONLY);
}

void KenwoodXS::setMode(KenwoodXSMode mode) {
    begin(mode);
}

KenwoodXSMode KenwoodXS::mode() const {
    return _mode;
}

// Send a raw command byte
void KenwoodXS::sendCommand(byte command) {
    if (_debugOutput) {
        Serial.print("Sending KenwoodXS command: ");
        Serial.print(command, DEC);
        Serial.print(" (0x");
        Serial.print(command, HEX);
        Serial.println(")");
    }

    // Set CTRL high to begin transmission
    pinMode(_ctrlPin, OUTPUT);
    pinMode(_sdatPin, OUTPUT);
    digitalWrite(_ctrlPin, HIGH);
    delay(CTRL_SETUP_DELAY);

    // Send the command byte
    sendByte(command);

    // Return to default state
    if (_mode == KENWOOD_XS_BIDIRECTIONAL) {
        releaseBus();
    } else {
        driveIdle();
    }

    // Call callback if set
    if (_onCommandSentCallback) {
        _onCommandSentCallback(command);
    }
}

// Send a command using the enum
void KenwoodXS::sendCommand(KenwoodXSCommand command) {
    sendCommand((byte)command);
}

void KenwoodXS::poll() {
    if (_mode != KENWOOD_XS_BIDIRECTIONAL) {
        return;
    }

    const unsigned long now = micros();
    const int ctrl = digitalRead(_ctrlPin);
    const int sdat = digitalRead(_sdatPin);

    if (_rxState != RX_IDLE && ctrl == LOW) {
        resetReceiveState();
        return;
    }

    switch (_rxState) {
        case RX_IDLE:
            if (ctrl == HIGH) {
                _rxCommand = 0;
                _rxBitCount = 0;
                _rxStateStartedAt = now;
                _rxState = (sdat == HIGH) ? RX_WAIT_START_FALL : RX_WAIT_START_RISE;
            }
            break;

        case RX_WAIT_START_RISE:
            if (sdat == HIGH) {
                _rxStateStartedAt = now;
                _rxState = RX_WAIT_START_FALL;
            } else if (now - _rxStateStartedAt > RX_START_TIMEOUT_US) {
                resetReceiveState();
            }
            break;

        case RX_WAIT_START_FALL:
            if (sdat == LOW) {
                _rxLowStartedAt = now;
                _rxStateStartedAt = now;
                _rxState = RX_WAIT_BIT_RISE;
            } else if (now - _rxStateStartedAt > RX_START_TIMEOUT_US) {
                resetReceiveState();
            }
            break;

        case RX_WAIT_BIT_RISE:
            if (sdat == HIGH) {
                bool bitValue = false;
                if (!classifyPulse(now - _rxLowStartedAt, bitValue)) {
                    resetReceiveState();
                    return;
                }

                _rxCommand = (_rxCommand << 1) | (bitValue ? 1 : 0);
                _rxBitCount++;
                _rxStateStartedAt = now;

                if (_rxBitCount >= 8) {
                    finishReceivedCommand(_rxCommand);
                    _rxState = RX_WAIT_CTRL_LOW;
                } else {
                    _rxState = RX_WAIT_BIT_FALL;
                }
            } else if (now - _rxStateStartedAt > RX_FRAME_TIMEOUT_US) {
                resetReceiveState();
            }
            break;

        case RX_WAIT_BIT_FALL:
            if (sdat == LOW) {
                _rxLowStartedAt = now;
                _rxStateStartedAt = now;
                _rxState = RX_WAIT_BIT_RISE;
            } else if (now - _rxStateStartedAt > RX_FRAME_TIMEOUT_US) {
                resetReceiveState();
            }
            break;

        case RX_WAIT_CTRL_LOW:
            break;
    }
}

bool KenwoodXS::available() {
    poll();
    return _hasReceivedCommand;
}

bool KenwoodXS::readCommand(byte &command) {
    if (!available()) {
        return false;
    }

    command = _lastReceivedCommand;
    _hasReceivedCommand = false;
    return true;
}

bool KenwoodXS::readCommand(byte &command, unsigned long timeoutMs) {
    const unsigned long startedAt = millis();
    while (millis() - startedAt < timeoutMs) {
        if (readCommand(command)) {
            return true;
        }
        yield();
    }
    return readCommand(command);
}

// Power control methods
void KenwoodXS::powerOn() {
    sendCommand(POWER_ON);
}

void KenwoodXS::powerOff() {
    sendCommand(POWER_OFF);
}

// Input selection method
void KenwoodXS::selectInput(KenwoodXSCommand input) {
    switch (input) {
        case TAPE_1:
        case TAPE_2_ON:
        case VIDEO_1:
        case VIDEO_2:
        case PHONO:
        case TUNER:
        case CD:
        case CD_START:
            sendCommand(input);
            break;
        default:
            if (_debugOutput) {
                Serial.println("Invalid input selection command");
            }
            break;
    }
}

// Tape A control methods
void KenwoodXS::playTapeA() {
    sendCommand(TA_PLAY);
}

void KenwoodXS::stopTapeA() {
    sendCommand(TA_STOP);
}

// Tape B control methods
void KenwoodXS::playTapeB() {
    sendCommand(TB_PLAY);
}

void KenwoodXS::stopTapeB() {
    sendCommand(TB_STOP);
}

// CD control methods
void KenwoodXS::playCD() {
    sendCommand(CD_PLAY_PAUSE);
}

void KenwoodXS::stopCD() {
    sendCommand(CD_STOP);
}

void KenwoodXS::startCD() {
    sendCommand(CD_START);
}

// Try all commands (useful for debugging and discovery)
void KenwoodXS::tryAllCommands(unsigned long delayMs) {
    if (_debugOutput) {
        Serial.println("Testing all KenwoodXS commands...");
    }
    
    for (unsigned cmd = 0; cmd < 256; cmd++) {
        // Skip some known uninteresting commands based on original code
        if (cmd == TAPE_1 || cmd == 112 || cmd == 113 || cmd == 115 || 
            cmd == 117 || cmd == 122 || cmd == 123 || cmd == 125) {
            if (_debugOutput) {
                Serial.print("Skipping command ");
                Serial.println(cmd);
            }
            continue;
        }
        
        sendCommand((byte)cmd);
        delay(delayMs);
    }
}

// Set callback for when a command is sent
void KenwoodXS::onCommandSentCallback(OnCommandSentCallback callback) {
    _onCommandSentCallback = callback;
}

void KenwoodXS::onCommandReceivedCallback(OnCommandReceivedCallback callback) {
    _onCommandReceivedCallback = callback;
}

// Enable/disable debug output
void KenwoodXS::setDebugOutput(bool enabled) {
    _debugOutput = enabled;
}

// ----- PRIVATE METHODS -----

void KenwoodXS::driveIdle() {
    pinMode(_ctrlPin, OUTPUT);
    pinMode(_sdatPin, OUTPUT);
    digitalWrite(_ctrlPin, LOW);
    digitalWrite(_sdatPin, LOW);
}

void KenwoodXS::releaseBus() {
    digitalWrite(_ctrlPin, LOW);
    digitalWrite(_sdatPin, LOW);
    pinMode(_ctrlPin, INPUT);
    pinMode(_sdatPin, INPUT);
}

void KenwoodXS::resetReceiveState() {
    _rxState = RX_IDLE;
    _rxCommand = 0;
    _rxBitCount = 0;
    _rxStateStartedAt = 0;
    _rxLowStartedAt = 0;
}

void KenwoodXS::finishReceivedCommand(byte command) {
    _lastReceivedCommand = command;
    _hasReceivedCommand = true;

    if (_debugOutput) {
        Serial.print("Received KenwoodXS command: ");
        Serial.print(command, DEC);
        Serial.print(" (0x");
        Serial.print(command, HEX);
        Serial.println(")");
    }

    if (_onCommandReceivedCallback) {
        _onCommandReceivedCallback(command);
    }
}

bool KenwoodXS::classifyPulse(unsigned long widthUs, bool &bitValue) const {
    if (widthUs < RX_MIN_PULSE_US || widthUs > RX_MAX_PULSE_US) {
        return false;
    }

    bitValue = widthUs < RX_ONE_ZERO_THRESHOLD_US;
    return true;
}

// Send a byte using the Kenwood XS protocol
void KenwoodXS::sendByte(byte b) {
    // Start bit - SDAT high for 15ms
    digitalWrite(_sdatPin, HIGH);
    delay(START_BIT_DELAY);

    // Send 8 data bits, MSB first
    for (byte mask = 0x80; mask; mask >>= 1) {
        // Data bit - SDAT low for either 15ms (bit 0) or 7.5ms (bit 1)
        digitalWrite(_sdatPin, LOW);
        if (b & mask) {
            delayMicroseconds(BIT_1);  // 7.5ms for bit 1
        } else {
            delayMicroseconds(BIT_0);  // 15ms for bit 0
        }

        // Frame signal - SDAT high for 7.5ms
        digitalWrite(_sdatPin, HIGH);
        delayMicroseconds(FRAME_SIGNAL_DELAY);
    }
}
