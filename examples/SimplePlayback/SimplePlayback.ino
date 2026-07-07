// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
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
