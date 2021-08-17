# Interval Watering

For this method you will need:

- ESP32 microcontroller
- Fullsize Bread Board
- Relay
- Solenoid Valve
- Jumper cables

We have made two versions:

### Simple Interval

This version is a completely open loop. The user inputs desired watering time in minutes and desired interval length in hours. The program opens the solenoid, waits, closes the solenoid and then waits again.

[The Code](https://github.com/adarsul/2021-AgroTech-Project/blob/main/Interval%20Watering/Interval_Watering.ino)

### Time Watering

This version utilizes NTP to check the time. User sets time for watering and watering duration. The code constantly checks the time and when it is watering time the solenoid opens for set time and then closes. Every 15 seconds the system sends to Thingspeak channel a boolean value indicating whether the watering is on or off.

The main advantages of this version are:
- The ability to track watering using I2C.
- System could be restarted and keep interval.
- System automatically adjusts to daylight savings.

[The Code](https://github.com/adarsul/2021-AgroTech-Project/blob/main/Interval%20Watering/Time_Watering.ino)
