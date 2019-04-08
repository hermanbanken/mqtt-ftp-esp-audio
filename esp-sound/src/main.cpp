#include <HTTPClient.h>
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#endif
#include <ESP8266FtpServer.h>
#include <player.h>
#include <config.h>
#include <Arduino.h>
#include <PubSubClient.h>

Config config;
FtpServer ftpSrv;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
char* topic_ping = "ping";
char* topic_pong = "ping";
char* topic_action = "trigger";

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
	if (topic == topic_ping) {
		mqtt_client.publish(topic_pong, "esp-sound");
	}
	if (topic == topic_action) {
		wav_start_random();
	}
}

void reconnect() {
  while (!mqtt_client.connected()) {
    if (mqtt_client.connect("espSound")) {
      mqtt_client.subscribe(topic_ping);
      mqtt_client.subscribe(topic_action);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("");

  // Load ConfigManager
  config = setupConfig();

  // Start FTP server.
  ftpSrv.begin("admin","gogogo");

  // Start MQTT
  mqtt_client.setServer(config.mqtt_host, String(config.mqtt_port).toInt());
  mqtt_client.setCallback(mqtt_callback);

  wav_setup();
}

void loop() {
  wav_loop();

  loopConfig();

  ftpSrv.handleFTP();

  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();
}
