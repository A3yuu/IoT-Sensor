#include "SensorSGP40.h"
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Wire.begin(25, 21);
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if(wakeup_reason != ESP_SLEEP_WAKEUP_TIMER){
  Serial.print("First Start");
    sensorSetupSGP40();
  }
  int32_t vocIndex;;
  int ret = sensorGetSGP40(50, 25, vocIndex);
  Serial.print("ret:");
  Serial.println(ret);
  if (ret > 0) {
    Serial.print("VOC Index:");
    Serial.println(vocIndex);
  }
  esp_deep_sleep(60*1000 * 1000);
}

void loop()
{
}
