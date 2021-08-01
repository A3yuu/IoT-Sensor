#include "WPS.h"

void setup() {
  Serial.begin(115200);
  delay(10);
  WPSSetup();
}

void loop() {
  WPSLoop();
  delay(10);
}
