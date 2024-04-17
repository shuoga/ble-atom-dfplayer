#include <M5Atom.h>
#include <esp_now.h>
#include <WiFi.h>

esp_now_peer_info_t slave;

// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { // 送信先の MAC ADDRESS と 送信ステータス
  char macStr[18]; // コロン区切りの MAC ADDRESS と、最後にヌル文字を格納するための配列
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  for (int i = 0; i < data_len; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
void setup() {
  // シリアルの初期設定
  Serial.begin(115200);
  // ESP-NOWの初期化
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == ESP_OK) {  // esp_now_init() が実行され、その結果を評価
    Serial.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();  // esp の再起動
  }

  // マルチキャスト用Slave登録
  memset(&slave, 0, sizeof(slave));  // slave 変数用メモリの初期化

  // 対象の MAC ADDRESS の指定
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;  // ここで対象の MAC ADDRESS の 6 桁を指定
  }

  // 対象となる MAC ADDRESS をピアリストに設定
  esp_err_t addStatus = esp_now_add_peer(&slave);  // 上記で設定した MAC ADDRESS が設定される

  if (addStatus == ESP_OK) {  // 設定が正常に完了したら
    Serial.println("Pair Success");
  } else {
    Serial.println("Pair Failed");
    ESP.restart();  // esp の再起動
  }
  // ESP-NOWコールバック登録
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}
void loop() {
  M5.update();
  // ボタンを押したら送信
  if (M5.Btn.wasPressed()) {
    uint8_t data[2] = { 123, 234 };
    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));
    Serial.print("Send Status: ");
    if (result == ESP_OK) {
      Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
      Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    } else {
      Serial.println("Not sure what happened");
    }
  }
  delay(1);
}