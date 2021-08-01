
#include <WiFi.h>

#include "Server.h"

const char* ssid = "LB-LAN";
const char* password = "LittleBear";

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  serverSetup();
}

void loop() {
  if (serverLoop()) {
    Serial.println(serverGet());
  }
}
