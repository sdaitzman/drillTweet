#define samples 120

float total;
float reading;
float acRMS;
float current;

float alpha = 0.33;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(33, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  analogSetCycles(2);
  analogSetSamples(1);
  analogSetWidth(10);
}

void loop() {
  total=0;
  for(int i = 0; i < samples; i++) {
    reading = analogRead(33) - 512;
    total += reading * reading;
    delayMicroseconds(275);
  }
  acRMS = acRMS * (1-alpha) + alpha * sqrt(total/samples);

  if(acRMS > 95) {
    digitalWrite(13, 100);
  } else {
    digitalWrite(13, 0);
  }

  current = 0.01304 * acRMS - 0.04421; // from calibration curve in calibration.csv
  if(current < 0) current = 0;

  Serial.println(current);

}
