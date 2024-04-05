#include <M5Atom.h>
#include <BLEDevice.h>

#include <DFPlayer.h>
#include <SoftwareSerial.h>

uint8_t seq;                     // remember number of boots in RTC Memory
#define MyManufacturerId 0xffee  // test manufacturer ID

#define MP3_RX_PIN 25
#define MP3_TX_PIN 21
#define MP3_SERIAL_SPEED 9600

BLEScan* pBLEScan;

SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
DFPlayer mp3;

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.print("\r\nscan start.\r\n");

  mySerial.begin(MP3_SERIAL_SPEED);
  mp3.begin(mySerial);
  mp3.setVolume(20);
  delay(1000);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(false);
}

void loop() {
  //bool found = false;
  float temp, humid, press;

  BLEScanResults foundDevices = pBLEScan->start(3);
  int count = foundDevices.getCount();
  Serial.printf("device count: %d\r\n", count);
  for (int i = 0; i < count; i++) {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    // data check
    // Serial.print("Device Address: ");
    // Serial.println(d.getAddress().toString().c_str());
    // Serial.print("Device Name: ");
    // if (d.haveName()) {
    //   Serial.println(d.getName().c_str());
    // } else {
    //   Serial.println("");
    // }
    // Serial.print("Device Manufacturer Data: ");
    // if (d.haveManufacturerData()) {
    //   Serial.println(d.getAddress().toString().c_str());
    // }
    // end
    if (d.haveManufacturerData()) {
      std::string data = d.getManufacturerData();
      int manu = data[1] << 8 | data[0];
      Serial.printf("manu: %d\r\n", manu);
      Serial.printf("myid: %d\r\n", MyManufacturerId);
      if (manu == MyManufacturerId && seq != data[2]) {
        Serial.printf("data[0]: %d, data[1]: %d, data[2]: %d, data[3]: %d, data[4]: %d, data[5]: %d, data[6]: %d, data[7]: %d, data[8]: %d\r\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
        //found = true;
        seq = data[2];
        temp = (float)(data[4] << 8 | data[3]) / 100.0;
        humid = (float)(data[6] << 8 | data[5]) / 100.0;
        press = (float)(data[8] << 8 | data[7]) * 10.0 / 100.0;
        Serial.printf(">>> seq: %d, t: %.1f, h: %.1f, p: %.1f\r\n", seq, temp, humid, press);
        mp3.playTrack(1);
      }
    }
  }
}
