/*
 * TM1637binaryLed
 * Copyright (C) 2026 İsmail Caner Naz
 *
 * A binary LED matrix driver using the TM1637 controller.
 *
 * LED matrix layout (front view):
 *
 *       SEG6 SEG5 SEG4 SEG3 SEG2 SEG1
 * GRID4   X    O    X    O    X    O
 * GRID3   X    O    O    O    O    O
 * GRID2   O    O    O    O    O    O
 * GRID1   O    O    O    O    O    O
 *
 * O = LED present
 * X = No LED
 *
 * Columns: SEG1 to SEG6, right to left.
 * Rows:    GRID1 to GRID4, bottom to top.
 * Binary weights: GRID1=1, GRID2=2, GRID3=4, GRID4=8.
 *
 * Licensed under the GNU Lesser General Public License v2.1 only.
 * See the LICENSE file for details.
 *
 * License: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 *
 * Attribution to the original author, İsmail Caner Naz, is requested
 * in derivative works and documentation.
 */

#include <TM1637binaryLed.h>

TM1637binaryLed::TM1637binaryLed(uint8_t clkPin, uint8_t dioPin)
    : _clkPin(clkPin),
      _dioPin(dioPin),
      _brightness(7) {
}

void TM1637binaryLed::begin(uint8_t brightness) {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);

    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);

    if (brightness > 7) {
        brightness = 7;
    }

    _brightness = brightness;

    clear();

    writeCommand(0x88 | _brightness);
}

void TM1637binaryLed::showNumber(uint32_t number) {
    number %= 1000000UL;

    uint8_t digits[6];

    for (int8_t i = 5; i >= 0; --i) {
        digits[i] = number % 10;
        number /= 10;
    }

    uint8_t rowData[4] = {0, 0, 0, 0};

    // SEG1 is the rightmost column; SEG6 is the leftmost.
    // GRID1 to GRID4 have binary weights 1, 2, 4 and 8.
    for (uint8_t seg = 0; seg < 6; ++seg) {
        uint8_t digit = digits[5 - seg];

        for (uint8_t grid = 0; grid < 4; ++grid) {
            bool ledExists;

            if (grid == 3) {
                // The top row contains LEDs only at SEG1, SEG3 and SEG5.
                ledExists = (seg == 0 || seg == 2 || seg == 4);
            } else if (grid == 2) {
                // The third row has no LED at SEG6.
                ledExists = (seg <= 4);
            } else {
                ledExists = true;
            }

            if (ledExists && (digit & (1U << grid))) {
                rowData[grid] |= (1U << seg);
            }
        }
    }

    // Select automatic address increment.
    writeCommand(0x40);

    // Write the four GRID rows to addresses 0xC0 through 0xC3.
    startTransmission();
    writeByte(0xC0);

    for (uint8_t grid = 0; grid < 4; ++grid) {
        writeByte(rowData[grid]);
    }

    stopTransmission();

    // Enable the display and restore the selected brightness.
    writeCommand(0x88 | _brightness);
}

void TM1637binaryLed::clear() {
    writeCommand(0x40);

    startTransmission();
    writeByte(0xC0);

    for (uint8_t grid = 0; grid < 4; ++grid) {
        writeByte(0x00);
    }

    stopTransmission();

    writeCommand(0x88 | _brightness);
}

void TM1637binaryLed::writeCommand(uint8_t command) {
    startTransmission();
    writeByte(command);
    stopTransmission();
}

void TM1637binaryLed::startTransmission() {
    pinMode(_dioPin, OUTPUT);

    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, LOW);
    digitalWrite(_clkPin, LOW);
}

void TM1637binaryLed::stopTransmission() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
}

void TM1637binaryLed::writeByte(uint8_t data) {
    for (uint8_t i = 0; i < 8; ++i) {
        digitalWrite(_clkPin, LOW);
        digitalWrite(_dioPin, data & 0x01);
        data >>= 1;
        digitalWrite(_clkPin, HIGH);
    }

    // Read the acknowledgement bit.
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT_PULLUP);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, OUTPUT);
}
