/*
  Reading CO2 from the SCD30, and outputting the readings to a Screen and LED
  By: R Robinson & H Timmermann
  LJCDS
  Date: Dec 26, 2021
  This example prints the current CO2 level (in PPM) to an LCD screen, and lights up an LED conditionally based on the reading.
*/

/*
  We begin by importing the three libraries we will be using.
  LCDIC2 is for communicating and controlling the LCD screen
  Wire is the basic Arduino library for communicating down a wire (from arduino to sensor, or vice versa)
  Sparkfun SCD30 is the library that handles the SCD30 CO2 sensor for us (http://librarymanager/All#SparkFun_SCD30)
*/

#include "LCDIC2.h"
#include "Wire.h"
#include "SparkFun_SCD30_Arduino_Library.h"
#include <SD.h>
#include <DS1307RTC.h>

/*
  Next we create two objects, lcd and airsensor, which tells the arduino the following:
  LCDIC2 - The address (0x27) of the screen (don't worry too much about this), the length of the screen (16 chars), and how many rows the screen has (2).
  SCD30 - This one takes no inputs, just tells the arduino we have the airsensor (SCD30) attached! :)
*/

LCDIC2 lcd(0x27, 16, 2);
SCD30 airSensor;

// Next up, we define a few variables for later use.

float co2_reading = 0.0;  // This will hold the CO2 reading we get from the Sensor
float temp = 0;           // This will hold the Temp reading (in C) we get from the sensor (not used in this)
float humidity = 0;       // This will hold the himidity reading (again, not used in this)
float co2_sum = 0;
float co2_avg = 0;
int times_to_read = 10;   // This will control how many readings we take before averaging the data.

int redLED = 2;           // These variables tell the Arduino which pins each colored LED is on.
int greenLED = 4;         // Make sure you plug them in to the right slots, or the colors might be wrong!
int blueLED = 6;

String co2_reading_str;   // An extra variable so we can store the CO2 reading as a string, instead of as a number (for printing purposes)

File myFile;              //used later; declares the file 

void setup()               // Setup is ran *ONCE* when the arduino first recieves power
{
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!"); //10 is the digital pin it is in; serial print isn't needed but tells you if it's working
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("co2data.txt", FILE_WRITE); //opens the file and kinda declares it for the rest of the code; not entirely sure if this is even needed
  Serial.println("SCD30 CO2 Read");
  Wire.begin();
 
  delay(3000);
  if (lcd.begin()) lcd.print("CO2 Reading");
  lcd.setCursor(8, 1);
  lcd.print("PPM");

  if (airSensor.begin() == false)     //This line checks if the sensor is *not* available.
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);                        // If it isn't available, throw out this error and lock up!
  }
 
  pinMode(redLED, OUTPUT);            //these pins are for the LED - 2 is red, 4 is green
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
}



void loop()                           // Loop is ran INFINITELY until power is turned off.
{
  co2_sum = 0;
  for (int i = 0; i < times_to_read; i++) { // Take as many readings as we definied up above.
    if (airSensor.dataAvailable())
    {
      Serial.print("co2(ppm):");
      Serial.print(airSensor.getCO2());
      Serial.println();
      co2_reading = int(airSensor.getCO2());
    }
    co2_sum += co2_reading;          // Add each new reading to our sum of readings
    delay(1000);
  }
 
  co2_avg = co2_sum / times_to_read; // Get an average by dividing Sum / Count
  co2_avg = int(co2_avg);
  co2_reading_str = String(co2_avg); // Convert number to a string for printing to screen
 
  lcd.setCursor(0, 1);               // Set location on screen to print at (Char 0, row 1)
  lcd.print(co2_reading_str);        // Write to the screen!
  tmElements_t tm;
  if (RTC.read(tm)) {
    myFile = SD.open("co2data.txt", FILE_WRITE);      //This is used to write the CO2 data to the SD card; specifically co2_reading_str
      myFile.print(co2_reading_str);
      myFile.print(" ");
      myFile.print(tm.Hour);
      myFile.print(":");
      myFile.print(tm.Minute);
      myFile.print(":");
      myFile.print(tm.Second);
      myFile.print(" ");
      myFile.print(tm.Day);
      myFile.print("/");
      myFile.print(tm.Month);
      myFile.print("/");
      myFile.print(tmYearToCalendar(tm.Year));
      myFile.println();
      myFile.close();   //needs to close every time; not really sure why but, you gotta go with the flow sometimes
  }
    if (co2_avg > 1000){            // This will make the LED red when the CO2 levels are above 1000 PPM
      digitalWrite(greenLED,LOW);
      digitalWrite(blueLED,LOW);
      digitalWrite(redLED,HIGH);
    }
    else if (co2_avg > 800){        // This will make the LED yellow (green + red) when levels are above 800, but below 1000
      digitalWrite(greenLED,HIGH);
      digitalWrite(redLED,HIGH);
      digitalWrite(blueLED, LOW);
    }
    else if (co2_avg == 0){         // This will make the LED BLUE if it reads 0 (which usually means calibrating or an error)
      digitalWrite(greenLED,LOW);
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED, HIGH);      
    }
    else{                           // If no other conditions are met, PPM is safely below 800 but above 0, and the LED is lit green
      digitalWrite(greenLED,HIGH);
      digitalWrite(redLED,LOW);
      digitalWrite(blueLED,LOW);
  
    }
  }

