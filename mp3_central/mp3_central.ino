#include "M5Atom.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include "esp_sleep.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String MACadd = "XA:XB:XC:XD:XE:XF";
uint8_t macBT[6];
uint8_t address[6]  = {0xXA, 0xXB, 0xXC, 0xXD, 0xXE, 0xXF};
bool connected;

int bluetooth_address =0;
uint16_t analogRead_value = 0;
uint16_t digitalRead_value = 0;
uint16_t device_id = 0x0101;        // your any sensor device id

BLEAdvertising *pAdvertising;

void readEarthSensor() {
  analogRead_value = analogRead(32);
  digitalRead_value = digitalRead(26);
  Serial.printf("0x%04x, %d, %d\n", device_id, digitalRead_value, analogRead_value);
}

void print_readEarthSensor() {
  analogRead_value = analogRead(32);
  digitalRead_value = digitalRead(26);
  //SerialBT.printf("%d,%d\r", digitalRead_value, analogRead_value);
  SerialBT.printf("%d\r", analogRead_value);
}

void setAdvData() {
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  oAdvertisementData.setFlags(0x06); // BR_EDR_NOT_SUPPORTED | LE General Discoverable Mode
  std::string strServiceData = "";
  strServiceData += (char)0x08;   // length
  strServiceData += (char)0xff;   // AD Type 0xFF: Manufacturer specific data
  strServiceData += (char)0xff;   // Test manufacture ID
  strServiceData += (char)0xff;   // Test manufacture ID
  strServiceData += (char)((device_id >> 8) & 0xff);
  strServiceData += (char)(device_id & 0xff);
  strServiceData += (char)((analogRead_value >> 8) & 0xff);
  strServiceData += (char)(analogRead_value & 0xff);
  strServiceData += (char)(digitalRead_value & 0xff);

  oAdvertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(oAdvertisementData);
}

void setup() {
  M5.begin(true, false, true);
  delay(10);
  Serial.begin(115200);
  M5.dis.clear();
  pinMode(26, INPUT);
  Serial.println("\nEarth Test");
  // Read earth sensor value
  readEarthSensor();
  
  // Create the BLE Device
  BLEDevice::init("Earth_Beacon");
  pAdvertising = BLEDevice::getAdvertising();

  // set advertizing packet data
  setAdvData();
  delay(3000);
  
  // Check my bluethooth address. for debug.
  if (bluetooth_address == 1){
  Serial.println("-----------------");
  esp_read_mac(macBT, ESP_MAC_BT);
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
  }
  // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
  delay(1200);
  pAdvertising->stop();
  
  // Set bluethooth master.
  SerialBT.begin("ESP32test", true);  //Bluetooth device name
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  connected = SerialBT.connect(address);  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();

}

void loop() {

  if (M5.Btn.wasPressed()) {
  // Read earth sensor value
  readEarthSensor();
  print_readEarthSensor();
  }
  M5.update();
  delay(1*1000);

}
