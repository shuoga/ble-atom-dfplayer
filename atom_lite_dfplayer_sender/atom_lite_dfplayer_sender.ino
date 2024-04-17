#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <esp_sleep.h>

#define T_PERIOD 100  // 送信間隔（ミリ秒）
// #define S_PERIOD 10                // 休止間隔

void setAdvData(BLEAdvertising *pAdvertising, int value) {
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06);  // BR_EDR_NOT_SUPPORTED | LE General Discoverable Mode
  std::string strServiceData = "";
  strServiceData += (char)0x05;   // データの長さ
  strServiceData += (char)0xff;   // データタイプ 0xFF: Manufacturer specific data
  strServiceData += (char)0xee;   // 製造者IDの下位バイト
  strServiceData += (char)0xdd;   // 製造者IDの上位バイト
  strServiceData += (char)value;  // 引数の値(1バイト)
  // ここでアドバタイジングデータをシリアルモニタに表示
  Serial.print("Advertising Data: ");
  for (size_t i = 0; i < strServiceData.length(); i++) {
    Serial.printf("%02X ", (unsigned char)strServiceData[i]);
  }
  Serial.println();  // 改行
  oAdvertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(oAdvertisementData);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  BLEDevice::init("ATOM-LITE-SENDER");  // デバイスを初期化
  Serial.println("Device Initialize started...");
}

void loop() {
  int value = analogRead(33);  // アナログ値を読み取る
  Serial.print("Analog Read Value: ");
  Serial.println(value);

  int v = int((value * value * 30) / (1024 * 1024));  // 平方根を計算
  Serial.print("Calculated Value (v): ");
  Serial.println(v);

  // BLEサーバーを生成（すでに存在しない場合）
  static BLEServer *pServer = nullptr;
  if (!pServer) {
    pServer = BLEDevice::createServer();
    Serial.println("Server created...");
  }

  BLEAdvertising *pAdvertising = pServer->getAdvertising();  // アドバタイズオブジェクトを取得
  setAdvData(pAdvertising, v);                               // アドバタイジングデーターをセット
  Serial.println("Advertizing Data set...");

  pAdvertising->start();  // アドバタイズ開始
  Serial.println("Advertising started.");
  delay(T_PERIOD);       // アドバタイジング開始を確実にするための遅延
  pAdvertising->stop();  // アドバタイズ停止
  Serial.println("Advertising stopped.");
}