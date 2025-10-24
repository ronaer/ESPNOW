/*******************************************************************************
     ____       __________              _ __  
    / __ \_____/_  __/ __ \____  ____  (_) /__
   / / / / ___/ / / / /_/ / __ \/ __ \/ / //_/
  / /_/ / /  _ / / / _, _/ /_/ / / / / / ,<   
 /_____/_/  (_)_/ /_/ |_|\____/_/ /_/_/_/|_|  

 EYLÜL 2025 / İzmir / TURKIYE
 ESPNOW
 ESP'ler arası direkt iletişim
 https:  https://youtu.be/zcTijyIbmYo
 Hardware Connections:  
  GPIO  14 / D5- DHT11
  BMP180 varsayılan / default I2C pinleri:
  BMP180 SCL = GPIO 5 / D1
  BMP180 SDA = GPIO 4 / D2
 
*******************************************************************************/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>  // BMP180 kütüphanesi
Adafruit_BMP085 bmp;
#define DHTPIN 14      //D5
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//int temp, hum, pressure;
unsigned long timer, timer1, timer2, timer3;

// ESP-NOW alıcıların MAC adresleri
uint8_t receiverList[][6] = {
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },  // Alıcı 1.Esp MAC ADRESİNİ giriniz, (Bu hali (tek satır 0xFF,...) ile tüm alıcı esp lere yayın / broadcast yapar.)
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },  // Alıcı 2.Esp MAC ADRESİNİ giriniz
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },  // Alıcı 3.Esp MAC ADRESİNİ giriniz
};
const int receiverCount = 3;  //Toplamda 3 alıcı ESP

// Gönderilecek veri yapısını oluşturma
typedef struct struct_message {
  int temp;
  int hum;
  int pressure;
} struct_message;

struct_message myData;

// Veri gönderildikten sonra çağrılır
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Gönderim durumu: ");
  if (sendStatus == 0) {
    Serial.println("Başarılı ✅");
  } else {
    Serial.println("Başarısız ❌");
  }
}

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {

  // Start DHT11
  dht.begin();
  delay(100);
  Serial.begin(9600);
  delay(100);
  WiFi.mode(WIFI_STA);
  // ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW başlatılamadı");
    return;
  }

  if (!bmp.begin()) {
    Serial.println("Geçerli sensor yok veya kablolama  hatalı!");
    while (1) {}
  }

  // Callback
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Alıcıları peer olarak ekle
  for (int i = 0; i < receiverCount; i++) {
    esp_now_add_peer(receiverList[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  }
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop() {

  if (millis() - timer > 60 * 1000) {
    timer = millis();

    myData.temp = dht.readTemperature();              //Sıcaklık değeri
    myData.hum = dht.readHumidity();                  //Nem değeri
    myData.pressure = (bmp.readPressure() / 100.0F);  // Pa/100.0F= hPa

    Serial.print("SICAKLIK *C: ");
    Serial.println(myData.temp);
    Serial.print("NEM % : ");
    Serial.println(myData.hum);
    Serial.print("BASINÇ :");
    Serial.print(myData.pressure) + Serial.println(" hPa");
    Serial.println("-------------");

    // Tüm alıcılara sırayla gönder
    for (int i = 0; i < receiverCount; i++) {
      esp_now_send(receiverList[i], (uint8_t *)&myData, sizeof(myData));
    }
  }
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
