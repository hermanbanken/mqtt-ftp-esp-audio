#ifdef ESP32
#include <SPIFFS.h>
#endif

#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;

void wav_setup() {
  mp3 = new AudioGeneratorMP3();
  out = new AudioOutputI2SNoDAC();
}

const char* randomFile() {
	int count = 0;

#ifdef ESP32
	File dir = SPIFFS.open("/");
	while(dir.openNextFile()){
		count++;
	}
#endif
#ifdef ESP8266
  Dir dir = SPIFFS.openDir("/");
	while (dir.next()) {
		count++;
  }
#endif

	int selected = random(count);

#ifdef ESP32
	dir = SPIFFS.open("/");
	File file;
	for (int i = 0; i < selected; i++) { file = dir.openNextFile(); }
	return file.name();
#endif
#ifdef ESP8266
  dir = SPIFFS.openDir("/");
	for (int i = 0; i < selected && dir.next(); i++);
	return dir.fileName().c_str();
#endif

}

void wav_start_random() {
	file = new AudioFileSourceSPIFFS(randomFile());
	mp3->begin(file, out);
}

void wav_loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  }
}
