#include <HTTPClient.h>

#include "Spreadsheet.h"

void spreadsheetPut(String uri, String data) {
  Serial.println("Spreadsheet:HTTP");
  HTTPClient http;
  http.begin(uri);
  Serial.println("Spreadsheet:POST");
  int httpCode = http.POST(data);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("Spreadsheet:payload:");
      Serial.println(payload);
    }
  } else {
    Serial.print("Spreadsheet:error:");
    Serial.println(http.errorToString(httpCode));
  }
  http.end();
}
