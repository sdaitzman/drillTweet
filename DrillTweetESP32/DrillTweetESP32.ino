#include "config.h"
#include <WiFi.h>
#include <WebSocketClient.h>
#define samples 120

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

float total;
float reading;
float acRMS;
float current;

float alpha = 0.33;

void wifiConnect() {
  int dotDelay = 500;
  unsigned long timeOut = millis() + 10 * 1000;
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.enableSTA(true);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.getAutoReconnect()) {
      WiFi.begin();
    }
    delay(dotDelay);
    dotDelay *= 2;
    Serial.print(".");
    if (millis() >= timeOut) {
      Serial.println("\nWiFi connection attempts have failed. Making one final 10-second connection attempt...");
      if (WiFi.getAutoReconnect()) {
        WiFi.begin();
      }
      delay(10 * 1000);
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nFinal WiFi connection attempt timed out. ESP32 will now reboot...\n\n\n");
        delay(1000);
        ESP.restart();
      }
    }
  }

  Serial.println("");
  Serial.print("WiFi connected with IP address: ");
  Serial.println(WiFi.localIP());
}

void serverConnect() {
  Serial.println("Attempting to connect to server...");
  // First, make sure we are connected to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to WiFi. Initiating reconnect...");
    wifiConnect();
  }
  // Connect to the websocket server
  if (client.connect(host, 8080)) {
    Serial.println("Connected to server. Attempting handshake...");
    serverHandshake();
  } else {
    Serial.println("Connection to server failed. Trying again...");
    delay(5);
    serverConnect();
  }
}

void serverHandshake() {
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake with server successful");
  } else {
    Serial.println("Handshake failed. Will try connecting again.");
    serverConnect();
  }
}

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
        total = 0;
        for (int i = 0; i < samples; i++) {
          reading = analogRead(33) - 512;
          total += reading * reading;
          delayMicroseconds(275);
        }
        acRMS = acRMS * (1 - alpha) + alpha * sqrt(total / samples);

        if (acRMS > 95) {
          digitalWrite(13, 100);
        } else {
          digitalWrite(13, 0);
        }

        current = 0.01304 * acRMS - 0.04421; // from calibration curve in calibration.csv
        if (current < 0) current = 0;
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
