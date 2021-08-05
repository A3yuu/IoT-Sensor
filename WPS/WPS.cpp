#include <WiFi.h>
#include <esp_wps.h>

#include "WPS.h"

static esp_wps_config_t config;

void wpsInitConfig() {
  config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
  config.wps_type = WPS_TYPE_PBC;
  strcpy(config.factory_info.manufacturer, "A3 System");
  strcpy(config.factory_info.model_number, "M5Atom SCD4x");
  strcpy(config.factory_info.model_name, "Env Sensor");
  strcpy(config.factory_info.device_name, "ESP STATION");
}

String wpspin2string(uint8_t a[]) {
  char wps_pin[9];
  for (int i = 0; i < 8; i++) {
    wps_pin[i] = a[i];
  }
  wps_pin[8] = '\0';
  return (String)wps_pin;
}

int wifiState = -1;
String IP;
String SSID;
String PIN;

int WPSStateGet() {
  return wifiState;
}
String IPGet() {
  return IP;
}
String SSIDGet() {
  return SSID;
}
String PINGet() {
  return PIN;
}

void WiFiEvent(WiFiEvent_t event, system_event_info_t info) {
  switch (event) {
    case SYSTEM_EVENT_STA_START:
      wifiState = STA_START;
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      SSID = WiFi.SSID();
      IP = (String)WiFi.localIP();
      wifiState = STA_GOT_IP;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      wifiState = STA_DISCONNECTED;
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      wifiState =  STA_WPS_ER_SUCCESS;
      SSID = WiFi.SSID();
      esp_wifi_wps_disable();
      delay(10);
      WiFi.begin();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      wifiState =  STA_WPS_ER_FAILED;
      esp_wifi_wps_disable();
      esp_wifi_wps_enable(&config);
      esp_wifi_wps_start(0);
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      wifiState =  STA_WPS_ER_TIMEOUT;
      esp_wifi_wps_disable();
      esp_wifi_wps_enable(&config);
      esp_wifi_wps_start(0);
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      wifiState =  STA_WPS_ER_PIN;
      PIN = wpspin2string(info.sta_er_pin.pin_code);
      break;
    default:
      break;
  }
}

void WPSSetup() {
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();
}
void WPSStart() {
  WiFi.disconnect();
  wpsInitConfig();
  esp_wifi_wps_disable();
  esp_wifi_wps_enable(&config);
  esp_wifi_wps_start(0);
}
