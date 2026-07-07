// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <KenwoodXS.h>

const int CTRL_PIN = 3;
const int SDAT_PIN = 2;

KenwoodXS kenwood(CTRL_PIN, SDAT_PIN);

void onCommandSent(byte command) {
    Serial.print("Command sent: ");
    Serial.print(command, DEC);
    Serial.print(" (0x");
    Serial.print(command, HEX);
    Serial.println(")");
}

void printHelp() {
    Serial.println("KenwoodXS basic example");
    Serial.println();
    Serial.println("Commands:");
    Serial.println("  p    Power on");
    Serial.println("  o    Power off");
    Serial.println("  c    Select CD input");
    Serial.println("  t    Select Tape 1 input");
    Serial.println("  v    Select Video 1 input");
    Serial.println("  s    Toggle CD play/pause");
    Serial.println("  r    Start CD");
    Serial.println("  x    Stop CD");
    Serial.println("  a    Play Tape A");
    Serial.println("  z    Stop Tape A");
    Serial.println("  ?    Print this help");
    Serial.println("  1-255 Send raw command byte");
    Serial.println("  999  Try all command bytes with a 1 second delay");
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;
    }

    kenwood.begin();
    kenwood.setDebugOutput(true);
    kenwood.onCommandSentCallback(onCommandSent);

    printHelp();
}

void loop() {
    if (!Serial.available()) {
        return;
    }

    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 1) {
        switch (input.charAt(0)) {
            case 'p':
                kenwood.powerOn();
                break;
            case 'o':
                kenwood.powerOff();
                break;
            case 'c':
                kenwood.selectInput(CD);
                break;
            case 't':
                kenwood.selectInput(TAPE_1);
                break;
            case 'v':
                kenwood.selectInput(VIDEO_1);
                break;
            case 's':
                kenwood.playCD();
                break;
            case 'r':
                kenwood.startCD();
                break;
            case 'x':
                kenwood.stopCD();
                break;
            case 'a':
                kenwood.playTapeA();
                break;
            case 'z':
                kenwood.stopTapeA();
                break;
            case '?':
                printHelp();
                break;
            default:
                Serial.println("Unknown command. Send ? for help.");
                break;
        }
        return;
    }

    long value = input.toInt();
    if (value == 999) {
        Serial.println("Trying all commands with 1 second delay.");
        kenwood.tryAllCommands(1000);
    } else if (value > 0 && value < 256) {
        kenwood.sendCommand((byte)value);
    } else {
        Serial.println("Invalid input. Send ? for help.");
    }
}
