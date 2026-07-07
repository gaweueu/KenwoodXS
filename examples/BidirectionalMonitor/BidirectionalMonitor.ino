// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <KenwoodXS.h>

const int CTRL_PIN = 3;
const int SDAT_PIN = 2;

KenwoodXS kenwood(CTRL_PIN, SDAT_PIN);

void onCommandSent(byte command) {
    Serial.print("Sent command: ");
    Serial.print(command, DEC);
    Serial.print(" (0x");
    Serial.print(command, HEX);
    Serial.println(")");
}

void onCommandReceived(byte command) {
    Serial.print("Received command: ");
    Serial.print(command, DEC);
    Serial.print(" (0x");
    Serial.print(command, HEX);
    Serial.println(")");
}

void printHelp() {
    Serial.println("KenwoodXS bidirectional monitor");
    Serial.println();
    Serial.println("Commands:");
    Serial.println("  p    Power on");
    Serial.println("  o    Power off");
    Serial.println("  c    Select CD input");
    Serial.println("  t    Select Tape 1 input");
    Serial.println("  s    Toggle CD play/pause");
    Serial.println("  x    Stop CD");
    Serial.println("  ?    Print this help");
    Serial.println("  1-255 Send raw command byte");
    Serial.println();
    Serial.println("Commands observed on the Kenwood XS bus are printed automatically.");
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    kenwood.begin(KENWOOD_XS_BIDIRECTIONAL);
    kenwood.setDebugOutput(true);
    kenwood.onCommandSentCallback(onCommandSent);
    kenwood.onCommandReceivedCallback(onCommandReceived);

    printHelp();
}

void loop() {
    kenwood.poll();

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
            case 's':
                kenwood.playCD();
                break;
            case 'x':
                kenwood.stopCD();
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
    if (value > 0 && value < 256) {
        kenwood.sendCommand((byte)value);
    } else {
        Serial.println("Invalid input. Send ? for help.");
    }
}
