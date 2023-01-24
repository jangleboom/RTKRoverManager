#include <Arduino.h>
#include <LittleFS.h>
#include <RTKRoverManager.h>
#include <ManagerConfig.h>

#ifdef DEBUGGING
#include <TestsRTKRoverManager.h>
#endif


using namespace RTKRoverManager;
AsyncWebServer server(80);
String scannedSSIDs[MAX_SSIDS];

void setup() 
{ 
  //===============================================================================
  #ifdef DEBUGGING
  Serial.begin(BAUD);
  while (!Serial) {};
  DBG.println(F("Serial setup done."));
  #endif

  //===============================================================================
  // Initialize LittleFS
  // Use board_build.partitions in platformio.ini
  if (!setupLittleFS()) 
  {
    formatLittleFS();
    if (!setupLittleFS()) while (true) {}; // Here you should never hang
  }

  // Uncomment if you want to format (e. g after changing partition sizes)
  // (And dont forget to comment this again after one run ;)
  //formatLittleFS();

  // Uncomment for deleting all data without formatting
  // wipeLittleFSFiles();  
  
  #ifdef DEBUGGING
  listFiles();
  delay(3000);
  #endif
  //===============================================================================
  

  if (!setupWiFi(&server)) 
  {
    DBG.println(F("Wifi setup failed, check your credentials"));
    while (true) {};
  }
}


unsigned long previousMillis = 0;
const unsigned long RECONNECT_INTERVAL = 10000;

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