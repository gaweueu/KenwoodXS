// SPDX-License-Identifier: Apache-2.0

#include "KenwoodXS.h"

// Create a new KenwoodXS instance
KenwoodXS::KenwoodXS(int ctrlPin, int sdatPin) : 
    _ctrlPin(ctrlPin), _sdatPin(sdatPin), _debugOutput(false), _onCommandSentCallback(nullptr) {
}

// Initialize the KenwoodXS pins
void KenwoodXS::begin() {
    pinMode(_ctrlPin, OUTPUT);
    pinMode(_sdatPin, OUTPUT);
    digitalWrite(_ctrlPin, LOW);
    digitalWrite(_sdatPin, LOW);
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
    digitalWrite(_ctrlPin, HIGH);
    delay(CTRL_SETUP_DELAY);

    // Send the command byte
    sendByte(command);

    // Return to default state
    digitalWrite(_ctrlPin, LOW);
    digitalWrite(_sdatPin, LOW);

    // Call callback if set
    if (_onCommandSentCallback) {
        _onCommandSentCallback(command);
    }
}

// Send a command using the enum
void KenwoodXS::sendCommand(KenwoodXSCommand command) {
    sendCommand((byte)command);
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

// Enable/disable debug output
void KenwoodXS::setDebugOutput(bool enabled) {
    _debugOutput = enabled;
}

// ----- PRIVATE METHODS -----

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
