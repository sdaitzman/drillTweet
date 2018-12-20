void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  Serial.println(analogRead(A0) - 512);
  delay(random(10));
}
