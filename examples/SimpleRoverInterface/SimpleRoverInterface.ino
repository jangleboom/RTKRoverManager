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

void setupWifi(void);

void setup() 
{ //===============================================================================
    #ifdef DEBUGGING
    Serial.begin(BAUD);
    while (!Serial) {};
    #endif
  //===============================================================================
  // Init file system
  bool shouldFormat = false;
  if (!setupSPIFFS(shouldFormat))
  {
    DEBUG_SERIAL.println(F("SPIFFS setup failed, Freezing..."));
  }
  while (true) {}; // Freezing
  //===============================================================================
  setupWifi();
}

void loop() 
{
  #ifdef DEBUGGING
  aunit::TestRunner::run();
  #endif

  checkConnectionToWifiStation();
  delay(30000);
}

void setupWifi()
{
  WiFi.setHostname(DEVICE_NAME);
  // Check if we have credentials for a available network
  String lastSSID = readFile(SPIFFS, PATH_WIFI_SSID);
  String lastPassword = readFile(SPIFFS, PATH_WIFI_PASSWORD);

  if (!savedNetworkAvailable(lastSSID) || lastPassword.isEmpty() ) 
  {
    setupAPMode(AP_SSID, AP_PASSWORD);
    delay(500);
  } else 
  {
   setupStationMode(lastSSID.c_str(), lastPassword.c_str(), DEVICE_NAME);
   delay(500);
  }
  startServer(&server);
}