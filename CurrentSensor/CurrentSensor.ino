#define samples 80

double total;
int reading;
float acRMS;

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
    delayMicroseconds(417);
  }
  acRMS = sqrt(total/samples);
  
  Serial.println(acRMS);
  
}
