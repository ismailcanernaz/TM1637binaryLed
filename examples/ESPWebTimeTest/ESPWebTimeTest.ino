/*
 * ESPWebTimeTest
 * Copyright (C) 2026 İsmail Caner Naz
 *
 * Web-based test example for the TM1637binaryLed library.
 *
 * Connects an ESP8266 NodeMCU to a Wi-Fi network and
 * hosts a web page for entering a six-digit time value
 * in HH:MM:SS format.
 *
 * The submitted value is displayed on the binary LED matrix.
 * The displayed value remains fixed until a new value is submitted.
 *
 * Hardware:
 * - ESP8266 NodeMCU
 * - TM1637 controller with a 20-LED binary matrix
 * - CLK: GPIO0
 * - DIO: GPIO2
 *
 * Requires the TM1637binaryLed library and ESP8266 Arduino core.
 *
 * Licensed under the GNU Lesser General Public License v2.1 only.
 * See the LICENSE file in the library repository for details.
 *
 * License: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TM1637binaryLed.h>

const char* WIFI_SSID = "mySSID";
const char* WIFI_PASSWORD = "myPASSWORD";

constexpr uint8_t TM_CLK_PIN = 0;
constexpr uint8_t TM_DIO_PIN = 2;

TM1637binaryLed display(TM_CLK_PIN, TM_DIO_PIN);
ESP8266WebServer server(80);

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Binary LED Clock Test</title>
<style>
body {
  font-family: sans-serif;
  max-width: 420px;
  margin: 40px auto;
  padding: 20px;
  background: #f2f2f2;
  color: #222;
}
main {
  background: white;
  padding: 24px;
  border-radius: 12px;
}
h1 { font-size: 1.5rem; }
label { display: block; margin-bottom: 8px; }
input, button {
  box-sizing: border-box;
  width: 100%;
  padding: 12px;
  margin: 8px 0 16px;
  font-size: 1.1rem;
}
button { cursor: pointer; }
#status { min-height: 1.5em; }
</style>
</head>
<body>
<main>
<h1>Binary LED Clock Test</h1>
<form id="clockForm">
<label for="clock">Test value (HH:MM:SS)</label>
<input id="clock" type="time" step="1" value="12:34:56" required>
<button type="submit">Show on LEDs</button>
</form>
<p id="status">Enter a time and press the button.</p>
</main>
<script>
const form = document.getElementById('clockForm');
const clock = document.getElementById('clock');
const status = document.getElementById('status');

form.addEventListener('submit', async (event) => {
  event.preventDefault();

  const parts = clock.value.split(':');
  if (parts.length !== 3) {
    status.textContent = 'Please enter hours, minutes and seconds.';
    return;
  }

  const [hh, mm, ss] = parts.map(Number);

  if (hh > 23 || mm > 59 || ss > 59) {
    status.textContent = 'Invalid time value.';
    return;
  }

  const value = parts.map(p => p.padStart(2, '0')).join('');

  status.textContent = 'Sending ' + value + '...';

  try {
    const response = await fetch('/show?value=' + value);
    const result = await response.text();
    status.textContent = response.ok ? result : 'Error: ' + result;
  } catch (error) {
    status.textContent = 'Connection failed.';
  }
});
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", PAGE);
}

void handleShow() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing time value.");
    return;
  }

  String value = server.arg("value");

  if (value.length() != 6) {
    server.send(400, "text/plain", "Expected six digits.");
    return;
  }

  for (uint8_t i = 0; i < 6; i++) {
    if (value[i] < '0' || value[i] > '9') {
      server.send(400, "text/plain", "Digits only.");
      return;
    }
  }

  uint8_t hours = value.substring(0, 2).toInt();
  uint8_t minutes = value.substring(2, 4).toInt();
  uint8_t seconds = value.substring(4, 6).toInt();

  if (hours > 23 || minutes > 59 || seconds > 59) {
    server.send(400, "text/plain", "Time out of range.");
    return;
  }

  uint32_t number = value.toInt();
  display.showNumber(number);

  server.send(200, "text/plain", "Displayed: " + value);
}

void setup() {
  Serial.begin(115200);

  display.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/show", HTTP_GET, handleShow);
  server.begin();

  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();
}
