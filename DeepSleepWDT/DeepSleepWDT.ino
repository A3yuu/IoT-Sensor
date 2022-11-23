#include <esp_task_wdt.h>

const int button = 37;
const int wdtTimeout = 3;

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.println(wakeup_reason);

  esp_task_wdt_init(wdtTimeout, true);
  esp_task_wdt_add(NULL);

  Serial.println("setup exit");
  if (digitalRead(button))esp_deep_sleep(1000 * 1000);
}

void loop() {
  Serial.println("running main loop");
  delay(1000);
}
