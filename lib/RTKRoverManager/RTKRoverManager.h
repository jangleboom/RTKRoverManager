/**
 * @file    RTKRoverManager.h
 * @author  jangleboom
 * @link    https://github.com/audio-communication-group/rwaht_esp_wifi_manager
 * <br>
 * @brief   This is part of a distributed software, here: the web interface to config 
 *          the realtime kinematics base station
 * <br>
 * @todo    - a simular version for the head tracker
 *          - a check for special characters in the form
 *          - a check of the number of decimal places in the input of the geo-coordinates 
 *            with regard to a suitable level of accuracy
 *          - upload html and (separated css and js) to SPIFFS 
 * 
 * @note    FYI: A good tutorial about how to transfer input data from a from and save them to SPIFFS
 *          https://medium.com/@adihendro/html-form-data-input-c942ba23224
 */

#ifndef RTK_ROVER_MANAGER_H
#define RTK_ROVER_MANAGER_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <index_html.h>
#include <error_html.h>
#include <reboot_html.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
#endif

namespace RTKRoverManager {
// DEVICE_NAME can be defined in separate config.h file, if not use this here
#ifndef DEVICE_NAME
#define DEVICE_NAME "rtkbase"
#endif
// WiFi credentials for AP mode
 #define MAX_SSIDS 10 // Space to scan and remember SSIDs
const char AP_SSID[] PROGMEM = "RTK-Base";
const char AP_PASSWORD[] PROGMEM = "12345678";
const char IP_AP[] PROGMEM = "192.168.4.1";
// Parameters for SPIFFS file management
const char PARAM_WIFI_SSID[] PROGMEM = "ssid"; 
const char PARAM_WIFI_PASSWORD[] PROGMEM = "password";
const char PARAM_RTK_LOCATION_METHOD[] PROGMEM = "location_method";
const char PARAM_RTK_SURVEY_ENABLED[] PROGMEM = "survey_enabled";
const char PARAM_RTK_COORDS_ENABLED[] PROGMEM = "coords_enabled";
const char PARAM_RTK_LOCATION_SURVEY_ACCURACY[] PROGMEM = "survey_accuracy";
const char PARAM_RTK_LOCATION_LONGITUDE[] PROGMEM = "longitude";
const char PARAM_RTK_LOCATION_LATITUDE[] PROGMEM = "latitude";
const char PARAM_RTK_LOCATION_HEIGHT[] PROGMEM = "height";
// Paths for SPIFFS file management
const char PATH_WIFI_SSID[] PROGMEM = "/ssid.txt";
const char PATH_WIFI_PASSWORD[] PROGMEM = "/password.txt";
const char PATH_RTK_LOCATION_METHOD[] PROGMEM = "/location_method.txt";
const char PATH_RTK_LOCATION_SURVEY_ACCURACY[] PROGMEM = "/survey_accuracy.txt";
const char PATH_RTK_LOCATION_LONGITUDE[] PROGMEM = "/longitude.txt";
const char PATH_RTK_LOCATION_LATITUDE[] PROGMEM = "/latitude.txt";
const char PATH_RTK_LOCATION_HEIGHT[] PROGMEM = "/height.txt";

/*** Wifi ***/

/**
 * @brief Setup RTK base station in station mode to enter further settings
 * 
 * @param ssid        SSID of the local network
 * @param password    Password of the local network
 * @param deviceName  MDNS name, connect via http://<deviceName>.local
 */
void setupStationMode(const char* ssid, const char* password, const char* deviceName);

/**
 * @brief Setup RTK base station in access point mode to enter local network
 *        credentials and other settings
 * 
 * @param apSsid      SSID of the access point
 * @param apPassword  Password of the access point
 */
void setupAPMode(const char* apSsid, const char* apPassword);

/**
 * @brief Check possibility of connecting with an availbale network.
 * 
 * @param ssid        SSID of saved network in SPIFFS
 * @return true       If the credentials are complete and the network is available.
 * @return false      If the credentials are incomplete or the network is not available.
 */
bool savedNetworkAvailable(const String& ssid);

/*** Web server ***/

/**
 * @brief         Start web server 
 * 
 * @param server  Pointer to global web server object
 */
void startServer(AsyncWebServer *server);

/**
 * @brief Relaces placeholders in HTML code
 * 
 * @param var Placeholder
 * @return String Text to replace the placeholder
 */
String processor(const String& var);

/**
 * @brief Request not found handler
 * 
 * @param request Request
 */
void notFound(AsyncWebServerRequest *request);

/**
 * @brief Action to handle wipe SPIFFS button
 * 
 * @param request Request
 */
void actionWipeData(AsyncWebServerRequest *request);

/**
 * @brief Action to handle Reboot button
 * 
 * @param request Request
 */
void actionRebootESP32(AsyncWebServerRequest *request);

/**
 * @brief Action to handle Save button
 * 
 * @param request Request
 */
void actionUpdateData(AsyncWebServerRequest *request);


/*** SPIFFS ***/

/**
 * @brief         Write data to SPIFFS
 * 
 * @param fs      Address of file system
 * @param path    Path to file
 * @param message Content to save in file on path
 */
void writeFile(fs::FS &fs, const char* path, const char* message);

/**
 * @brief           Read data from SPIFFS
 * 
 * @param fs        Address of file system
 * @param path      Path to file
 * @return String   Content saved in file on path
 */
String readFile(fs::FS &fs, const char* path);

/**
 * @brief List all saved SPIFFS files 
 * 
 */
void listFiles(void);

/**
 * @brief Delete all saved SPIFFS files 
 * 
 */
void wipeSpiffsFiles(void);


}



#endif /*** RTK_ROVER_MANAGER_H ***/