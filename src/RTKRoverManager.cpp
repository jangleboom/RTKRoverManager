#include <RTKRoverManager.h>

/*
=================================================================================
                                WiFi
=================================================================================
*/

#pragma region: WIFI

bool RTKRoverManager::setupStationMode(const char* ssid, const char* password, const char* deviceName) 
{
  bool success = false;

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    DBG.println("WiFi Failed! Try to decrease the distance to the AP or check your PW!");
    success = false;
  }
  else 
  {
    DBG.print(F("WiFi connected to SSID: "));
    DBG.println(WiFi.SSID());
    DBG.print(F("Wifi client started: "));
    DBG.println(WiFi.getHostname());
    DBG.print(F("IP Address: "));
    DBG.println(WiFi.localIP());
    success = true;

    if (!MDNS.begin(deviceName)) 
    {
      DBG.println("Error starting mDNS, use local IP instead!");
    } 
    else 
    {
      DBG.print(F("Starting mDNS, find me under <http://www."));
      DBG.print(getDeviceName(DEVICE_TYPE));
      DBG.println(F(".local>"));
    }
  }

  return success;
}

bool RTKRoverManager::checkConnectionToWifiStation() 
{ 
  bool isConnectedToStation = false;

  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      DBG.println(F("Reconnecting to access point."));
      isConnectedToStation = WiFi.reconnect();
    } 
    else 
    {
      DBG.println(F("WiFi connected."));
      isConnectedToStation = true;
    }
  }

  return isConnectedToStation;
}

void RTKRoverManager::setupAPMode(const char* apSsid, const char* apPassword) 
{
  DBG.print("Setting soft-AP ... ");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  bool result = WiFi.softAP(apSsid, apPassword);
  DBG.println(result ? "Ready" : "Failed!");
  DBG.print(F("Access point started: "));
  DBG.println(apSsid);
  DBG.print(F("IP address: "));
  DBG.println(WiFi.softAPIP());
}

void RTKRoverManager::setupWiFi(AsyncWebServer* server)
{
  // Check if we have credentials for a available network
  String lastSSID = readFile(SPIFFS, PATH_WIFI_SSID);
  String lastPassword = readFile(SPIFFS, PATH_WIFI_PASSWORD);

  if (lastSSID.isEmpty() || lastPassword.isEmpty() ) 
  {
    setupAPMode(getDeviceName(DEVICE_TYPE).c_str(), AP_PASSWORD);
    delay(500);
  } 
  else
  {
    while (! savedNetworkAvailable(lastSSID)) 
    {
      DBG.print(F("Waiting for HotSpot "));
      DBG.print(lastSSID);
      DBG.println(F(" to appear..."));
      vTaskDelay(1000/portTICK_RATE_MS);
    }
    setupStationMode(lastSSID.c_str(), lastPassword.c_str(), getDeviceName(DEVICE_TYPE).c_str());
    delay(500);
  }

  startServer(server);
}

bool RTKRoverManager::savedNetworkAvailable(const String& ssid) 
{
  if (ssid.isEmpty()) return false;

  uint8_t nNetworks = (uint8_t) WiFi.scanNetworks();
  DBG.print(nNetworks);  
  DBG.println(F(" networks found."));
  for (uint8_t i=0; i<nNetworks; i++) 
  {
    if (ssid.equals(String(WiFi.SSID(i)))) 
    {
      DBG.print(F("A known network with SSID found: ")); 
      DBG.print(WiFi.SSID(i));
      DBG.print(F(" (")); 
      DBG.print(WiFi.RSSI(i)); 
      DBG.println(F(" dB), connecting..."));
      return true;
    }
  }
  return false;
}

#pragma endregion

/*
=================================================================================
                                Web server
=================================================================================
*/
void RTKRoverManager::startServer(AsyncWebServer *server) 
{
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) 
  {
    request->send_P(200, "text/html", INDEX_HTML, processor);
  });

  server->on("/actionUpdateData", HTTP_POST, actionUpdateData);
  server->on("/actionWipeData", HTTP_POST, actionWipeData);
  server->on("/actionRebootESP32", HTTP_POST, actionRebootESP32);

  server->onNotFound(notFound);
  server->begin();
}
  
void RTKRoverManager::notFound(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}

void RTKRoverManager::actionRebootESP32(AsyncWebServerRequest *request) 
{
  DBG.println("ACTION actionRebootESP32!");
  request->send_P(200, "text/html", REBOOT_HTML, RTKRoverManager::processor);
  delay(3000);
  ESP.restart();
}

void RTKRoverManager::actionWipeData(AsyncWebServerRequest *request) 
{
  DBG.println("ACTION actionWipeData!");
  int params = request->params();
  DBG.printf("params: %d\n", params);
  for (int i = 0; i < params; i++) 
  {
    AsyncWebParameter* p = request->getParam(i);
    DBG.printf("%d. POST[%s]: %s\n", i+1, p->name().c_str(), p->value().c_str());
    if (strcmp(p->name().c_str(), "wipe_button") == 0) 
    {
      if (p->value().length() > 0) 
      {
        DBG.printf("wipe command received: %s",p->value().c_str());
        wipeSpiffsFiles();
      } 
     }
    } 

  DBG.print(F("Data in SPIFFS was wiped out!"));
  request->send_P(200, "text/html", INDEX_HTML, processor);
}

void RTKRoverManager::actionUpdateData(AsyncWebServerRequest *request) 
{
  DBG.println("ACTION actionUpdateData!");

  int params = request->params();
  for (int i = 0; i < params; i++) 
  {
    AsyncWebParameter* p = request->getParam(i);
    DBG.printf("%d. POST[%s]: %s\n", i+1, p->name().c_str(), p->value().c_str());

    if (strcmp(p->name().c_str(), PARAM_WIFI_SSID) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_WIFI_SSID, p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_WIFI_PASSWORD) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_WIFI_PASSWORD, p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_HOST) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_RTK_CASTER_HOST, p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_PORT) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_RTK_CASTER_PORT, p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_USER) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_RTK_CASTER_USER, p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_MOINT_POINT) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(SPIFFS, PATH_RTK_MOINT_POINT, p->value().c_str());
      } 
    }

  }
  DBG.println(F("Data saved to SPIFFS!"));
  request->send_P(200, "text/html", INDEX_HTML, RTKRoverManager::processor);
}

// Replaces placeholder with stored values
String RTKRoverManager::processor(const String& var) 
{
  if (var == PARAM_WIFI_SSID) 
  {
    String savedSSID = readFile(SPIFFS, PATH_WIFI_SSID);
    return (savedSSID.isEmpty() ? String(PARAM_WIFI_SSID) : savedSSID);
  }
  else if (var == PARAM_WIFI_PASSWORD) 
  {
    String savedPassword = readFile(SPIFFS, PATH_WIFI_PASSWORD);
    return (savedPassword.isEmpty() ? String(PARAM_WIFI_PASSWORD) : "*******");
  }

  else if (var == PARAM_RTK_CASTER_HOST) 
  {
    String savedCaster = readFile(SPIFFS, PATH_RTK_CASTER_HOST);
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_HOST) : savedCaster);
  }

  else if (var == PARAM_RTK_CASTER_PORT) 
  {
    String savedCaster = readFile(SPIFFS, PATH_RTK_CASTER_PORT);
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_PORT) : savedCaster);
  }

  else if (var == PARAM_RTK_CASTER_USER) 
  {
    String savedCaster = readFile(SPIFFS, PATH_RTK_CASTER_USER);
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_USER) : savedCaster);
  }

  else if (var == PARAM_RTK_MOINT_POINT) 
  {
    String savedCaster = readFile(SPIFFS, PATH_RTK_MOINT_POINT);
    return (savedCaster.isEmpty() ? String(PARAM_RTK_MOINT_POINT) : savedCaster);
  }
 
  else if (var == "next_addr") 
  {
    String savedSSID = readFile(SPIFFS, PATH_WIFI_SSID);
    String savedPW = readFile(SPIFFS, PATH_WIFI_PASSWORD);
    if (savedSSID.isEmpty() || savedPW.isEmpty()) 
    {
      return String(IP_AP);
    } else 
    {
      String clientAddr = getDeviceName(DEVICE_TYPE);
      clientAddr += ".local";
      return clientAddr;
    }
  }
  else if (var == "next_ssid") 
  {
    String savedSSID = readFile(SPIFFS, PATH_WIFI_SSID);
    return (savedSSID.isEmpty() ? getDeviceName(DEVICE_TYPE) : savedSSID);
  }
  return String();
}

/*
=================================================================================
                                SPIFFS
=================================================================================
*/
bool RTKRoverManager::setupSPIFFS(bool formatIfFailed) 
{
  bool success = false;

  #ifdef ESP32
    if (SPIFFS.begin(formatIfFailed)) 
    {
      DBG.println("SPIFFS file system successfully mounted");
      success = true;
    }
  #else
    if (!SPIFFS.begin()) 
    {
      DBG.println("An Error has occurred while mounting SPIFFS");
      success = false;
      return success;
    }
  #endif

  return success;
}

String RTKRoverManager::readFile(fs::FS &fs, const char* path) 
{
  DBG.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");

  if (!file || file.isDirectory()) 
  {
    DBG.println("- empty file or failed to open file");
    return String();
  }
  DBG.println("- read from file:");
  String fileContent;

  while (file.available()) 
  {
    fileContent += String((char)file.read());
  }
  file.close();
  DBG.println(fileContent);

  return fileContent;
}

void RTKRoverManager::writeFile(fs::FS &fs, const char* path, const char* message) 
{
  DBG.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if (!file) 
  {
    DBG.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) 
  {
    DBG.println("- file written");
  } else 
  {
    DBG.println("- write failed");
  }
  file.close();
}
void RTKRoverManager::listFiles() 
{
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while (file) 
  {
      DBG.print("FILE: ");
      DBG.println(file.name());
      file = root.openNextFile();
  }
  file.close();
  root.close();
}

void RTKRoverManager::wipeSpiffsFiles() 
{
  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  DBG.println(F("Wiping: "));

  while (file) 
  {
    DBG.print("FILE: ");
    DBG.println(file.path());
    SPIFFS.remove(file.path());
    file = root.openNextFile();
  }
}

String RTKRoverManager::getDeviceName(const String& prefix) 
  {
    String deviceName((char *)0);
    String suffix = String(getChipId(), HEX);

    unsigned int prefixLen = prefix.length();
    unsigned int suffixLen = suffix.length();
    
    deviceName.reserve(prefixLen + suffixLen);
    deviceName += prefix;
    deviceName += suffix;


    return deviceName;
  }

  uint32_t RTKRoverManager::getChipId() {
    uint32_t chipId = 0;

    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
      DBG.print("chipId: ");
      DBG.println(chipId, HEX);
      return chipId;
  }
