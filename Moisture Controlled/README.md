A soil moisture watering based system, with data logging using ThingSpeak platform for further data analysis

Equipment used:

* [1528-2003-ND Valve](https://bc-robotics.com/tutorials/controlling-a-solenoid-valve-with-arduino/)
* [Four-channel Arduino Relay](https://arduinogetstarted.com/tutorials/arduino-relay)
* [PM-WCS-3 Sensor](https://tinovi.com/wp-content/uploads/2020/01/PM-WCS-3-I2C.pdf)


In order to determine the system's moisture (VWC) thresholds, we connected the PM-WCS-3 Sensor to a plant that was watered by a readymade irrigation system.

![data_collection](https://user-images.githubusercontent.com/88232723/127740810-655d412a-11b2-47ff-a1f7-7e5107859046.jpeg)


Code for the data collection and cloud uploading can be found [here](https://github.com/adarsul/2021-AgroTech-Project/blob/main/Moisture%20Controlled/data_collection_and_upload.ino).

Thats how the data looked like:

<img width="560" alt="data_collection" src="https://user-images.githubusercontent.com/88232723/127740099-42e496c7-87fc-4e32-a748-e2cde4227e71.png">

Based on the results, We decided to set the min VWC threshold to 64 and the max VWC threshold 90.


