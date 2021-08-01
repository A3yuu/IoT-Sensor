#include <WiFi.h>

#include "Spreadsheet.h"

const char* ssid = "LB-LAN";
const char* password = "LittleBear";
String published_url = "https://script.google.com/macros/s/AKfycbxmxmwkK1w1yby30aSWK8KfJN1ZPdCCxJFfSfnSraJ3kB50XBv3JjzWTQvJ9wlUtElr_g/exec";

void setup(){
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
}

void loop(){
  spreadsheetPut(published_url, "5");
   delay(3000);
}
