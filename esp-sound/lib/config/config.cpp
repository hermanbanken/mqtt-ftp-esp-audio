#include <Arduino.h>
#include "ConfigManager.h"
#include "config.h"

const char mimeHTML[] PROGMEM = "text/html";
const char *configHTMLFile = "/settings.html";

struct Metadata {
  int8_t version;
} meta;

ConfigManager configManager;

void createCustomRoute(WebServer *server) {
  server->on("/settings.html", HTTPMethod::HTTP_GET, [server](){
    SPIFFS.begin();

    File f = SPIFFS.open(configHTMLFile, "r");
    if (!f) {
      Serial.println(F("file open failed"));
      server->send(404, FPSTR(mimeHTML), F("File not found"));
      return;
    }

    server->streamFile(f, FPSTR(mimeHTML));

    f.close();
  });
}

Config _config;

Config setupConfig() {
  // Setup config manager
  configManager.setAPName("ESP Sound");
  configManager.setAPFilename("/index.html");
  configManager.addParameter("name", _config.name, 255);
  configManager.addParameter("mqtt_host", _config.mqtt_host, 255);
  configManager.addParameter("mqtt_port", _config.mqtt_port, 10);
  configManager.addParameter("mqtt_username", _config.mqtt_username, 255);
  configManager.addParameter("mqtt_password", _config.mqtt_password, 255, set);
  configManager.addParameter("mqtt_topic", _config.mqtt_topic, 255);
  configManager.addParameter("enabled", &_config.enabled);
  configManager.addParameter("version", &meta.version, get);
  configManager.setAPICallback(createCustomRoute);
  configManager.begin(_config);
  return _config;
}

void loopConfig() {
  configManager.loop();
}
