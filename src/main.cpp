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

void setup() {
    #ifdef DEBUGGING
    Serial.begin(BAUD);
    while (!Serial) {};
    #endif

  // Initialize SPIFFS, set true for formatting
  bool format = false;
  if (!RTKRoverManager::setupSPIFFS(format)) {
    DEBUG_SERIAL.println(F("setupSPIFFS failed, freezing"));
    while (true) {};
  }
  
  WiFi.setHostname(DEVICE_NAME);
  // Check if we have credentials for a available network
  String lastSSID = readFile(SPIFFS, PATH_WIFI_SSID);
  String lastPassword = readFile(SPIFFS, PATH_WIFI_PASSWORD);

  if (!savedNetworkAvailable(lastSSID) || lastPassword.isEmpty() ) {
    setupAPMode(AP_SSID, AP_PASSWORD);
    delay(500);
  } else {
   setupStationMode(lastSSID.c_str(), lastPassword.c_str(), DEVICE_NAME);
   delay(500);
 }
  startServer(&server);
}

void loop() {
  #ifdef DEBUGGING
  aunit::TestRunner::run();
  #endif
}