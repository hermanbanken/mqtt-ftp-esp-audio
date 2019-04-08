// void wav_start_random();
// void wav_loop();#define SS_PIN 0
#ifdef ESP32
#define MOSI_PIN 0
#define MISO_PIN 0
#define SCK_PIN 0
#include <SD.h>
#endif

#ifdef ESP8266
#include <FS.h>   // Include the SPIFFS library
#endif

char* randomFile();
void wav_setup();
void wav_loop();
void wav_start_random();
