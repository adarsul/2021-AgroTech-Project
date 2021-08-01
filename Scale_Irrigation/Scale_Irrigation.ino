#include "HX711.h"
#include <WiFi.h>
#include "ThingSpeak.h"


 
const char* server = "api.thingspeak.com";
float MAX_WEIGHT = 3500; //Set max weight to determine growth
float MIN_WEIGHT = 2500; // Set min weight to trigger watering
float GROWTH; //tracks gain in weight attributed to plant growth
const int WATER_DURATION = 60; // 15 second increments for watering. For 1 liter using 4L per hour watering. 
bool give_water = false; //is irrigation on
int water_count = 0; //amount of loops passed while watering


unsigned long myChannelNumber = ""; //enter channel number
const char * myWriteAPIKey = ""; // enter API write key

const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword
WiFiClient client;

//open solenoid valve
void giveWater(int solenoid){
        digitalWrite(solenoid, HIGH);
          Serial.println("Scale solenoid open");
          give_water = true;
}

void stopWater(int solenoid){
        digitalWrite(solenoid, LOW);
          Serial.println("Scale solenoid open");
          give_water = false;
          water_count = 0;
}

void weightUpdate(float weight){
    float  diff = weight - MAX_WEIGHT;
    GROWTH += diff;
    MIN_WEIGHT += diff; 
    MAX_WEIGHT = weight;
    ThingSpeak.setField(2,GROWTH);
}


HX711 scale;

// HX711 circuit wiring
const int scale_solenoidPin = 27; //pin conneceted to relay
const int LOADCELL_DOUT_PIN = 35; 
const int LOADCELL_SCK_PIN = 32;
const float slope = -0.0098;
float zero = -620;

/*Gets average read during time period for weight.
  secs = how many seconds average be taken
  rate = how many readings per second
  zero = intercept */
float averageRead (int sec, int rate){
   int reps = rate * sec;  // amount of times the loop runs
   int count = 0;
   float sum, weight;
  while (count < reps){
    if (scale.is_ready()) {
    long reading = scale.read();
    grams = slope*(float)reading + zero; // reading to grams conversion
    //Serial.print("current: ");
    //Serial.println(grams);
    sum += weight;
    count++;
    delay(1000/rate);
    }
    else{
      continue;
    }
  }
    float average = weight / reps;
   Serial.print("HX711 reading: ");
    Serial.print(average);
    Serial.println(" g");
    return average; 
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
 
  // default settings
  Serial.println("-- Default Test --");
  Serial.println();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
}

void loop() {
  float weight = averageRead(15, 10); //average weight read for time period
  
  //when irrigation is on, count loops
  if (give_water == true){                  
    water_count++;
  }

  //Irrigation Activation
  if (weight < MIN_WEIGHT && give_water == false){ 
    giveWater(scale_solenoidPin);
  }
  
  //Irrigation stop
  if (water_count > WATER_DURATION){
    stopWater(scale_solenoidPin);
  }

  //Check for growth
  if (weight > MAX_WEIGHT){
    weightUpdate(weight);
    ThingSpeak.setField(2,GROWTH);
  }

  //------------------------------------------------------------------------------------
  //Thingspeak setup:  
  ThingSpeak.setField(1,weight);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}
