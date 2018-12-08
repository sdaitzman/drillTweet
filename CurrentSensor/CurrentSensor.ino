#define samples 120

float total;
float reading;
float acRMS;

float alpha = 0.33;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  total=0;
  for(int i = 0; i < samples; i++) {
    reading = analogRead(A0) - 512;
    total += reading * reading;
    delayMicroseconds(275);
  }
  acRMS = acRMS * (1-alpha) + alpha * sqrt(total/samples);
  if(acRMS > 150) {
    analogWrite(11, 100);
  } else {
    analogWrite(11, 0);
  }
  if(acRMS > 95) {
    digitalWrite(13, 100);
  } else {
    digitalWrite(13, 0);
  }
  Serial.println(acRMS);
  
}
