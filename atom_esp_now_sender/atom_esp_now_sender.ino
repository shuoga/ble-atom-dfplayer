#include <M5Atom.h>

#include <DFPlayer.h>
#include <SoftwareSerial.h>

#define MP3_RX_PIN 25
#define MP3_TX_PIN 21
#define MP3_SERIAL_SPEED 9600

SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
DFPlayer mp3;

void setup() {
  M5.begin();
  Serial.begin(115200);

  mySerial.begin(MP3_SERIAL_SPEED);
  mp3.begin(mySerial);

  delay(1000);

  mp3.setVolume(20);     // 初期音量の設定
  mp3.playMP3Folder(1);  // MP3フォルダの特定のトラックを再生
  mp3.repeatCurrentTrack(true);
}

void loop() {
  M5.update();
  if (M5.Btn.isPressed()) {
    mp3.setVolume(20);
  } else {
    mp3.setVolume(0);
  }
}