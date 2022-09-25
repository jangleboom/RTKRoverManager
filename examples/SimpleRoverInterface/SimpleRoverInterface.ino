#include <Arduino.h>
#include <SPIFFS.h>
#include <RTKRoverManager.h>
#include <ManagerConfig.h>

#ifdef DEBUGGING
#include <TestsRTKRoverManager.h>
#endif


using namespace RTKRoverManager;
AsyncWebServer server(80);
String scannedSSIDs[MAX_SSIDS];
/**
 * @brief Setup WiFi: Access point on first run or if no credentials saved in SPIFFS,
 *                    Station if it can connect to an AP successfully.
 * 
 */
void setupWifi(void);

void setup() 
{ //===============================================================================
  #ifdef DEBUGGING
  Serial.begin(BAUD);
  while (!Serial) {};
  DEBUG_SERIAL.println(F("Serial setup done."));
  #endif

  //===============================================================================
  // Init file system
  if (!setupSPIFFS(FORMAT_SPIFFS_IF_FAILED)) while (true) {}; // Freezing
  
  //===============================================================================
  
  setupWifi();
}


unsigned long previousMillis = 0;
const unsigned long RECONNECT_INTERVAL = 30000;

void loop() 
{
  #ifdef DEBUGGING
  aunit::TestRunner::run();
  #endif

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every RECONNECT_INTERVAL seconds
  if (currentMillis - previousMillis > RECONNECT_INTERVAL) 
  {
    RTKRoverManager::checkConnectionToWifiStation();
    previousMillis = currentMillis;
  }
}

void setupWifi()
{
  const char* deviceName = getDeviceName(DEVICE_TYPE).c_str();
  WiFi.setHostname(deviceName);

  // Check if we have credentials for a available network
  String lastSSID = readFile(SPIFFS, PATH_WIFI_SSID);
  String lastPassword = readFile(SPIFFS, PATH_WIFI_PASSWORD);

  if (!savedNetworkAvailable(lastSSID) || lastPassword.isEmpty() ) 
  {
    setupAPMode(deviceName, AP_PASSWORD);
    delay(500);
  } else 
  {
   setupStationMode(lastSSID.c_str(), lastPassword.c_str(), getDeviceName(DEVICE_TYPE).c_str());
   delay(500);
  }
  startServer(&server);
}
