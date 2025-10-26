#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define DEBUG
#include "DebugSerial.h"
#include "TimeKeeper.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_I2C_ADDRESS 0x3C
#define ACS712_PIN A3
#define ACS712_MEASURE_TIMES 100
#define ACS712_VREF 5.0
// #define ACS712_SENSITIVITY 0.185
#define ACS712_SENSITIVITY 0.37

#define SENSOR_READ_INTERVAL_MS 100

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
TimeKeeper timeKeeper(SENSOR_READ_INTERVAL_MS);

float offset = 512.0;
float sensorValue = 0.0;

typedef enum {
  VIEW_MODE_GRAPHICS = 0,
  VIEW_MODE_TEXT,
  VIEW_MODE_COUNT,
} ViewMode;

void setup() {
  DEBUG_SERIAL_BEGIN(115200);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Ammeter");

  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) { DEBUG_SERIAL_PRINTLN("Failed to initialize display!"); }
  display.display();
  timeKeeper.begin();

  offset = cariblate();
  DEBUG_SERIAL_PRINTLN("offset: " + String(offset));

  delay(1000);
}

void loop() {
  static ViewMode viewMode = VIEW_MODE_TEXT;
  static bool needRender = true;

  timeKeeper.update();

  if (timeKeeper.isTimeUp()) {
    // DEBUG_SERIAL_PRINTLN("Time to read sensors");
    sensorValue = aquire();
    needRender = true;
  }

  if (needRender) {
    switch (viewMode) {
      case VIEW_MODE_GRAPHICS:
        renderGraphics();
        needRender = false;
        break;

      case VIEW_MODE_TEXT:
        renderText();
        needRender = false;
        break;

      default: break;
    }
  }

  delay(100);
}

double cariblate() {
  double sum = 0;
  for (int i = 0; i < ACS712_MEASURE_TIMES; i++) { sum += analogRead(ACS712_PIN); }
  return sum / ACS712_MEASURE_TIMES;
}

double aquire() {
  double sum = 0;
  for (int i = 0; i < ACS712_MEASURE_TIMES; i++) { sum += analogRead(ACS712_PIN); }

  double avg = sum / ACS712_MEASURE_TIMES;
  double voltage = (avg - offset) * (ACS712_VREF / 1023.0);
  double current = voltage / ACS712_SENSITIVITY;

  if (-0.001 < current && current < 0.001) current = 0.0;
  return current;
}

void renderGraphics() {
  display.clearDisplay();
  display.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, SSD1306_WHITE);
  display.display();
}

void renderText() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(String(sensorValue, 2) + "A");
  display.display();
}
