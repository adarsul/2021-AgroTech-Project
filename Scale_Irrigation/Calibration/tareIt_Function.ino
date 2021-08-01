//This function returns the intercept value using an average value from a time frame
float slope = -0.0098;

float tareIt (int sec){
  Serial.println("Tare start...");
  float total, zero, average;
  int reps = sec * 10;  //10 readings per second
  delay(1000);
  for (int i = 0; i < reps ; i++){
    if (scale.is_ready()) {
    long reading = scale.read();
    zero = 0 - (slope*(float)reading);
    total = total + zero;
    delay(100);
    }
    else{   //If no reading then repeat loop
      i--;
    }
  }
  average = total / reps;
  Serial.print("Tare success = ");
  Serial.println(average);
  return average;
}
