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
    DBG.print(F("Station IP Address: "));
    DBG.println(WiFi.localIP());

    success = true;
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
  DBG.print(F("AP IP address: "));
  DBG.println(WiFi.softAPIP());
  DBG.print(F("AP Password: "));
  DBG.println(AP_PASSWORD);

}

bool RTKRoverManager::setupWiFi(AsyncWebServer* server)
{
  bool success = false;

  String deviceName = getDeviceName(DEVICE_TYPE);

  WiFi.softAPdisconnect(true); // AP  sollte noch verbunden sein
  WiFi.disconnect(true);       // STA sollte noch verbunden sein
  WiFi.setHostname(deviceName.c_str());
  // Check if we have credentials for a available network
  String lastSSID = readFile(LittleFS, getPath(PARAM_WIFI_SSID).c_str());
  String lastPassword = readFile(LittleFS, getPath(PARAM_WIFI_PASSWORD).c_str());

  if (lastSSID.isEmpty() || lastPassword.isEmpty() ) 
  {
    setupAPMode(deviceName.c_str(), AP_PASSWORD);
    delay(500);
    startServer(server);
    delay(500);
    success = true;
  } 
  else
  {
    if ( !savedNetworkAvailable(lastSSID) ) 
    {
      DBG.print(F("HotSpot '"));
      DBG.print(lastSSID);
      DBG.println(F("' not found."));
      success = false;
    }
    else 
    {
      setupStationMode(lastSSID.c_str(), lastPassword.c_str(), deviceName.c_str());
      success = true;
    }
  }

  return success;
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
        wipeLittleFSFiles();
      } 
    }
  } 

  DBG.print(F("Data in LittleFS was wiped out!"));
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
        writeFile(LittleFS, getPath(PARAM_WIFI_SSID).c_str(), p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_WIFI_PASSWORD) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(LittleFS, getPath(PARAM_WIFI_PASSWORD).c_str(), p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_HOST) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(LittleFS, getPath(PARAM_RTK_CASTER_HOST).c_str(), p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_PORT) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(LittleFS, getPath(PARAM_RTK_CASTER_PORT).c_str(), p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_CASTER_USER) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(LittleFS, getPath(PARAM_RTK_CASTER_USER).c_str(), p->value().c_str());
      } 
    }

    if (strcmp(p->name().c_str(), PARAM_RTK_MOINT_POINT) == 0) 
    {
      if (p->value().length() > 0) 
      {
        writeFile(LittleFS, getPath(PARAM_RTK_MOINT_POINT).c_str(), p->value().c_str());
      } 
    }

  }
  DBG.println(F("Data saved to LittleFS!"));
  request->send_P(200, "text/html", INDEX_HTML, RTKRoverManager::processor);
}

// Replaces placeholder with stored values
String RTKRoverManager::processor(const String& var) 
{
  if (var == PARAM_WIFI_SSID) 
  {
    String savedSSID = readFile(LittleFS, getPath(PARAM_WIFI_SSID).c_str());
    return (savedSSID.isEmpty() ? String(PARAM_WIFI_SSID) : savedSSID);
  }
  else if (var == PARAM_WIFI_PASSWORD) 
  {
    String savedPassword = readFile(LittleFS, getPath(PARAM_WIFI_PASSWORD).c_str());
    return (savedPassword.isEmpty() ? String(PARAM_WIFI_PASSWORD) : "*******");
  }

  else if (var == PARAM_RTK_CASTER_HOST) 
  {
    String savedCaster = readFile(LittleFS, getPath(PARAM_RTK_CASTER_HOST).c_str());
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_HOST) : savedCaster);
  }

  else if (var == PARAM_RTK_CASTER_PORT) 
  {
    String savedCaster = readFile(LittleFS, getPath(PARAM_RTK_CASTER_PORT).c_str());
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_PORT) : savedCaster);
  }

  else if (var == PARAM_RTK_CASTER_USER) 
  {
    String savedCaster = readFile(LittleFS, getPath(PARAM_RTK_CASTER_USER).c_str());
    return (savedCaster.isEmpty() ? String(PARAM_RTK_CASTER_USER) : savedCaster);
  }

  else if (var == PARAM_RTK_MOINT_POINT) 
  {
    String savedCaster = readFile(LittleFS, getPath(PARAM_RTK_MOINT_POINT).c_str());
    return (savedCaster.isEmpty() ? String(PARAM_RTK_MOINT_POINT) : savedCaster);
  }
 
  else if (var == "next_addr") 
  {
    String savedSSID = readFile(LittleFS, getPath(PARAM_WIFI_SSID).c_str());
    String savedPW = readFile(LittleFS, getPath(PARAM_WIFI_PASSWORD).c_str());
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
    String savedSSID = readFile(LittleFS, getPath(PARAM_WIFI_SSID).c_str());
    return (savedSSID.isEmpty() ? getDeviceName(DEVICE_TYPE) : savedSSID);
  }
  return String();
}

/*
=================================================================================
                                LittleFS
=================================================================================
*/
bool RTKRoverManager::setupLittleFS() 
{
  bool isMounted = false; 

  if ( !LittleFS.begin() ) 
  {
    Serial.println(F("An Error has occurred while mounting LittleFS"));
    return isMounted;
  } 
  else
  {
    DBG.println("LittleFS mounted");
    isMounted = true;
  }

  return isMounted;
}

bool RTKRoverManager::formatLittleFS()
{
  DBG.println("Formatting file system, please wait...");
  bool formatted = LittleFS.format();
 
  if (formatted) 
  {
    DBG.println("\n\nSuccessfully formatted");
  }
  else
  {
    DBG.println("\n\nError during formatting");
  }

  return formatted;
}

String RTKRoverManager::readFile(fs::FS &fs, const char* path) 
{
  String fileContent;

  DBG.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, FILE_READ);

  if ( !file || file.isDirectory() ) 
  {
    DBG.println("- empty file or failed to open file");
    return String();
  }

  DBG.println("- read from file:");

  while ( file.available() ) 
  {
    fileContent += String( (char) file.read() );
  }
  file.close();
  DBG.println(fileContent);

  return fileContent;
}

bool RTKRoverManager::writeFile(fs::FS &fs, const char* path, const char* message) 
{ 
  bool success = false;
  DBG.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) 
  {
    DBG.println("- failed to open file for writing");
    return success;
  }
  if (file.print(message)) 
  {
    DBG.println("- file written");
    success = true;
  } else 
  {
    DBG.println("- write failed");
    success = false;
  }
  file.close();

  return success;
}

void RTKRoverManager::listFiles() 
{
  File root = LittleFS.open("/", FILE_READ);
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

String RTKRoverManager::getPath(const char* fileName)
{
  String path = "/";
  path += fileName;
  path += ".txt";

  return path;
}

void RTKRoverManager::clearFile(const char* fileName)
{
  if ( LittleFS.exists(fileName) )
  {
    if ( ! LittleFS.remove(fileName) )
    {
        assert("Failed to remove LittleFS path.");
    }
  }
}

void RTKRoverManager::wipeLittleFSFiles() 
{
  File root = LittleFS.open("/", FILE_WRITE);
  File file = root.openNextFile();

  DBG.println(F("Wiping: "));

  while (file) 
  {
      const char* pathStr = strdup(file.path());
      file.close();
      LittleFS.remove(pathStr);
      file = root.openNextFile();
  }

  DBG.println(F("Data in LittleFS wiped out!"));
}


String RTKRoverManager::getDeviceName(const String& prefix) 
  {
    String deviceName((char *)0);
    String suffix = String(getChipId(), HEX);

    unsigned int prefixLen = prefix.length();
    unsigned int suffixLen = suffix.length();
    
    deviceName.reserve(prefixLen + suffixLen);
    deviceName += prefix;
    deviceName += "-";
    deviceName += suffix;


    return deviceName;
  }

  uint32_t RTKRoverManager::getChipId() 
  {
    uint32_t chipId = 0;

    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
      return chipId;
  }
