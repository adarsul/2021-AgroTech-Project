//------------------------------------------------------------------------------------
// Wifi and Thingspeak setup:
#include <WiFi.h>
#include "time.h"

#include "ThingSpeak.h"
unsigned long myChannelNumber = 1412103;
const char * myWriteAPIKey = "YB9T1IGWXRYY17SS";

const char* ssid = "HUJI-guest"; // your wifi SSID name
const char* password = "" ;// wifi pasword

//const char* ssid = "eduroam"; // your wifi SSID name
//const char* password = "gabribarse@huji.ac.il" ;// wifi pasword
 
const char* server = "api.thingspeak.com";

WiFiClient client;

//**************************************************************************************

#include "i2cArduino.h"

SVCS3 vcs;

//**************************************************************************************
//NTP

// Set time
#include <NTPClient.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = 3600*3;  // For UTC +2.00 : 2 * 60 * 60 : 3600
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long delayTime;


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int time_h ; // variable for the hour
int time_m ; // variable for the minute

int moisture_solenoidPin = 28;

float e25;
float ec;
float temp;
float vwc;


//**************************************************************************************
void setup() {
  //------------------------------------------------------------------------------------

  // solenoid setup
  pinMode(moisture_solenoidPin, OUTPUT);
  

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

  // stings for time:
   timeClient.begin();
}



//************************************************************************************** 

void loop() {
    for (int i = 0; i <= 15; i++) {
      timeClient.update();
      time_h = timeClient.getHours(); 
      time_m = timeClient.getMinutes();
      Serial.print("Time:");
      Serial.printf("%d, %d", time_h, time_m);  
      Serial.println();
      
      vcs.newReading(); // start sensor reading
      
      float dat[4]={0,0,0,0}; //getting values one by one
      delay(10);//let sensor read data
      vcs.getData(dat);
      
      Serial.println(F("Soil Moisture"));
      Serial.println("-----");
      
      e25 = dat[0];
      Serial.print("e25");
      Serial.print("=");
      Serial.println(dat[0]);
      
      ec = dat[1];
      Serial.print("ec");
      Serial.print("=");
      Serial.println(dat[1]);
      
      temp = dat[2];
      Serial.print("temp");
      Serial.print("=");
      Serial.println(dat[2]);
      
      vwc = dat[3];
      Serial.print("vwc");
      Serial.print("=");
      Serial.println(dat[3]);

      //solenoid activation
      if ( time_h  == 7 && time_m < 1 ) {
        
        while ( vwc < 90 ){
          digitalWrite(moisture_solenoidPin, HIGH);
          vcs.newReading(); // start sensor reading
          delay(10);//let sensor read data
          float dat[4]={0,0,0,0}; //getting values one by one
          vcs.getData(dat);
          vwc = dat[3];
          
          }
        }
      
      else {
        digitalWrite(moisture_solenoidPin, LOW);
        }
        
      delay(1000);
      
    }
    
    //Thingspeak setup:
    ThingSpeak.setField(1,e25);
    ThingSpeak.setField(2,ec);
    ThingSpeak.setField(3,temp);
    ThingSpeak.setField(4,vwc);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
    Serial.println("uploaded to Thingspeak server....");
  
    client.stop();
   
    Serial.println("Waiting to upload next reading...");
    Serial.println();
}
