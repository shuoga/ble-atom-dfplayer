#include <M5Atom.h>
#include <BLEDevice.h>
#include <DFPlayer.h>
#include <SoftwareSerial.h>

#define MyManufacturerId 0xddee  // テスト用の製造者ID
#define MP3_RX_PIN 25
#define MP3_TX_PIN 21
#define MP3_SERIAL_SPEED 9600

BLEScan* pBLEScan;
SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
DFPlayer mp3;

bool scanStarted = false;  // スキャンが開始されたかどうかのフラグ

// BLEデバイスが見つかった時のコールバックを処理するクラス
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    if (advertisedDevice.haveManufacturerData()) {
      std::string manData = advertisedDevice.getManufacturerData();
      uint16_t foundManuId = (manData[1] << 8) | manData[0];
      if (foundManuId == MyManufacturerId) {
        // Serial.println("Desired device found, stopping scan...");
        pBLEScan->stop();            // スキャンを停止
        scanStarted = false;         // スキャンのフラグをリセット
        uint8_t value = manData[2];  // 最後のバイトをボリューム値として使用
        if (value > 30) value = 30;  // ボリュームの最大値を30に制限
        mp3.setVolume(value);        // ボリュームを設定
        Serial.printf("New volume set to: %d\n", value);
      }
    }
  }
};

void setup() {
  M5.begin();
  Serial.begin(115200);
  mySerial.begin(MP3_SERIAL_SPEED);
  mp3.begin(mySerial);
  delay(1000);
  mp3.setVolume(0);      // 初期音量の設定
  mp3.playMP3Folder(1);  // MP3フォルダの特定のトラックを再生
  mp3.repeatCurrentTrack(true);
  Serial.println("scan start.");
  BLEDevice::init("ATOM-LITE-RECIEVER");
  pBLEScan = BLEDevice::getScan();  // BLEスキャンを設定
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // アクティブスキャンを使用
}

void loop() {
  if (!scanStarted) {
    pBLEScan->start(1, false);  // スキャン開始（指定された秒数でスキャンを開始）
    scanStarted = true;
  } else {
    scanStarted = false;
  }
}