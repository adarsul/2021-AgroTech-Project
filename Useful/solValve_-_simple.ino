int solPin1 = 25;
int solPin2 = 26; //output on board for Solenoid valve
int solPin3 = 27;
void setup() {
  // put your setup code here, to run once:
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(solPin3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(solPin1, HIGH);
  delay(1000);
  digitalWrite(solPin1, LOW);
  delay(1000);
  digitalWrite(solPin2, HIGH);
  delay(1000);
  digitalWrite(solPin2, LOW);
  delay(1000);
  digitalWrite(solPin3, HIGH);
  delay(1000);
  digitalWrite(solPin3, LOW);
  delay(1000);
}
