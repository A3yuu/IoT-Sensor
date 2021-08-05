#include "Save.h"
#include "WPS.h"
#include "Server.h"
#include "Spreadsheet.h"
#include "Sensor.h"
#include <FastLED.h>

const int BUTTON_WPS = 39;
const int LED_PIN = 27;
const int TEMPERATURE_OFFSET = 2;

static CRGB leds[1];
struct SaveData saveData;
unsigned long oldTime;
Measurements measurements;
const int hueRed = 98;

int wpsFlag = 0;

void setup()
{
  setCpuFrequencyMhz(60);
  pinMode(BUTTON_WPS, INPUT);
  //Sensor
  sensorSingleStart(TEMPERATURE_OFFSET);
  //EEPROM
  saveSetup();
  dataGet(saveData);
  //WPS
  WPSSetup();
  //LED
  FastLED.addLeds<WS2812, LED_PIN>(leds, 1);
  FastLED.setBrightness(20);
  //input
  if (!digitalRead(BUTTON_WPS)) {
    Serial.begin(115200);
    wpsFlag = 1;
    //Server
    serverSetup();
    serverSet(String(saveData.uri));
    //LED
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
  }
  //Sensor
  sensorSingleGet(measurements);
  //LED
  float co2LED = constrain((measurements.co2 - 400) / 1600.0, 0, 1);
  if (!wpsFlag) {
    leds[0] = CHSV(hueRed * co2LED, 255, 255);
    FastLED.show();
  }
  //Spreadsheet
  String dataString = String(measurements.co2) + " " + String(measurements.temperature) + " " + String(measurements.humidity);
  spreadsheetPut(saveData.uri, dataString);
  //Sleep
  if (!wpsFlag) {
    esp_sleep_enable_timer_wakeup(600 * 1000000);
    esp_deep_sleep_start();
  }
  //Time
  oldTime =  millis();
}

void loop() {
  unsigned long nowTime = millis();
  //sleep
  if (abs(nowTime - oldTime) > 600 * 1000) {
    esp_sleep_enable_timer_wakeup(1000);
    esp_deep_sleep_start();
  }
  //WPS start
  if (!digitalRead(BUTTON_WPS)) {
    Serial.println("WPS Start");
    WPSStart();
    leds[0] = CRGB(0, 255, 255);
    FastLED.show();
    wpsFlag = 2;
  }
  //WPS success
  if (wpsFlag == 2 && WPSStateGet() == STA_GOT_IP) {
    Serial.println("WPS Success");
    Serial.print("IP:");
    Serial.println(IPGet());
    Serial.print("SSID:");
    Serial.println(SSIDGet());
    Serial.print("PIN:");
    Serial.println(PINGet());
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
    wpsFlag = 1;
  }
  //Server
  if (serverLoop()) {
    Serial.print("URI Update:");
    serverGet().toCharArray(saveData.uri, 256);
    Serial.println(saveData.uri);
    dataSave(saveData);
  }
  delay(10);
}
