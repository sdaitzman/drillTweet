void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(33, INPUT_PULLUP);
}

void loop() {
  Serial.println(analogRead(33) - 512);
  delay(random(10));
}
