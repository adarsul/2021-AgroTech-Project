#include "HX711.h"

HX711 scale;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 22;
const int LOADCELL_SCK_PIN = 23;
void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(100);
  
}
