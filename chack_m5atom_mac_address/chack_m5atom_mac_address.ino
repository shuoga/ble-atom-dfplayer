#include <M5Atom.h>
#include <WiFi.h>

uint8_t macBT[6];

void setup() {
  Serial.begin(115200);
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed()) {
    Serial.print("WIfi MAC ADDRESS: ");
    Serial.println(WiFi.macAddress());
    esp_read_mac(macBT, ESP_MAC_BT);
    Serial.printf("Bluetooth MAC ADDRESS: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
  }
}