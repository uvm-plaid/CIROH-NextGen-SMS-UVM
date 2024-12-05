#include <Wire.h> //allows you to communitcate with I2C/TWI devices
#include "Sodaq_DS3231.h" //library for the DS3231 RTC (Real Time Clock)
#include "Adafruit_SHTC3.h" //library for the SHTC3 temperature & humidity sensor

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2023, 11, 07, 16, 07, 0, 2);

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600); //Sets the data rate in bits per second (baud) for serial data transmission
  Wire.begin(); //This function initializes the Wire library and join the I2C bus as a controller or a peripheral (QU: This means that it adds teh arduino into the bus correct? Does it add the temperature sensors, etc. onto the bus?)
  rtc.begin(); //RTCZero-begin(): Initializes the internal RTC. Needs to be called before any other RTC library methods.
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 

  //Code from the SHTC3 example, found at https://learn.adafruit.com/adafruit-sensirion-shtc3-temperature-humidity-sensor/arduino
  Serial.println("SHTC3 test");

  //shtc3.begin
  if (! shtc3.begin()) { 
    Serial.println("Couldn't find SHTC3");
    while (1) delay(1); //QU: This is from the example code-why is it delaying indefinitely? Should it be checking to try to find the shtc3 again?
  }
  Serial.println("Found SHTC3 sensor");
}

void loop() {
  // put your main code here, to run repeatedly: (Control loop)

  //The code below shows how to get the date and time 
  DateTime now = rtc.now(); //get the current date-time
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.date(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print(weekDay[now.dayOfWeek()]);
  Serial.println();

  //TODO: Is there an integer value for seconds since December 12, 1971? Is this what the library is doing? Unix timestamp?
  //For outputting to log files, we would want to use the Unix timestamp

  //TODO: Work on reading & writing to nonvolatile memory (SD card on the Arduino)

  //Code for reading in temp and RH from the SHTC3 example, found at https://learn.adafruit.com/adafruit-sensirion-shtc3-temperature-humidity-sensor/arduino

  sensors_event_t humidity, temp;
  
  shtc3.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
 
  //The below code will be uncommented once temp and RH can be received through the sensor, to then do the dew point calculation
  int temperature = temp.temperature;
  int RH = humidity.relative_humidity;
  int dewPoint = temperature - ((100 - RH) / 5); 

  Serial.print("Dew Point: ");   Serial.println(dewPoint);

  //example if humidity < 50%, don't do anythiing. If humidity > 50%, check dew point. If dew point <70%, don't do anything. If dew point > 70%, check if it's raining
  delay(1000);

}
