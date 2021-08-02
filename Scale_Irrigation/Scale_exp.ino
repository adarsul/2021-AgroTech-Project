#include "HX711.h"
#include <WiFi.h>

#include "ThingSpeak.h"
unsigned long myChannelNumber = ;
const char * myWriteAPIKey = "";

const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword
 
const char* server = "api.thingspeak.com";

WiFiClient client;

HX711 scale;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 22;
const int LOADCELL_SCK_PIN = 23;
const float slope = -0.0098;
float zero;

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
    delay(100);
    }
    else{
      i--;
    }
  }
  average = total / reps;
  Serial.print("Tare success = ");
  Serial.println(average);
  return average;
}

float averageRead (int sec, int rate, float zero){
   int reps = rate * sec;  // amount of times the loop runs
   int count = 0;
   float total=0;
   float grams;
  while (count < reps){
    if (scale.is_ready()) {
    long reading = scale.read();
    grams = slope*(float)reading + zero; // reading to grams conversion
    Serial.print("current: ");
    Serial.println(grams);
    total = total + grams;
    count++;
    delay(1000/rate);
    }
    else{
      Serial.println("oops");
      continue;
    }
  }
    float average = total / count;
   Serial.print("HX711 reading: ");
    Serial.print(average);
    Serial.println(" g");
  }



void setup() {
  
  Serial.begin(57600);
  //--WIFI-------------------------
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

    ThingSpeak.begin(client);
 
  WiFi.begin(ssid, password);
  
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();


  //------------------------------------------------------------------------------------

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
  zero = -570;
}

void loop() {
 float readvalue;
 readvalue = averageRead(15, 10, zero);
  ThingSpeak.setField(1,readvalue);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}
