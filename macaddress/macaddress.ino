uint8_t MACAddress[6];

void setup() {
  Serial.begin(115200);
  esp_read_mac(MACAddress, ESP_MAC_BT);
  Serial.printf("MAC Address = %02X:%02X:%02X:%02X:%02X:%02X\n", MACAddress[0], MACAddress[1], MACAddress[2], MACAddress[3], MACAddress[4], MACAddress[5]);
}

void loop() {
  Serial.printf("MAC Address = %02X:%02X:%02X:%02X:%02X:%02X\n", MACAddress[0], MACAddress[1], MACAddress[2], MACAddress[3], MACAddress[4], MACAddress[5]);
}