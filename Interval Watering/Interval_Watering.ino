const int solPin1 = 25;
const int WATER_DUR = 30 //how long the water is open
const int WATER_INTERVAL = 24 //how many hours between waterings
void setup() {
  unsigned long water_dur = WATER_DUR * 3600 * 1000;
  unsigned long water_interval = WATER_INTERVAL * 60 * 3600 * 1000;
  pinMode(solPin1, OUTPUT)
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(solPin1, HIGH);
  delay(water_dur);
  digitalWrite(solPin1, LOW);
  delay(water_interval);

}
