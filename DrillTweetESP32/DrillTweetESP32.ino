#include "config.h"
#include <WiFi.h>
#include <WebSocketClient.h>
#define samples 120

WebSocketClient webSocketClient;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set up ADC
  pinMode(33, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  analogSetCycles(2);
  analogSetSamples(1);
  analogSetWidth(10);

  wifiConnect();

  delay(50);

  serverConnect();
}


void loop() {
  String data;

  if (client.connected()) {
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data == "getReading") {
        Serial.println("Sending a reading!");
        float current = getCurrent();
        webSocketClient.sendData(String(current));
        Serial.println(current);
        Serial.println(WiFi.macAddress());
      }
    }
  } else {
    Serial.println("Disconnected from WebSocket server. Will attempt reconnect in 5 seconds...");
    delay(5000);
    serverConnect();
  }

}
