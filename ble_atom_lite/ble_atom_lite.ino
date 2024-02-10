#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <esp_sleep.h>

#define T_PERIOD 1                 // Transmission period
#define S_PERIOD 10                // Silent period
RTC_DATA_ATTR static uint8_t seq;  // remember number of boots in RTC Memory

void setAdvData(BLEAdvertising *pAdvertising) {
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06);  // BR_EDR_NOT_SUPPORTED | LE General Discoverable Mode
  std::string strServiceData = "";
  strServiceData += (char)0x0b;  // データの長さ
  strServiceData += (char)0xff;  // データタイプ 0xFF: Manufacturer specific data
  strServiceData += (char)0xee;  // 製造者IDの下位バイト
  strServiceData += (char)0xff;  // 製造者IDの上位バイト
  strServiceData += (char)seq;   // シーケンス番号
  strServiceData += (char)0x02;  // センサ１の下位バイト
  strServiceData += (char)0x03;  // センサ１の上位バイト
  strServiceData += (char)0x04;  // センサ２の下位バイト
  strServiceData += (char)0x05;  // センサ２の上位バイト
  strServiceData += (char)0x06;  // センサ３の下位バイト
  strServiceData += (char)0x07;  // センサ３の上位バイト
  oAdvertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(oAdvertisementData);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.printf("setup start seq: %d\n", seq);

  BLEDevice::init("ATOM-LITE");  // デバイスを初期化
  Serial.println("Device Initialize started...");

  // BLEServer *pServer = BLEDevice::createServer();  // サーバーを生成
  // Serial.println("Server created...");

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // アドバタイズオブジェクトを取得
  // setAdvData(pAdvertising);                                  // アドバタイジングデーターをセット
  // Serial.println("Advertizing seted...");

  // pAdvertising->start();  // アドバタイズ起動
  // Serial.println("Advertizing started...");
  // delay(T_PERIOD * 1000);  // T_PERIOD秒アドバタイズする
  // pAdvertising->stop();    // アドバタイズ停止
  // Serial.println("Advertizing ended...");

  // seq++;  // シーケンス番号を更新

  // Serial.printf("enter deep sleep\n");
  // delay(10);
  // esp_deep_sleep(1000000LL * S_PERIOD);  // S_PERIOD秒Deep Sleepする
  // Serial.printf("in deep sleep\n");
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed()) {  //If the button is pressed
    Serial.println("Button is pressed.");

    BLEServer *pServer = BLEDevice::createServer();  // サーバーを生成
    Serial.println("Server created...");

    BLEAdvertising *pAdvertising = pServer->getAdvertising();  // アドバタイズオブジェクトを取得
    setAdvData(pAdvertising);                                  // アドバタイジングデーターをセット
    Serial.println("Advertizing Data set...");

    pAdvertising->start();  // アドバタイズ起動
    Serial.println("Advertizing started...");
    delay(T_PERIOD * 1000);  // T_PERIOD秒アドバタイズする
    pAdvertising->stop();    // アドバタイズ停止
    Serial.println("Advertizing ended...");

    seq++;  // シーケンス番号を更新
  }
}