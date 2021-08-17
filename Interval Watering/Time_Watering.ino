//------------------------------------------------------------------------------------
//General
const int interval_solenoidPin = 26;

const int WATER_DUR_M =  30; // amount of minutes for watering
const int WATER_DUR_H =  0; //amount of hours for watering
const int HOUR_TRIG = 7; //time for interval watering (hour)
const int MIN_TRIG = 00; //time for interval watering (min)
bool water_open;


// Wifi and Thingspeak setup:
#include <WiFi.h>
#include "time.h"

#include "ThingSpeak.h"
unsigned long myChannelNumber = 0 ;
const char * myWriteAPIKey = "";

const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword

const char* server = "api.thingspeak.com";

WiFiClient client;
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

int finishTimeMin(int trig, int dur){
  int finish_min = trig + dur;
  if (finish_min > 59){
    finish_min -= 60;
  }
  return finish_min;
}

  int finishTimeHour(int trig, int dur){
  int finish_hour = trig + dur;
  if (finish_hour > 23){
    finish_hour -= 24;
  }
  return finish_hour;
}
//**************************************************************************************

void setup() {
  //------------------------------------------------------------------------------------

  // solenoids setup
  pinMode(interval_solenoidPin, OUTPUT);
  
 //------------------------------------------------------------------------------------
  // Wifi and Thingspeak setup:
  Serial.begin(115200);
   
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
  
   timeClient.begin();

int minStop = finishTimeMin(MIN_TRIG, WATER_DUR_M);
int hourStop = finishTimeHour(HOUR_TRIG, WATER_DUR_H);
}
void loop() {
  for (int i = 0; i < 15 ; i++){
      //Get time values
      timeClient.update();
      time_h = timeClient.getHours(); 
      time_m = timeClient.getMinutes();
      
      Serial.print("Time:");
      Serial.printf("%d, %d", time_h, time_m);  
      Serial.println();

      delay(1000);
   
//interval_solenoidPin activation - if time is trigger time and solenoid is closed
   if (time_h  == HOUR_TRIG && time_m == MIN_TRIG && water_open == false) {
      digitalWrite(interval_solenoidPin, HIGH);
      water_open = true;
   } 
   if (time_h == HOUR_TRIG + WATER_DUR_H && time_m >= MIN_TRIG + WATER_DUR_M && water_open == true){
      digitalWrite(interval_solenoidPin, LOW);
      water_open = false;
   }
  }
 //Thingspeak setup:
    ThingSpeak.setField(1,water_open);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
    Serial.println("uploaded to Thingspeak server....");
  
    client.stop();
   
    Serial.println("Waiting to upload next reading...");
    Serial.println();
}
