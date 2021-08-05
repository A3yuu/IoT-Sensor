
enum WifiState {
  STA_START,
  STA_GOT_IP,
  STA_DISCONNECTED,
  STA_WPS_ER_SUCCESS,
  STA_WPS_ER_FAILED,
  STA_WPS_ER_TIMEOUT,
  STA_WPS_ER_PIN
};
void WPSSetup();
void WPSStart();
int WPSStateGet();
String IPGet();
String SSIDGet();
String PINGet();
