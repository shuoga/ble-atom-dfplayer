#include <M5Atom.h>

#include <DFPlayer.h>
#include <SoftwareSerial.h>

#define MP3_RX_PIN 25          //GPIO4/D2 to DFPlayer Mini TX
#define MP3_TX_PIN 21          //GPIO5/D1 to DFPlayer Mini RX
#define MP3_SERIAL_SPEED 9600  //DFPlayer Mini suport only 9600-baud

SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);  // RX, TX
DFPlayer mp3;

void setup() {
  Serial.begin(9600);
  mySerial.begin(MP3_SERIAL_SPEED);
  mp3.begin(mySerial);
  mp3.setVolume(20);
  Serial.println(mp3.getStatus());  //0=stop, 1=playing, 2=pause, 3=sleep or standby, 4=communication error, 5=unknown state
  delay(3000);
}

void loop() {
  M5.update();  // 本体のボタン状態更新
  if (M5.Btn.wasPressed()) {
    mp3.playTrack(1);
    // delay(3000);  //play for 60 seconds
    // mp3.pause();
    delay(1000);  //pause for 10 seconds
  }
}