/*******************************************************************************
     ____       __________              _ __  
    / __ \_____/_  __/ __ \____  ____  (_) /__
   / / / / ___/ / / / /_/ / __ \/ __ \/ / //_/
  / /_/ / /  _ / / / _, _/ /_/ / / / / / ,<   
 /_____/_/  (_)_/ /_/ |_|\____/_/ /_/_/_/|_|  

 EYLÜL 2025 / İzmir / TURKIYE
 ESPNOW Reciever
 ESP'ler arası direkt iletişim
 https:  https://youtu.be/zcTijyIbmYo
 
 
*******************************************************************************/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/

#include <ESP8266WiFi.h>
#include <espnow.h>

// Vericiyle aynı veri yapısını tanımla
typedef struct struct_message {
  int temp;
  int hum;
  int pressure;
} struct_message;

struct_message incomingData;

// Veri alındığında tetiklenecek callback
void onDataRecv(uint8_t *mac, uint8_t *incomingDataBuf, uint8_t len) {
  memcpy(&incomingData, incomingDataBuf, sizeof(incomingData));

  // Verileri sabit uzunlukla UART’a gönderelim (ör: 2 hane sıcaklık, 2 hane nem, 4 hane basınç)
  char buffer[10];
  sprintf(buffer, "%02d%02d%04d", incomingData.temp, incomingData.hum, incomingData.pressure);
  Serial.println(buffer);  // TX üzerinden ESP8266 master’a gönder
}

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  // ESP-NOW başlat
  if (esp_now_init() != 0) {
    return;
  }
  // Alıcı / Slave olarak ayarla
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  // Callback fonksiyonunu kaydet
  esp_now_register_recv_cb(onDataRecv);
}


/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop() {
}

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/

/*
📌//___:
📩 bilgi@ronaer.com
🟩 https://whatsapp.com/channel/0029VaxtFPiLSmbgUryuGs0E
📷 https://www.instagram.com/dr.tronik2023/   
📺 www.youtube.com/c/DrTRonik 
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C
*/
