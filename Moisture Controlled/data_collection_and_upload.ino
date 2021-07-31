//**************************************************************************************

//ThingSpeak setup
#include "ThingSpeak.h"
unsigned long myChannelNumber = <ChannelNumber>;
const char * myWriteAPIKey = <APIKey>;
const char* server = "api.thingspeak.com";

//wifi setup
#include <WiFi.h>
const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword

//**************************************************************************************

// soile moisture setup
#include "i2cArduino.h"
SVCS3 vcs;

//**************************************************************************************

void setup() {
  Serial.begin(115200);
  vcs.init(0x63);
  
// Wifi and Thingspeak setup:
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

}

void loop() {
  vcs.newReading(); // start sensor reading
  delay(100); //let sensor read data

//getting values all at one request
  float dat[4]={0,0,0,0};
  vcs.getData(dat);
  
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
     
  delay(15000); // 15 secs interval between each Thingspeak upload
     
  //Thingspeak upload
  ThingSpeak.setField(myChannelNumber,e25);
  ThingSpeak.setField(myChannelNumber,ec);
  ThingSpeak.setField(myChannelNumber,temp);
  ThingSpeak.setField(myChannelNumber,vwc);
  
} 
