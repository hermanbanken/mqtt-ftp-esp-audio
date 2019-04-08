struct Config {
  char name[255];
  bool enabled;

  char mqtt_host[255];
  char mqtt_port[10];
  char mqtt_username[255];
  char mqtt_password[255];
  char mqtt_topic[255];
};

Config setupConfig();
void loopConfig();
