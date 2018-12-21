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

void setup() {
  Serial.begin(115200);
  delay(10);

  // Set up ADC
  pinMode(33, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  analogSetCycles(2);
  analogSetSamples(1);
  analogSetWidth(10);


  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.enableSTA(true);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if(WiFi.getAutoReconnect()) {
      WiFi.begin();
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);


  // Connect to the websocket server
  if (client.connect(host, 8080)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while (1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while (1) {
      // Hang on failure
    }
  }

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
      }
    }

    data = String(random(100));

    //    webSocketClient.sendData(data);

  } else {
    Serial.println("Client disconnected.");
    //    while (1) {
    //      // Hang on disconnect.
    //    }
  }

}
