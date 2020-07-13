#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "UARTTransfer.h"

UARTTransfer UT;
#define SET_NTP_TIME 51
#define GET_NTP_TIME 52

const char *ssid     = "tydirium";
const char *password = "heron271828";

// Часовой пояс
const long utcOffsetInSeconds = 3*3600;
// флаг обновления времени
bool flag = true;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  UT.begin(&Serial);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    }
timeClient.begin();
}

void loop() {
// если установлен флаг - запрашиваем время у сервера
  if ( flag)
  {  
    //WiFi.begin(ssid, password);
    if (timeClient.update()) {
      uint32_t tt = timeClient.getEpochTime();
      UT.sendData(SET_NTP_TIME, 4, (uint8_t*)&tt);
      flag = false;
    }
    //WiFi.disconnect();
  }

// если по Сериал получен запрос на обновление времени - устанавливаем флаг
if (UT.receiveData() ) {
  uint8_t m_id = UT.getMessageID();
  if (m_id == GET_NTP_TIME) flag = true;
  UT.clearMessage();
}

  delay(1000);
}