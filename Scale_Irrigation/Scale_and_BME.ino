#include "HX711.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "ThingSpeak.h"
unsigned long myChannelNumber = 1434967;
const char * myWriteAPIKey = "W97NYDXHRRPM0KDC";

const char* ssid = "sulima"; // your wifi SSID name
const char* password = "waysler12345" ;// wifi pasword
 
const char* server = "api.thingspeak.com";

WiFiClient client;

//BME280
#define SEALEVELPRESSURE_HPA (1007)
Adafruit_BME280 bme; 
/*float calc_VPD(float humidity, float pressure){
  
}*/


void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}


HX711 scale;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 35;
const int LOADCELL_SCK_PIN = 32;
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

float* averageRead (int sec, int rate, float zero){
   int reps = rate * sec;  // amount of times the loop runs
   int count = 0;
   float weight, temp, humidity;
   float grams;
  while (count < reps){
    if (scale.is_ready()) {
    long reading = scale.read();
    grams = slope*(float)reading + zero; // reading to grams conversion
    //Serial.print("current: ");
    //Serial.println(grams);
    weight = weight + grams;
    temp = temp + bme.readTemperature();
    humidity = humidity + bme.readHumidity(); 
    count++;
    delay(1000/rate);
    }
    else{
      Serial.println("oops");
      continue;
    }
  }
    static float average[3];
    average[0] = weight / count;
    average[1] = temp / count;
    average[2] = humidity / count;

   Serial.print("HX711 reading: ");
    Serial.print(average[0]);
    Serial.println(" g");
    Serial.print("Temp: ");
    Serial.print(average[1]);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(average[2]);
    Serial.println("%");
   /* Serial.print("max:");
    Serial.println(maxVal);
    Serial.print("min: ");
    Serial.println(min_val);
    Serial.print("middle: ");
    Serial.println(middle);
    Serial.println("");
    Serial.print("average: ");
    Serial.println(average);
    Serial.println(""); */
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
 bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  Serial.println();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
  zero = -620;
}

void loop() {
  float temp, humidity,SVP,VPD;
  float *readvalue;
  readvalue = averageRead(15, 10, zero);
  temp = readvalue[1];
  humidity = readvalue[2];
  Serial.println(temp);
  Serial.println(humidity);
  SVP = 0.61078*exp((temp/(temp+238.3))*17.2694);
  VPD = SVP*(1-(humidity/100));
  //------------------------------------------------------------------------------------
  //Thingspeak setup:  
  ThingSpeak.setField(1,readvalue[0]);
  ThingSpeak.setField(2,temp);
  ThingSpeak.setField(3,humidity);
  ThingSpeak.setField(4,(bme.readPressure() / 100.0F));
  ThingSpeak.setField(5,(bme.readAltitude(SEALEVELPRESSURE_HPA)));
  ThingSpeak.setField(6,VPD);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}
