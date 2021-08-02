//This code reads the weight from the scale every 1/10 of a second and prints it
//Using alpha and beta from your linear equation - the weight should be in grams

#include "HX711.h"

HX711 scale;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 22;
const int LOADCELL_SCK_PIN = 23;
float slope = ; //enter alpha value - slope
float zero = ; // enter beta value - intercept
void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    float grams = slope * (float)reading + zero;
    Serial.println(grams);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(100);
  
}
