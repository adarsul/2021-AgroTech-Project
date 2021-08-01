float tareIt (int sec){
  Serial.println("Tare start...");
  float total, zero, average;
  int reps = sec * 10;
  delay(1000);
  for (int i = 0; i < reps ; i++){
    if (scale.is_ready()) {
    long reading = scale.read();
    zero = 0 - (slope*(float)reading);
    total = total + zero;
    Serial.println("good");
    delay(100);
    }
    else{
      i--;
      Serial.println("bad");
    }
  }
  average = total / reps;
  Serial.print("Tare success = ");
  Serial.println(average);
  return average;
}
