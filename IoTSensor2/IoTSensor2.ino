#include "Save.h"
#include "WPS.h"
#include "Server.h"
#include "Spreadsheet.h"
#include "Sensor.h"
#include "SensorSGP40.h"
#include <FastLED.h>
#include <esp_task_wdt.h>

const int BUTTON_WPS = 39;
const int LED_PIN = 27;
const float TEMPERATURE_OFFSET = 2.5;
const int hueRed = 98;
const int VOC_SAMPLE_RATE = 5000;//ms
const int CO2_SAMPLE_RATE = 6;//VOC×NUM
const int SERVER_POST_RATE = 120;//VOC×NUM
const int wdtTimeout = 60;

static CRGB leds[1];
RTC_DATA_ATTR SaveData saveData;
unsigned long oldTime;
RTC_DATA_ATTR Measurements measurements = { -1, -1, -1};
RTC_DATA_ATTR int32_t counter = 0;

void setup()
{
  setCpuFrequencyMhz(60);
  esp_task_wdt_init(wdtTimeout, true);
  esp_task_wdt_add(NULL);
  counter++;
  pinMode(BUTTON_WPS, INPUT);
  Wire.begin(25, 21);
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  //First Boot
  if (wakeup_reason != ESP_SLEEP_WAKEUP_TIMER) {
    //EEPROM
    saveSetup();
    dataGet(saveData);
    //SGP40
    sensorSetupSGP40();
    //SCD4x
    sensorSetup(TEMPERATURE_OFFSET);
  }
  //SGP40
  int32_t vocIndex = -1;
  if (measurements.humidity >= 0)sensorGetSGP40(measurements.humidity, measurements.temperature, vocIndex);
  //WPS Mode
  if (!digitalRead(BUTTON_WPS)) {
    Serial.begin(115200);
    //EEPROM
    saveSetup();
    //WPS
    WPSSetup();
    //Server
    serverSetup();
    serverSet(String(saveData.uri));
    //LED
    FastLED.addLeds<WS2812, LED_PIN>(leds, 1);
    FastLED.setBrightness(20);
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
    //Time
    oldTime =  millis();
    delay(3000);
    return;
  }
  //Sleep
  if (counter % CO2_SAMPLE_RATE != 0) {
    esp_deep_sleep(VOC_SAMPLE_RATE * 1000);
  }
  //Time
  oldTime =  millis();
  //WPS
  if (counter % SERVER_POST_RATE == 0) {
    WPSSetup();
  }
  //SCD4x
  sensorGet(measurements);
  if (measurements.co2 >= 0 && vocIndex >= 0) {
    //LED
    float co2LED = constrain((measurements.co2 - 400) / 1600.0, 0, 1);
    FastLED.addLeds<WS2812, LED_PIN>(leds, 1);
    FastLED.setBrightness(20);
    leds[0] = CHSV(hueRed * co2LED, 255, 255);
    FastLED.show();
    //Net
    if (counter % SERVER_POST_RATE == 0) {
      while (counter > SERVER_POST_RATE)counter -= SERVER_POST_RATE;
      for (int i = 0; i < 10; i++) {
        if (WPSStateGet() == STA_GOT_IP) {
          //Spreadsheet
          String dataString = String(measurements.co2) + " " + String(measurements.temperature) + " " + String(measurements.humidity) + " " + String(vocIndex);
          spreadsheetPut(saveData.uri, dataString);
          break;
        }
        delay(100);
      }
    }
  }
  unsigned long pastTime = millis() - oldTime;
  if (pastTime >= VOC_SAMPLE_RATE)pastTime = VOC_SAMPLE_RATE - 1;
  esp_deep_sleep((VOC_SAMPLE_RATE - pastTime) * 1000);
}

bool wpsFlag = false;

void loop() {
  unsigned long nowTime = millis();
  //sleep
  if (abs(nowTime - oldTime) > 600 * 1000) {
    esp_deep_sleep(1000);
  }
  //WPS start
  if (!digitalRead(BUTTON_WPS)) {
    Serial.println("WPS Start");
    WPSStart();
    leds[0] = CRGB(0, 255, 255);
    FastLED.show();
    wpsFlag = true;
  }
  //WPS success
  if (wpsFlag && WPSStateGet() == STA_GOT_IP) {
    Serial.println("WPS Success");
    Serial.print("IP:");
    Serial.println(IPGet());
    Serial.print("SSID:");
    Serial.println(SSIDGet());
    Serial.print("PIN:");
    Serial.println(PINGet());
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
    wpsFlag = false;
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
