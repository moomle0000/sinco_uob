#include <DHT.h>
#include "header.h"
#include "setwifi.h"
#include "setupSinricPro.h"
#include "handler/http.h"
#include "handler/temp.h"
#define WIFI_SSID "TP-LINK_Extender_2.4GHz"
#define WIFI_PASS "jf981103"


// main setup function
void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");
  dht.begin();
  pinMode(23, OUTPUT); // red
  pinMode(22, OUTPUT); // green
  pinMode(21, OUTPUT); // green

  setupWiFi(WIFI_SSID, WIFI_PASS);
  setupSinricPro();
  beginServer();
}

void loop()
{

  SinricPro.handle();
  server.handleClient();
  handleTemperaturesensor();
}

void beginServer()
{
  server.on("/", handleRoot);
  server.on("/lm", requestHandlerarq);
  server.begin();
  Serial.println("HTTP server started");
}