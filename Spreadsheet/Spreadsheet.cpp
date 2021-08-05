#include <HTTPClient.h>

#include "Spreadsheet.h"

String spreadsheetPut(String uri, String data) {
  String ret = "";
  HTTPClient http;
  http.begin(uri);
  int httpCode = http.POST(data);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      ret = http.getString();
    }
  } else {
    ret = http.errorToString(httpCode);
  }
  http.end();
  return ret;
}
