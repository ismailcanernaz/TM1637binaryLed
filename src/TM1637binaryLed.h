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

#ifndef TM1637_BINARY_LED_H
#define TM1637_BINARY_LED_H

#include <Arduino.h>

class TM1637binaryLed {
public:
    TM1637binaryLed(uint8_t clkPin, uint8_t dioPin);

    void begin(uint8_t brightness = 7);
    void showNumber(uint32_t number);
    void clear();

private:
    uint8_t _clkPin;
    uint8_t _dioPin;
    uint8_t _brightness;

    void writeCommand(uint8_t command);
    void startTransmission();
    void stopTransmission();
    void writeByte(uint8_t data);
};

#endif
