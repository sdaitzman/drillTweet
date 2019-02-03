void wifiConnect() {
  int dotDelay = 200;
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
