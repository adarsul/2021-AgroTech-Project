# Weight Controlled Irrigation

## Our Goal
In this project we created a weight controlled irrigation system - pot weight is monitored to indicate it's water content, triggering irrigation when minimum threshold is reached. Since the plant grows and the weight will constantly become higher, the system will track growth and update it's thresholds automatically.

##  Parts

* ESP32 microcontroller
* Full size breadboard
* Jumper Wires
* 20kg load cell scale and HX711 amplifier
* 4 channel arduino relay
* 1528-2003-ND Solenoid Valve

## Code Explanation

### Main Premise
The program constantly collects data of the pot's weight and sends it to thingspeak channel every 15 seconds. If the weight is lower than a user set minimum threshold - irrigation is a activated. If after Irrigation the weight of the pot is higher than the last recorded (or user input) maximum weight, then maximum weight, minimum threshold are updated and growth is tracked.


The libraries that were used:

Wifi.h allows internet connection

Thingspeak.h allows I2C communication

HX711.h scale driver


### General

#### User Variables:

**MAX_WEIGHT**: the maximum weight which refers to the pot weight after watering. This value is used as refrence to asses plant growth in the total pot weight.

**MIN_WEIGHT**: the minimum weight threshold. If the weight readind is lower that this value - irrigation is intiated.

**WATER_DURATION**: this is value refers to the amount of time the irrigation is open upon activation. The program loops every 15 second therefore the water duration is measured in 15 seconds increments.

Say you wanted the system to give 1 liter per watering and you are using a 4 liter per hour watering sprinkler.
You set Maximum weight to the weight of the plant after watering, minimum weight to that value and subtract 1000 grams. For 1 liter watering the solenoid needts to open for 15 minutes which is 900 seconds. because the loop runs every 15 seconds, we will set the water duration to 60 because:

900 (seconds) : 15 (seconds) = 60 (loops).

#### Other useful Variables:

**GROWTH**: every time the max weight is updated, the differnece is added to the growth variable. Assuming that the changes in weight due the watering are fixed and that soil and pot weight are constant, the growth in weight is explained by the growth of the plant.

**give_water**: a boolean that is true when solenoid is open and false when it is closed.

**water_count**: tracks how many loops (15 second increments) the solenoid is open.

#### Scale Calibration
HX711 translates force to Electrical output values. In order to get readings in grams calibration was needed. We used a linear regression in order to match the output value to a reading in grams. In this code, the conversion is done within the averageRead function.

For more information about calibration:


#### averageRead function
Initially when we got proper readings in grams, the values tended to flunctuate over a big range of values (compared to the subject weight). In order to flatten these flunctuations we created the averageRead function.
The function recieves 2 arguments: sample time in seconds and sampe rate in time per second.
In our case, because the thingspeak server allows for communication every 15 seconds, we set the seconds to 15 and rate to 10. The meaning of that is that the instrument will take 150 samples in 15 seconds and return the average of those values. This function helped greatly to decrease flunctuations*. 

*Another step to help reduce flunctuations was to use heavier subject - over 2kg.

#### Watering algorithm

* When reading is less than MIN_WEIGHT the solenoid is opened and water starts flowing, give_water is turned to true. 
* Every iteration of main loop, if give_water is true (meaning soleniod is open) water_count adds one.
* If water_count is greater than WATER_DURATION then the watering reached it's limit time - solenoid is closed, give_water is set to false and water count is set to 0. 
* If the measured weight is greater than MAX_WEIGHT then the weightUpdate function is triggered - updating MAX_WEIGHT and MIN_WEIGHT and adding the differnce to the GROWTH variable.
* Send data to thingspeak channel.

For the data analysis we used for the watering algorithm [click here](https://github.com/adarsul/2021-AgroTech-Project/blob/main/Scale_Irrigation/Watering%20Algorithm/Watering%20algorithm.ipynb)
