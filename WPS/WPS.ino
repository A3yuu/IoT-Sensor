#include "WPS.h"

#define BUTTON_WPS  39

int wpsFlag = 1;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_WPS, INPUT);
  delay(10);
  WPSSetup();
}

void loop() {
  if (!digitalRead(BUTTON_WPS)) {
    Serial.println("WPS Start");
    wpsFlag = 2;
    WPSStart();
  }
  if (wpsFlag== 2 && WPSStateGet() == STA_GOT_IP) {
    Serial.println("WPS Success");
    wpsFlag = 1;
  }
  delay(10);
}
