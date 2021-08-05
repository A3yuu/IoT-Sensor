#include "Sensor.h"

void setup()
{
  setCpuFrequencyMhz(60);
  Serial.begin(115200);
  while (!Serial);
  sensorSetup();
}

void loop()
{
  Measurements measurements;
  int ret = sensorGet(measurements);
  Serial.print("ret:");
  Serial.println(ret);
  if (ret > 0) {
    Serial.print("co2:");
    Serial.println(measurements.co2);
    Serial.print("tmp:");
    Serial.println(measurements.temperature);
    Serial.print("hum:");
    Serial.println(measurements.humidity);
  }
  delay(30000);
}
