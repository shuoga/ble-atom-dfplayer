#include <M5Atom.h>

#include <DFPlayer.h>
#include <SoftwareSerial.h>

#define MP3_RX_PIN 25
#define MP3_TX_PIN 21
#define MP3_SERIAL_SPEED 9600

#define AVERAGE_SIZE 16  // 平滑化に使用する測定値の数

int valueArray[AVERAGE_SIZE] = { 0 };  // 測定値を格納する配列
int valueIndex = 0;                    // 現在の配列インデックス

SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
DFPlayer mp3;

void setup() {
  M5.begin();
  Serial.begin(115200);
  mySerial.begin(MP3_SERIAL_SPEED);
  mp3.begin(mySerial);
  delay(1000);
  mp3.setVolume(0);      // 初期音量の設定
  mp3.playMP3Folder(3);  // MP3フォルダの特定のトラックを再生
  mp3.repeatCurrentTrack(true);
}

void loop() {
  int rawValue = analogRead(33) - 5;  // 生のアナログ値を読み取る
  if (rawValue < 0) {
    rawValue = 0;
  }

  // 配列に値を追加し、古い値を上書き
  valueArray[valueIndex] = rawValue;
  valueIndex = (valueIndex + 1) % AVERAGE_SIZE;  // インデックスを更新

  // 移動平均の計算
  int sum = 0;
  for (int i = 0; i < AVERAGE_SIZE; i++) {
    sum += valueArray[i];
  }
  int averageValue = sum / AVERAGE_SIZE;
/
  // 平滑化された値を使用して音量を設定
  int v = map(int(sqrt((averageValue * 0.8))), 7, 27, 0, 12);
  v = constrain(v, 0, 30);  // vを0から30の範囲に制限
  Serial.println(v);
  mp3.setVolume(v);
}