#include <DHT.h>
#include "header.h"
#include "setwifi.h"
#include "handler/http.h"
#include "handler/setupSinricPro.h"

#define BAUD_RATE 115200 // Change baudrate to your need (used for serial monitor)
#define WIFI_SSID "TP-LINK_Extender_2.4GHz"
#define WIFI_PASS "jf981103"

// main setup function
// It initializes the serial communication, sets the pin modes for various pins, sets up the WiFi connection, sets up a SinricPro connection, and begins a server.
void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");
  // dht.begin();
  pinMode(23, OUTPUT); // red
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  setupWiFi(WIFI_SSID, WIFI_PASS);
  setupSinricPro();
  beginServer();
}

void loop()
{
  server.handleClient();
  SinricPro.handle();
  handleTemperaturesensor();
  controalWindow();
}

void beginServer()
{
  server.on("/", handleRoot);
  server.on("/device", requestHandlerarq);
  server.begin();
  Serial.println("HTTP server started");
}