/*This program is a unified version of our 3 projects:
 * Time Interval Watering - waters plants every day in the same time
 * Soil Moisture Sensor Watering - watering is triggerd by soil humidity going below threshold
 * Weight Sensor Watering - watering is triggered by weight going below threshold
 * 
 * Main premise: 
 * data is collected in a loop every 15 seconds
 * if watering is triggered - solenoid is opened and counter is counting (every 15 second loop)
 * if close watering is triggered - solenoid is closed and counter is set back to 0
 * in the end of each loop data is sent to thingspeak
 */


//------------------------------------------------------------------------------------
//General
const int interval_solenoidPin = 26;
const int moisture_solenoidPin = 25;
const int scale_solenoidPin = 27;

const int INTERVAL_DUR = 5; //15 seconds increments of watering for interval watering
const int MOISTURE_DUR = 5; //15 seconds increments of watering for soil sensor
const int SCALE_DUR = 5;    //15 seconds increments of watering for scale
const int HOUR_TRIG = 7; //time for interval watering (hour)
const int MIN_TRIG = 00; //time for interval watering (min)
const int MIN_MOISTURE = 64; //mint threshold for soil sensor
const int MAX_MOISTURE = 90; //max threshold for soil sensor
float MAX_WEIGHT = 3700; //max weight with water
float MIN_WEIGHT = 3200; //min weight threshold
float GROWTH; // tracks gains in weight

int interval_count, moisture_count, scale_count; //counters for watering
bool interval_open, moisture_open, scale_open; //track when solenoid is open

//open solenoid valve
void giveWater(int solenoid, int count){
        digitalWrite(solenoid, HIGH);
        switch(solenoid){
        case scale_solenoidPin:
          Serial.println("Scale solenoid open");
          scale_open = true;
          break;
        case moisture_solenoidPin:
          Serial.println("Moisture solenoid open");
          moisture_open = true;
          break;
        case interval_solenoidPin:
          Serial.println("Interval solenoid open");
          interval_open = true;
          break;
        }
}

//close solenoid valve, set back to 0
void stopWater(int solenoid, int count){
        digitalWrite(solenoid, LOW);
         count = 0;
        switch(solenoid){
        case scale_solenoidPin:
          Serial.println("Scale solenoid closed");
          scale_open = false;
          break;
        case moisture_solenoidPin:
          Serial.println("Moisture solenoid closed");
          moisture_open = false;
          break;
        case interval_solenoidPin:
          Serial.println("Interval solenoid closed");
          interval_open = false;
          break;
        }
  }

//raise count for open solenoids
void counter(){
  
  if (interval_open == true){
    interval_count++;
  }
  
  if (moisture_open == true){
    moisture_count++;
  }
  
  if (scale_open == true){
    scale_count++;
  }
}

// Wifi and Thingspeak setup:
#include <WiFi.h>
#include "time.h"
#include "ThingSpeak.h"
unsigned long myChannelNumber = "" ; //your channel ID
const char * myWriteAPIKey = ""; //your channel's write API key

const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword


 
const char* server = "api.thingspeak.com";

WiFiClient client;

//------------------------------------------------------------------------------------
//Moisture Sensor Setup
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "i2cArduino.h"
SVCS3 vcs;

float e25, ec, soil_temp, vwc;

//Prints the valuse from the soil moisture sensor data array
void printSensor(float dat[4]){
      Serial.println("Soil Sensor:");
      Serial.print("e25 = ");
      Serial.println(dat[0]);
      Serial.print("ec = ");
      Serial.println(dat[1]);
      Serial.print("temp = ");
      Serial.println(dat[2]);
      Serial.print("vwc = ");
      Serial.println(dat[3]);
      Serial.println("------------------");
      }
//------------------------------------------------------------------------------------
//Interval Watering Setup
//NTP

// Set time
#include <NTPClient.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = 3600*3;  // For UTC +00 : 2 * 60 * 60 : 3600

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int time_h,time_m; // variables for the hour and for minutes

//------------------------------------------------------------------------------------
//Scale Setup
#include "HX711.h"
const float zero = -620;
HX711 scale;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 35;
const int LOADCELL_SCK_PIN = 32;
const float slope = -0.0098;

void printSensor(float weight){
        Serial.print("HX711 reading: ");
        Serial.print(weight);
        Serial.println(" g");
}

//get 15 sec average weight to improve accuracy
float getScaleValue(float weightarr[]){
  float weight_sum, average;
  for (int i = 0; i < 15 ; i++){
    weight_sum += weightarr[i];
  }
  average = weight_sum / 15;
  Serial.print("Average: ");
  Serial.println(average);
  return average;
}

//update weight data due to growth in mass
void weightUpdate(float weight){
    float  diff = weight - MAX_WEIGHT;
    GROWTH += diff;
    MIN_WEIGHT += diff; 
    MAX_WEIGHT = weight;
    ThingSpeak.setField(6,GROWTH);
}

//**************************************************************************************

void setup() {
  //------------------------------------------------------------------------------------

  // solenoids setup
  pinMode(interval_solenoidPin, OUTPUT);
  pinMode(scale_solenoidPin, OUTPUT);
  pinMode(moisture_solenoidPin, OUTPUT);
  
 //------------------------------------------------------------------------------------
  // Wifi and Thingspeak setup:
  Serial.begin(115200);
  
  vcs.init(0x63);
  
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
// settings for time:
   timeClient.begin();

//settings fo soil sensor
vcs.init(0x63);

//settings for scale  
  Serial.println("-- Default Test --");
  Serial.println();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
}

void loop() {

  float weight;
  float weight_arr[15];
   for (int i = 0; i < 15; i++) {
      //Get time values
      timeClient.update();
      time_h = timeClient.getHours(); 
      time_m = timeClient.getMinutes();
      
      Serial.print("Time:");
      Serial.printf("%d, %d", time_h, time_m);  
      Serial.println();

      //Get soil sensor values
      vcs.newReading();// start sensor reading
      delay(100);
      float dat[4]={0,0,0,0}; //getting values one by one
      vcs.getData(dat);
      e25 = dat[0];
      ec = dat[1];
      soil_temp = dat[2];
      vwc = dat[3];

      //Get scale values
      if (scale.is_ready()) {
        long reading = scale.read();
        weight = slope*(float)reading + zero;
        weight_arr[i] = weight;
        printSensor(weight); 
      }
      else{
      Serial.println("No reading");
      i--;
      }
      delay(1000);
   }
   
   float average = getScaleValue(weight_arr);
   
//interval_solenoidPin activation - if time is trigger time and solenoid is closed
   if (time_h  == HOUR_TRIG && time_m == MIN_TRIG && interval_open == false) {
      giveWater(interval_solenoidPin, interval_count);
   } 
   if (interval_count > INTERVAL_DUR){
      stopWater(interval_solenoidPin, interval_count);
   }
 //moisture_solenoidPin activation - if moisture is below minimum trigger and solenoid is closed
   if ( vwc < MIN_MOISTURE && moisture_open == false) {
      giveWater(moisture_solenoidPin, moisture_count);
   }
   if (vwc > MAX_MOISTURE || moisture_count > MOISTURE_DUR){
      stopWater(moisture_solenoidPin, moisture_count);
   }
//scale solenoid activation - if weight is below minimum trigger and solenoid is closed
   if (average < MIN_WEIGHT && scale_open == false){
      giveWater(scale_solenoidPin, scale_count);
   }
   if (scale_count > SCALE_DUR){ 
      stopWater(scale_solenoidPin, scale_count);
      //if after watering weight is above max - update triggers and report growth
      if (average > MAX_WEIGHT){
          weightUpdate(average);
      }
   }
   counter(); //if a solenoid is open update counter
   
 //Thingspeak setup:
    ThingSpeak.setField(1,weight);
    ThingSpeak.setField(2,vwc);
    ThingSpeak.setField(3,soil_temp);
    ThingSpeak.setField(4,interval_open);
    ThingSpeak.setField(5,moisture_open);
    ThingSpeak.setField(6,scale_open);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
    Serial.println("uploaded to Thingspeak server....");
  
    client.stop();
   
    Serial.println("Waiting to upload next reading...");
    Serial.println();
}
