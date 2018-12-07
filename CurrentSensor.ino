#define samples 1000

float total;
float reading;
float acRMS;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  total=0;
  for(int i = 0; i <= samples; i++) {
    reading = analogRead(A0)/1023*5-2.5;
    total += reading * reading;
  }
  acRMS = sqrt(total / samples);
  
  
  Serial.println(analogRead(A0));

  delay(50);
}
