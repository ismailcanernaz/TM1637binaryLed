# TM1637binaryLed

An Arduino library for controlling a custom 20-LED binary matrix driven by the TM1637 controller.

The library displays a six-digit decimal number as binary values across six columns. Each column represents one decimal digit, and each LED row represents a binary weight.

## Features

- Controls a 20-LED matrix using the TM1637 controller.
- Displays six-digit decimal numbers in binary.
- Preserves leading zeros.
- Supports configurable CLK and DIO pins.
- Provides brightness control.
- Includes examples for LED scanning and ESP8266 web-based testing.

## Hardware

- Arduino-compatible board
- TM1637 controller
- Custom 20-LED binary matrix

### LED Matrix Layout

The matrix has four GRID rows and six SEG columns.

```text
       SEG6 SEG5 SEG4 SEG3 SEG2 SEG1
GRID4    X    O    X    O    X    O
GRID3    X    O    O    O    O    O
GRID2    O    O    O    O    O    O
GRID1    O    O    O    O    O    O
```

- `O` = LED present
- `X` = No LED installed

The columns are numbered from right to left, from SEG1 to SEG6. The rows are numbered from bottom to top, from GRID1 to GRID4.

### Binary Weights

| GRID | Binary Weight |
|---|---:|
| GRID1 | 1 |
| GRID2 | 2 |
| GRID3 | 4 |
| GRID4 | 8 |

Each column represents one decimal digit as a binary value. The physical matrix has 20 LEDs because positions without installed LEDs are skipped.

## Installation

### Arduino IDE

1. Download or clone this repository.
2. Copy the `TM1637binaryLed` folder into your Arduino libraries directory.
3. Restart the Arduino IDE.
4. Open one of the examples from the library's **Examples** menu.

## Usage

```cpp
#include <TM1637binaryLed.h>

// Set the CLK and DIO pins.
TM1637binaryLed display(2, 3);

void setup() {
    display.begin(7);
    display.showNumber(123456);
}

void loop() {
}
```

### Constructor

```cpp
TM1637binaryLed display(uint8_t clkPin, uint8_t dioPin);
```

Creates a display object using the specified CLK and DIO pins.

### `begin()`

```cpp
display.begin(7);
```

Initializes the display and sets its brightness. The brightness parameter ranges from `0` to `7`; `7` is the maximum setting.

Calling `begin()` without an argument uses the default brightness of `7`.

### `showNumber()`

```cpp
display.showNumber(123456);
```

Displays a six-digit decimal number as binary values across the six columns.

Leading zeros are preserved. For example:

```cpp
display.showNumber(123);
```

Displays the six-digit value `000123`.

### `clear()`

```cpp
display.clear();
```

Turns off all LEDs.

## Examples

### LedScanTest

Scans the physical LEDs one at a time, moving from right to left and from bottom to top. Each LED is illuminated for 100 milliseconds, and nonexistent LED positions are skipped.

Default example pin assignment:

- CLK: D2
- DIO: D3

### ESPWebTimeTest

An ESP8266 NodeMCU example that hosts a web page for entering a value in `HH:MM:SS` format. The submitted six-digit value is displayed on the binary LED matrix and remains fixed until another value is submitted.

Default example pin assignment:

- CLK: GPIO0
- DIO: GPIO2

The ESP8266 example requires the ESP8266 Arduino core and a Wi-Fi connection.

## Limitations

- The library drives the display; it does not keep time or synchronize with a clock.
- The `HH:MM:SS` format is an example of how six digits can be used, not a built-in timekeeping feature.
- The physical LED layout is specific to the described 20-LED matrix.

## License

This project is licensed under the **GNU Lesser General Public License v2.1 only (LGPL-2.1-only)**.

See the [LICENSE](LICENSE) file for the full license text.

License information: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

## Author

**İsmail Caner Naz**

Copyright (C) 2026 İsmail Caner Naz
