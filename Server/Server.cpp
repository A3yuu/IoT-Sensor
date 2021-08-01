
#include <WiFi.h>

#include "Server.h"


WiFiServer server(80);

String uri_gs = "";

void serverSetup() {
  server.begin();
}
const char decTable[128] = {
  //       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
  /* 0 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 1 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 2 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 3 */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 4 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 5 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 6 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 7 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};
String urlDecode(const char* msg) {
  String decoded = "";
  while (*msg != '\0') {
    if (*msg == '%') {
      char dec1 = decTable[*(msg + 1)];
      char dec2 = decTable[*(msg + 2)];

      if (dec1 != -1 && dec2 != -1) {
        decoded += (char)((dec1 << 4) + dec2);
      }
      msg += 3;
    }
    else {
      decoded += *msg;
      msg++;
    }
  }

  return decoded;
}

void send_response(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print("<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'>");
  client.print("<title>Sensor Settings</title></head>");
  client.print("<style>body{text-align:center;}</style>");
  client.print("<body><h1>Sensor Settings</h1>");
  client.print("<form method='get'> ");
  client.print("Google Spreadsheet URI<br>");
  client.print("<input type='url' name='uri_gs' value='");
  client.print(uri_gs);
  client.print("'style='width: 100%;'><br>");
  client.print("<input type='submit' name='set' value='SET' style='width: 100%;'>");
  client.print("</form> ");
  client.print("</body>");
  client.print("</html>");
  Serial.println( "Server:send responce");
}

bool serverLoop() {
  bool ret = false;
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Server:accessed");
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.print("Server:client:") ;
        Serial.println(line);
        int pos;
        if ((pos = line.indexOf("GET /?uri_gs")) != -1) {
          uri_gs = urlDecode(line.substring(pos + 13, line.length() - 18).c_str());
          Serial.print("Server:uri:");
          Serial.println(uri_gs) ;
          ret = true;
        }
        if (line.length() == 1 && line[0] == '\r') {
          send_response(client) ;
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Server:disconnected");
  }
  return ret;
}

String serverGet() {
  return uri_gs;
}
