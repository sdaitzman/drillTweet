#define samples 80

float total;
float reading;
float acRMS;

float alpha = 0.9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  total=0;
  for(int i = 0; i < samples; i++) {
    reading = analogRead(A0) - 512;
    total += reading * reading;
    if(total < 0) {
      Serial.print(total);
      Serial.print("AAAAAAAAAAAA");
      Serial.println(reading);
    }
    delayMicroseconds(417);
  }
  acRMS = acRMS * alpha + (1-alpha)*sqrt(total/samples);
  
  Serial.println(acRMS);
  
}
