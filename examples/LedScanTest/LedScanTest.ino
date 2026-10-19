/*
 * LedScanTest
 * Copyright (C) 2026 İsmail Caner Naz
 *
 * LED scanning test example for the TM1637binaryLed library.
 *
 * Scans the physical LEDs of the binary LED matrix one at a time,
 * moving from right to left and from bottom to top.
 *
 * Each LED is illuminated for 100 milliseconds.
 * Nonexistent LED positions are skipped.
 *
 * Hardware:
 * - Arduino Nano or ESP8266
 * - TM1637 controller with a 20-LED binary matrix
 * - CLK: D2
 * - DIO: D3
 *
 * Requires the TM1637binaryLed library.
 *
 * Licensed under the GNU Lesser General Public License v2.1 only.
 * See the LICENSE file in the library repository for details.
 *
 * License: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include <Arduino.h>
#include <TM1637binaryLed.h>

#define CLK_PIN 2
#define DIO_PIN 3

TM1637binaryLed display(CLK_PIN, DIO_PIN);

void setup() {
    display.begin(7);
}

void loop() {
    // Scan columns from right to left.
    for (uint8_t seg = 0; seg < 6; seg++) {

        // Scan each column from bottom to top.
        for (uint8_t grid = 0; grid < 4; grid++) {

            bool ledExists;

            if (grid == 3) {
                // Top row: SEG1, SEG3 and SEG5.
                ledExists = (seg == 0 || seg == 2 || seg == 4);
            } else if (grid == 2) {
                // Third row: SEG1 through SEG5.
                ledExists = (seg <= 4);
            } else {
                // Bottom two rows: all six segments.
                ledExists = true;
            }

            if (!ledExists) {
                continue;
            }

            // Encode one LED using the binary digit display.
            uint32_t testNumber = 0;

            // Set the bit belonging to the selected GRID.
            uint8_t digit = (1U << grid);

            // Place the test digit in the selected SEG position.
            uint32_t placeValue = 1;

            for (uint8_t i = 0; i < seg; i++) {
                placeValue *= 10;
            }

            testNumber = digit * placeValue;

            display.showNumber(testNumber);
            delay(100);

            display.clear();
        }
    }
}
