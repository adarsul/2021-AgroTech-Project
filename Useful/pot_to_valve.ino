/*
This code is used to open and close a solenoid valve using a potentiometer.
Our use for this code was to test the opening and closing of the solenoid valve when it's connected to a water source.
*/
int solenoid_Pin = 25;
int pot_Pin = 32;
int val ;  // variable to store the value read

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); //
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(solenoid_Pin, OUTPUT);
  pinMode(led_pin,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  val = analogRead(pot_Pin);  // read the input pin
  if (val > 2048){
  digitalWrite(solenoid_Pin, HIGH);   // Open solenoid, water flows
  }
  else {
  digitalWrite(solenoid_Pin, LOW);    // Close solenoid, water stops
  }
  Serial.println(val);          // debug value
  delay(100);
}
