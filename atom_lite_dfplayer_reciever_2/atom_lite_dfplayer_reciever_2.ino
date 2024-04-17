#include <M5AtomS3.h>
#include <BLEDevice.h>
#include <DFPlayer.h>
#include <SoftwareSerial.h>

#define MyManufacturerId 0xddee  // テスト用の製造者ID
BLEScan* pBLEScan;

bool scanStarted = false;  // スキャンが開始されたかどうかのフラ

const int ledChannel = 0;  // 使用するPWMチャネルをグローバル変数として宣言

void setup() {
  AtomS3.begin(true);  // Init M5AtomS3Lite.
  AtomS3.dis.setBrightness(0);
  AtomS3.dis.drawpix(0x990000);
  AtomS3.update();

  Serial.begin(115200);
  delay(1000);
  Serial.println("scan start.");
  BLEDevice::init("ATOM-LITE-RECIEVER");
  pBLEScan = BLEDevice::getScan();  // BLEスキャンを設定
  pBLEScan->setActiveScan(false);   // パッシブスキャンを使用
  pBLEScan->start(1, false);        // スキャン開始（指定された秒数でスキャンを開始）

  // PWMの設定
  const int freq = 5000;     // PWMの周波数
  const int resolution = 8;  // 分解能（8ビットなら0-255の値）
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(8, ledChannel);
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(1, false);
  int count = foundDevices.getCount();
  for (int i = 0; i < count; i++) {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    if (d.haveManufacturerData()) {
      std::string manData = d.getManufacturerData();
      uint16_t foundManuId = (manData[1] << 8) | manData[0];
      // Serial.printf("foundManuId: %d\r\n", foundManuId);
      // Serial.printf("MyManufacturerId: %d\r\n", MyManufacturerId);
      if (foundManuId == MyManufacturerId) {
        uint8_t value = manData[2];  // 最後のバイトをボリューム値として使用
        int v = map(value, 0, 30, 0, 255);
        Serial.printf("value: %d\r\n", value);
        Serial.printf("v: %d\r\n", v);
        ledcWrite(ledChannel, v);  // PWM信号を出力
        Serial.printf("New volume set to: %d\n", value);
      }
    }
  }
}