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

void setup() 
{ //===============================================================================
  #ifdef DEBUGGING
  Serial.begin(BAUD);
  while (!Serial) {};
  DBG.println(F("Serial setup done."));
  #endif

  //===============================================================================
  // Init file system
  if (!setupSPIFFS(FORMAT_SPIFFS_IF_FAILED)) while (true) {}; // Freezing
  
  //===============================================================================
  
  setupWifi(&server);
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