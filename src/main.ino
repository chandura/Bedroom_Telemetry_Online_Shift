#include <Arduino.h>

//#include<MFSSegControl.h>

// Include the libraries that are required for the temp and humidity sensor
#include <dht.h>
dht DHT;

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

// Switches to switch on various special modes
boolean verbose = 0;  //Switch this on (1) if you want to debugging prompts to be output
boolean setTheTime = false;  //Switch this on (1) if you want to reset the time on the RTC at restart.
int debug = 0;        //A value of 0 swithces off all debugging
                      //A value of 1 swicthes on the debugging of the help message
                      //A value of 2 switches on the Summer Time debugging
                      //A value of 3 switches on the Temp overrun debugging
                      //A value of 4 switches on the day and night debugging 

int Brightness = 500;
//#define DISPLAY_BRIGHTNESS  500  // Define the brightness of the 7 Seg LEDs. The higher the number the brighter the lights 2000 - 0
//int DISPLAY_BRIGHTNESS = 500;  // Define the brightness of the 7 Seg LEDs. The higher the number the brighter the lights 2000 - 0

// Set up the initial variables associated with the time functions
char GMT = 'Y';              // Start the programme assuming that the time on the RTC is set to GMT
                             // This assumes that the PC being used to set the time is running BST
char temp_read = 'N';        // Start with this value set to N so the temp will be read next time
int theyear;                 // The value of the year in the current time
int themonth;                // The value of the month in the current time
int theday;                  // The value of the day in the current time
int monthday;                // The day of the month in the current time
int hours;                   // The value of the hours in the current time 
int minutes;                 // The value of the minutes in the current time
int seconds;                 // The value of the seconds in the current time
int morningon;               // Holds the time in the morning that the lights should be turned on.
int nightoff = 2235;         // Holds the time im the evening that the lights should be turned off.
int testday;                 // Hold details of the day value to test.  Used to help determine of Daylight Saving should be applied
String timeString;           // Holds the time as a string for display on the 7 segment display

// Set the pins that will be used to control the LED segments for the clock
const char digit1 = A0;      //Digit 1
const char digit2 = A1;      //Digit 2
const char digit3 = A2;      //Digit 3
const char digit4 = A3;      //Digit 4

// Declare the variables that are required to get the time
#define YES 1;
#define NO 0;

//Define the DS3231 as rtc - the Real Time Clock
RTC_DS3231 rtc;

//Set up for the shift register
int latchPin = 8;            // Pin connected to ST_CP of 74HC595
int clockPin = 7;            // Pin connected to SH_CP of 74HC595
int dataPin = 6;             // Pin connected to DS of 74HC595

//Set up the initial variables associated with the tempreture functions
float input_voltage = 4.45;  // Set the value used to calculate the voltage for the temprature reading
char lights_on = 'Y';        // Start the programme assuming that the thermometer lights should be on
int delayOffset = 1;         // Set the delay offset value to 1
int red = 0;                 // Set the starting value of the Red LEDs to 0 - off
int green = 0;               // Set the starting value of the Green LEDs to 0 - off
int blue = 0;                // Set the starting value of the Blue LEDs to 0 - off
int green_adjust = 0;        // The variable used to control the intensity of the LEDs when there is a temprature overrun.
int over_run_temp = 26;      // Set the value at which the temprature goes out of the upper range  

//Declare everthing required to measure the temprature
const char temp = A6;        // Input for the input from the temp sensor
int Count;                   // Declare the counter to read the temp sensor many times
#define Count 1000           // The sensor will sample the temp 1000 times for accuracy
float delayCount = 0;        // Set up a counter to delay the reset of the Arduino if it is over temprature
float temperature;           // Holds the calculated tempreture
int inttemp;                 // Use to convert the calculated temprature into an integer for display
String displaytemp;          // Convert the integer version of the temprature to a string to be dislayed on the 7 segment LED
long SensorAverage = 0;      // Hold an average sensor value over a number of cycles to improve accuracy
int sensorVal = 0;           // Hold the value measured at the temprature sensor
float voltage = 0;           // Hold the value of the voltage calculated from the temprature reading

// Initilize thw Neo pixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

#define DHT11_PIN 9           // Initialize DHT sensor on PIN 9
#define DHT11_POWER 5         // Define a pin to power 

void setup() {
    // put your setup code here, to run once:
    // Open up the commnications channels
    Serial.begin(9600);      // Open the serial port, for communicating to the terminal monitor
    Wire.begin();            // Open the interface for the rtc
    delay(3000);             // Give the inteface to the rtc chance to open

    Serial.println("Serial communicatons are open");  // Confirm that the channels are opened
    Serial.print("GMT has been initilised as ");      
    Serial.println (GMT);                             // Confirm the setting of the GMT indicator
    Serial.print("Debugging mode is set to ");
    Serial.println(debug);

    if (! rtc.begin()) {                    // Need to make a change to set the LED to Help if the RTC can't be found
        Serial.println("Couldn't find RTC");  // Confirm that the rtc could not be found 
        displayHelp();                        // Cry for help, no rtc can be found
        while (1);                            // Loop forever, what else can I do.  I'm a clock with no time
    }
    else {
        Serial.println("Found the rtc");      // Confirm that the rtc has been found, all is good
    }

    if (setTheTime) {                         // Reset the time on the RTC if requested
        setrtcTime();                         // Request to reset the time received
    }

    // Set the relevent pins for to control the LED segments - confirm that this is still required or has now been replaced with the Shoft register
    for (int pinNumber = 3; pinNumber <13; pinNumber++)
    {
      pinMode(pinNumber, OUTPUT);
    }
  
    // Set the relevant pins to control the LED digits - confirm that this is still required or has now been replaced with the Shoft register
    pinMode(digit1, OUTPUT); //Digit 1
    pinMode(digit2, OUTPUT); //Digit 2
    pinMode(digit3, OUTPUT); //Digit 3
    pinMode(digit4, OUTPUT); //Digit 4
    
    pinMode(DHT11_POWER, OUTPUT);     // Set the relevant pin to power the DHT11.  So that it can be switched off
    digitalWrite(DHT11_POWER, HIGH);  // Switch on the temp sensor
    pinMode(temp, INPUT);             // Set the relevant pin to read the temprature
    writeTemp();                      // Why do this, there is no temp set yet?


    strip.begin();            // Initilise the communications to the LED strip
    strip.setBrightness(6);   // Set the brightness at which to display the LEDs
    strip.show();             // Initialize all pixels to 'off'

    //Turn off all the segments of the display
    off();

    getthertcTime(); //Set the time from the RTC
    summertime();    //Test for British Summer Time

    //Here on 23/02/2018

}

void loop() {
    
  night_and_day();                    // Check whether the clock should be in night or day mode 
  
  // Display the temprature when the time is between 30 and 43 seconds past the minute, otherwise show the time

  if (lights_on == 'N') {              
    // We are in the lights off mode.  There is no need to check or display the temprature during this time 
    timeString = getthertcTime();      // Get the latest time from the rtc, write it to the time sring for display 
    displayTime(timeString);           // Dsplay the time obtained from the rtc
    if (debug==3) {
      temperature = getTemp();           // Get the current temprature from from the sensor  
    }
  }
  else {
    if (seconds < 30 || seconds > 45){ 
      // We are in the lights on mode.  The temprature should be checked and displayed during this time 
      timeString = getthertcTime();    // Get the latest time from the rtc, write it to the time sring for display 
      displayTime(timeString);         // Dsplay the time obtained from the rtc
      temp_read = 'N';                  // Make sure that the temp wi;; be read next time it needs to be
    }
    else {
      //temperature = getTemp();         // Get the current temprature from from the sensor
      if (temp_read == 'N'){             // Read the temprature once for this cycle
        Serial.println("Go get the temp now");
        temperature = measure_DHT_values();
        writeTemp();                     // Write the temprature out to the serial monitor 
      }
      else {
        inttemp = int(temperature);      // Convert it to an integer ready for display on the 7 segment LED
        displaytemp = String(inttemp);   // Move the ingerer value into a string to be displayed on the 7 segment LED
        displayTemp(displaytemp);        // Call the routine to display the temprature on the 7 segment LED   
        timeString = getthertcTime();    // Get the latest time from the rtc, write it to the time sring for display
      }
    }
  }
}

/////////////////////////////
//Code from the display tab//
/////////////////////////////

void displayHelp() {

  //Display 'Help' when the clock is on trouble
  //Passed in parameter is the time determed in RTC_Control 

  debug_help ();
  
  #define DISPLAY_BRIGHTNESS  500                                // Define the brightness of the LEDs.  Consider moving out of here for global brightness control.

  long beginTime = millis();                                     // Set 'beginTime' based on the number of miliseconds that the Arduino has been running
  
  for(int digit = 4 ; digit > 0 ; digit--) {                     // Set up a loop to loop around the 4 digits of the display.  Starting with digit 4 of the display
    switch(digit) {
    case 1:
      digitalWrite(digit1, LOW);                                 // Set the left most digit to 'H'
      aitch();
      break;
    case 2:
      digitalWrite(digit2, LOW);                                 // Set the second left digit to 'e'
      eee();
      break;
    case 3:
      digitalWrite(digit3, LOW);                                 // Set the second right most digit to 'l'
      ell();
      break;
    case 4:
      digitalWrite(digit4, LOW);                                 // Set the right most digit to 'p'
      pee();
      break;
    }
    
    delayMicroseconds(DISPLAY_BRIGHTNESS);                       // Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off();                                                       // Switch off all of the segments on the LED

    //Turn off all digits                                        // This should have been retired now with the above.  It can be reinstated if required
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ;                         // Wait for 20ms to pass before we paint the display again
}

void displayTime(String toDisplay) {
  
  //Display the time on the 7 Segment LED
  //Passed in parameter is the time determed in RTC_Control 
  
  //#define DISPLAY_BRIGHTNESS  500                                // Define the brightness of the LEDs.  Consider moving out of here for global brightness control.  

  long beginTime = millis();                                     // Set 'beginTime' based on the number of miliseconds that the Arduino has been running

  String displayString = String(toDisplay);                      // Set the string to display to that passed in from the RTC 
  
  for(int digit = 4 ; digit > 0 ; digit--) {                     // Set up a loop to loop around the 4 digits of the display.  Starting with digit 4 of the display

    int minusOne = digit -1;                                     // Set up minusOne to one less then the count.  This will be used to set the beging and end of the substring below
    String theDigit = displayString.substring(minusOne, digit);  // Get the value of the first digit to display, from the passed in value
    
    int displayDigit = theDigit.toInt();                         // Convert the digit to be displayed to an integer
    //Turn on a digit for a short amount of time
    switch(digit) {                                              // Based on the count in the loop light the corresponding digit
    case 1:                                                      
      digitalWrite(digit1, LOW);                                 // Display the right digit to the first in the time string
      lightNumber(displayDigit);
      break;
    case 2:
      digitalWrite(digit2, LOW);                                 // Display the second right digit to the second in the time string
      lightNumber(displayDigit);
      break;
    case 3:
      digitalWrite(digit3, LOW);                                 // Display the second left digit to the third in the time string
      lightNumber(displayDigit);
      break;
    case 4:
      digitalWrite(digit4, LOW);                                 // Display the right digit to the forth in the time string
      lightNumber(displayDigit);
      break;
    }
    
    //delayMicroseconds(DISPLAY_BRIGHTNESS);                       // Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)
    delayMicroseconds(Brightness);                         // Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off();                                                       // Switch off all of the segments on the LED

    //Turn off all digits                                        // This should have been retired now with the above.  It can be reinstated if required
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ;                          // Wait for 20ms to pass before we paint the display again
}

void displayTemp(String toDisplay) {

  //Display the temp on the 7 Segment LED
  //Passed in parameter is the time determed in Temp_Sync 

  //#define DISPLAY_BRIGHTNESS  500                                // Define the brightness of the LEDs.  Consider moving out of here for global brightness control.

  long beginTime = millis();                                     // Set 'beginTime' based on the number of miliseconds that the Arduino has been running 

  String displayString = String(toDisplay);                      // Set the dispay string tp match the temp value passed in
  
  for(int digit = 4 ; digit > 0 ; digit--) {                     // Set up a loop to loop around the 4 digits of the display.  Starting with digit 4 of the display

    int minusOne = digit -1;                                     // Set up minusOne to one less then the count.  This will be used to set the beging and end of the substring below
    String theDigit = displayString.substring(minusOne, digit);  // Get the value of the first digit to display, from the passed in value
    
    int displayDigit = theDigit.toInt();                         // Convert the digit to be displayed to an integer
    //Turn on a digit for a short amount of time
    switch(digit) {                                              // Based on the count in the loop light the corresponding digit
    case 1:
      digitalWrite(digit1, LOW);                                 // Display the right digit to the first in the time string
      lightNumber(displayDigit);
      break;
    case 2:
      digitalWrite(digit2, LOW);                                 // Display the second right digit to the second in the time string
      lightNumber(displayDigit);
      break;
    case 3:
      digitalWrite(digit3, LOW);                                 // Display the second left digit to the third in the time string
      degs();
      break;
    case 4:
      digitalWrite(digit4, LOW);                                 // Display the right digit to the forth in the time string
      cee();
      break;
    }
    
    delayMicroseconds(Brightness);                       // Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off();                                                       // Switch off all of the segments on the LED

    //Turn off all digits                                        // This should have been retired now with the above.  It can be reinstated if required
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ;                          // Wait for 20ms to pass before we paint the display again
}

//End of code from the display tab

///////////////////////////////
//Code from the debugging tab//
///////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//The code in this tab moves all of the debugging text away from the main code//
////////////////////////////////////////////////////////////////////////////////

void debug_help () {
  // Show that the help message is to be displayed 
  
  if (debug == 1){
    Serial.println ("HELP SETTING Debugging - Display the help message");
  }
}

void debug_summertime () {
  //Show the month and day that the rtc says. This will be used to test whether the clock should be adjusted
  
  if (debug==2){
    Serial.print ("SUMMERTIME SETTING Debugging - RTC time settings - ");
    Serial.print ("the month is ");
    Serial.print (themonth);
    Serial.print (", the day is ");
    Serial.print (theday);
    Serial.print (" and the month day is ");
    Serial.println (monthday);
  }
}

//End of code from the debugging tab

/////////////////////////////////////
//Code from the segment control tab//
/////////////////////////////////////

//One

void one()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 6);
  digitalWrite(latchPin, HIGH);
}

void two()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 91);
  digitalWrite(latchPin, HIGH);
}

void three()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 79);
  digitalWrite(latchPin, HIGH);
}

void four()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 102);
  digitalWrite(latchPin, HIGH);
}

void five()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 109);
  digitalWrite(latchPin, HIGH);
}

void six()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 125);
  digitalWrite(latchPin, HIGH);
}

void seven()
{
  //Display a number 7
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 7);
  digitalWrite(latchPin, HIGH);
}

void eight()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 127);
  digitalWrite(latchPin, HIGH);
}

void nine()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 111);
  digitalWrite(latchPin, HIGH);
}

void zero()
{
  if (verbose==1){
    Serial.println("This is a zero");
  }
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 63);
  digitalWrite(latchPin, HIGH);
}

void off()
{
  //Switch off all segments
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void degs()
{
  //Display degress sign
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 99);
  digitalWrite(latchPin, HIGH);

}

void cee()
{
  //Display a capital C
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 57);
  digitalWrite(latchPin, HIGH);
}

void eee()
{
  //Display a capital E
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void aitch()
{
  //Display a capital H
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void ell()
{
  //Display a capital L
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, LOW);
}

void pee()
{
  //Display a capital L
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

//Digit end

void lightNumber(int numberToDisplay) {

  //zero;
  switch (numberToDisplay){

  case 0:
    zero();
    break;

  case 1:
    one();
    break;

  case 2:
    two();
    break;

  case 3:
    three();
    break;

  case 4:
    four();
    break;

  case 5:
    five();
    break;

  case 6:
    six();
    break;

  case 7:
    seven();
    break;

  case 8:
    eight();
    break;

  case 9:
    nine();
    break;

  case 10:
    off();
    break;
  
  default:
    nine();
    break;
  }
}

//End of code from the segment control tab

////////////////////////////////////////////////
//Start of the code from the RTC Control block//
////////////////////////////////////////////////

void setrtcTime () {
  
  // A request has been made to reset the time on the rtc. Set it to the time that the sketch was compiled
  Serial.println("Setting the clock in 30 seconds");   // Confirm to the outside world that the time is being reset
  delay(30000);                                        // Delay for 30 seconds to ensure that the monitor has been started and the world is watching
    
  // Display the time that the clock will be set to
  Serial.print ("Setting the RTC time to ");
  Serial.print(__DATE__);
  Serial.print (" ");
  Serial.println(__TIME__);
  // End of the time confirmation block
    
  rtc.adjust(DateTime(__DATE__, __TIME__));            // This sets the RTC to the date & time this sketch was compiled
  setTheTime=false;                                           // Now switch off the reset so it isn't set again
}

//End of the code from the RTC Control block

//Start of code from the Get RTC Time block

String getthertcTime () {
  if (verbose==1){
      Serial.print("Getting the time now");
  }
    
  DateTime now = rtc.now();

  theyear = int(now.year());
  themonth = int(now.month());
  theday = int(now.dayOfTheWeek());
  monthday = int(now.day());
  hours = int(now.hour());
  minutes= int(now.minute());
  seconds = int(now.second());

  clockchange();
  setmorningtime();

  if (debug==2){
    Serial.print("GMT at the time display ");
    Serial.print(GMT);
    Serial.print(" and the hours are ");
    Serial.print(hours);
  }
  if (GMT=='Y') {
    if (debug==4){
      Serial.println("Inside the adjust");
    }
    
    if (hours==0){
      hours = 23;
    }
    else {
      hours = hours - 1;  
    }
  }

  if (hours<10){
    timeString = '0' + String(hours);
  }
  else {
    timeString = String(hours);
  }
  
  if (minutes<10){
    timeString = timeString + '0' + String(minutes);
  }
  else {
    timeString = timeString + String(minutes);
  }
  
  return timeString;
}

void setmorningtime () {
  if (theday == 0 || theday == 6) {
    morningon = 900;
  }
  else {
    morningon = 625;
  }
}

//End of code from the Get RTC Time block

//Start of code from the Summertime block

void summertime () {

  if (debug==3){
    Serial.print("The hours are ");
    Serial.println(hours);
  }
  
  GMT = 'Y';

  if (debug==3){
    Serial.print("The month = ");
    Serial.println(themonth);
  }
  if (themonth > 3 && themonth < 11){
    GMT = 'N';
  }
  else if (themonth == 10){
    if (monthday !=0) {
      testday = monthday;
      while (testday > 0, testday, testday--){
        if (monthday + 7 > 31){
          GMT = 'Y';
        }
      }
    } 
  }
  else if (themonth == 3){
    if (monthday !=0){
      testday = monthday;
      while (testday > 0, testday, testday--){
        if (monthday + 7 > 31){
          GMT = 'N';
        }
      }  
    }
  } 

  if (debug==3){
    Serial.print("GMT ");
    Serial.println(GMT);
  }
}

void clockchange () {
  
  debug_summertime();
   
  if (themonth == 10) {
    if (theday == 0) {
      if (monthday + 7 > 31) {
        GMT = 'Y';
      }
    }
  }
  if (themonth == 3) {
    if (theday == 0) {
      if (monthday + 7 > 31) {
        GMT = 'N';
      }
    }
  }
}

void night_and_day ()
{
  // Routine to set the day/night value (lights_on) used to switch on/off the neo-pixels and temp dislpay durimg day/night time
  if (debug==4) {
    Serial.print ("The timestring is set to ");
    Serial.print (timeString);
    Serial.print (" and the time, ");
    Serial.print (hours);
    Serial.println (minutes); 
  }
  
  int timeset = hours * 100;
  timeset = timeset + minutes;
  if (timeset < nightoff && timeset > morningon) {                 
    lights_on = 'Y';                                  // Ensure that the on/off swicth is set to on if the time is between the morningon time and the nightoff time
    Brightness = 500;                         // Set the 7 Seg LED brightness to day time brightness
  }
  else 
  {
    if (lights_on=='Y'){                              // The light switch is being set to 'N'
      setColours(strip.Color(0, 0, 0), 0, 8);         // Set all the lights to off                               
    }
    lights_on = 'N';                                  // Ensure that the on/off swicth is set to off if the time is between the nightoff time and the morningon time
    Brightness = 200;                         // Set the 7 Seg LED brightness to night time brightness
  } 

      if (debug==4){
        Serial.print ("Lights on value ");
        Serial.println (lights_on);
    }
}

//End of code from the Summertime block

//Start of code from the Write Temp block

void writeTemp (){
  
  //Write the calculated temp out to the serial port for transmission in JSON format
  Serial.print("{\"Sensor_ID\":7,\"Sensor_Value\":");
  Serial.print(sensorVal);
  Serial.print(",\"Voltage\":\"");
  
  Serial.print(voltage);
  Serial.print("\",\"Temperature\":\"");
 
  Serial.print(temperature);
  
  Serial.println("\"}");
}

//End of code from the Write Temp block

//Start of code from the NeoPixel block

// Fill the dots one after the other with a colour
void setColours(uint32_t c, uint8_t wait, int setNumber) {
  for(uint16_t i=0; i<setNumber; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//End of code from the NeoPixel block

//Start of code from Temp Sync block

float getTemp (){  
  // Get the temprature
  
  Serial.println("****REALLY!****");
  SensorAverage = 0;  // Using an average mechinisum for accuracy.  Start by settimg the value to 0.  
  int i;              // Create variable for the counting of the sensor values

  for(i=0; i<Count; i++)
  {
    sensorVal = analogRead(temp);                 // Get the voltage measured at the analogue input
    SensorAverage = SensorAverage + sensorVal;    // Add the value collected to those previously collected
    
    timeString = getthertcTime();                 //Get the time from the realtime clock
    if (seconds < 30 || seconds > 45){            //It is not between 30 and 45 seconds on the clock.  
        displayTime(timeString);                  //Display the time
      }
      else{                                       //It is between 30 and 45 seconds on the clock
       inttemp = int(temperature);                //Get the temperature value ready for display
       displaytemp = String(inttemp);             //Convert it to a string
       displayTemp(displaytemp);                  //Display the temperature
     }
  }

  //Calculate the temp from the from the reading
  sensorVal = SensorAverage / Count;               //Find the average value that was calculated
  voltage = (sensorVal/1024.0) * input_voltage;    //Calculate the voltage from the average value calculated
  temperature = (voltage - 0.5) * 100;             //Calculate the tempreture from the voltage calculated
  
  //Check to see if the temp has drifted from a reasonable range
  //If the temp has risen above 26 degrees - reset the Arduino
  
  if (temperature > over_run_temp) {
    Serial.println("Inside the checking loop");  // Debugging prompt to infom that we are in the loop to adjust the lights
    
    int delayAdjust = delayOffset - 1;           // ??
    int green_adjust = 765 * delayAdjust;        // ??

    //debug_temp_overrun_led_values ();
    if (debug==3){
      Serial.println ("TEMP OVERRUN SETTING Debugging");
      Serial.print ("Record Time - ");
      Serial.print (monthday);
      Serial.print ("/");
      Serial.print (themonth);
      Serial.print ("/");
      Serial.print (theyear);
      Serial.print (" - ");
      Serial.print (hours);
      Serial.print (":");
      Serial.print (minutes);
      Serial.print (":");
      Serial.println (seconds);
      Serial.print ("Display the values used to set the LEDs - ");
      Serial.print ("Setting green ");
      Serial.print (delayCount);
      Serial.print (" adjust ");
      Serial.print(delayAdjust);
      Serial.print(" green adjust ");
      Serial.print (green_adjust);
      Serial.print(" Adjusted ");
      Serial.print(delayCount - green_adjust);
      Serial.print(" Offset ");
      Serial.println(delayOffset);
    }
    
    if (delayCount - green_adjust < 256) {                                     // If the sum is below 256, only the Green LEDs should be lit.  delayCount sets by how much.
      green = delayCount - green_adjust; 
      red = 0;
      blue = 0;
    }
    if (delayCount - green_adjust > 255 && delayCount - green_adjust < 511) {  // If the sum is between 255 and 511 the green is at full intensity, start lighting the red
      red = delayCount - green_adjust - 255;
    }
    if (delayCount - green_adjust > 510) {                                     // Green and red are at full intensity.  Start lighting the blue.
      blue = delayCount - green_adjust - 510;
    }
    
    if (delayCount == 0.9) {
       setColours(strip.Color(0, 0, 0), 0, 8);                                 //Set all the lights to off
    }

    if (debug==3) {
      Serial.print ("Red : ");
      Serial.print (red);
      Serial.print (" Green : ");
      Serial.print (green);
      Serial.print (" Blue : ");
      Serial.print (blue);
      Serial.print (" Delay Offest (LED to light) : ");
      Serial.println (delayOffset - 1);
    }
    
    if (lights_on == 'Y') {                                                    // Are the lights suppose to be on?
      setColours(strip.Color(red, green, blue), 0, delayOffset);               // delayOffset indicates the highest light that should be lit.  Set it based on the calcualted RGB intensity
      setColours(strip.Color(0, 0, 0), 0, delayOffset -1);                     // Switch all the lights bellow that to off. This is to reduce the brighness of the out of range lights
    }

    if (delayCount - green_adjust == 765){  
      delayOffset++;                                                           // All LEDs are set to their full intensity.  Move the delayCount up by one to lights the next row.
    }

    if (delayOffset > 8){
      input_voltage = input_voltage - 0.01;   // Reduce the input voltage to try and bring the temp reading down
    }
    delayCount = delayCount + 15.0;         
  }
  else if (delayCount > 0)
  {
    int delayAdjust = delayOffset - 1;           // ??
    int green_adjust = 765 * delayAdjust;        // ??

    //debug_temp_overrun_led_values ();
    if (debug==3){
      Serial.println ("TEMP OVERRUN SETTING (INSIDE RANGE) Debugging");
      Serial.print ("Record Time - ");
      Serial.print (monthday);
      Serial.print ("/");
      Serial.print (themonth);
      Serial.print ("/");
      Serial.print (theyear);
      Serial.print (" - ");
      Serial.print (hours);
      Serial.print (":");
      Serial.print (minutes);
      Serial.print (":");
      Serial.println (seconds);
      Serial.print ("Display the values used to set the LEDs - ");
      Serial.print ("Setting green (delayCount ");
      Serial.print (delayCount);
      Serial.print (" adjust (delayAdjust) ");
      Serial.print(delayAdjust);
      Serial.print(" green adjust (green_Adjust) ");
      Serial.print (green_adjust);
      Serial.print(" Adjusted (delayCount - green_adjust) ");
      Serial.print(delayCount - green_adjust);
      Serial.print(" Offset ");
      Serial.println(delayOffset);
      
      Serial.print ("Red : ");
      Serial.print (red);
      Serial.print (" Green : ");
      Serial.print (green);
      Serial.print (" Blue : ");
      Serial.print (blue);
      Serial.print (" Delay Offest (LED to light) : ");
      Serial.println (delayOffset - 1);
    }
    
    if (lights_on == 'Y') {                                           // Are the lights suppose to be on?
      setColours(strip.Color(red, green, blue), 0, delayOffset);      // delayOffset indicates the highest light that should be lit.  Set it based on the calcualted RGB intensity
      setColours(strip.Color(0, 0, 0), 0, delayOffset -1);            // Switch all the lights below that to off. This is to reduce the brighness of the out of range lights
    }  
    
    delayCount = delayCount - 15.0;

    if (delayCount - green_adjust < 256) {                                     // If the sum is below 256, only the Green LEDs should be lit.  delayCount sets by how much.
      green = delayCount - green_adjust; 
      red = 0;
      blue = 0;
    }
    if (delayCount - green_adjust > 255 && delayCount - green_adjust < 511) {  // If the sum is between 255 and 511 the green is at full intensity, start lighting the red
      red = delayCount - green_adjust - 255;
    }
    if (delayCount - green_adjust > 510) {                                     // Green and red are at full intensity.  Start lighting the blue.
      blue = delayCount - green_adjust - 510;
    }

    if (delayCount - green_adjust == 0 && delayOffset > 1){  
      delayOffset--;                                                           // All LEDs are set to their full intensity.  Move the delayCount up by one to lights the next row.
    }
  }
  
  //Put the reset code in here ^^^
 
  int offset = int(temperature-17);     //Adjust the temprature ready for dislay on the neo-pixels (it starts at 17 dgrees)
  
  if (lights_on == 'Y') {
    if (delayCount < 1) {
      if (debug==3){
        Serial.print ("Inside the standard light setting routine.  delayCount = ");
        Serial.println (delayCount);
      }
      setColours(strip.Color(0, 0, 255), 0, 8);      //Set all the lights to blue
      int midlights = 7 - offset;                    //Calculate first of the middle (pink) lights.  7 in the strip - the lights to turn red
    
      if (midlights > 0){                            //If not all the lights are red turn some pink
        for (int l=offset+midlights; l>offset; l--){
             int val = 255/(midlights+2);            //Calculate a shade of pink to start with
             int change = val*(l-offset);            //Adjust it by the light that is been lit (make to bluer nearer the blue light)
             int r = (255 - change);                 //Calculate the red value (making it less red)
             int b = (0 + change);                   //Calculate the blue value (making it more blue)
   
             setColours(strip.Color(r, 0, b), 0, l); //Set the value of the light based on the calculated value off r and b green is asways off
        }
      }
      setColours(strip.Color(255, 0, 0), 0, offset); //Set the remaining lights to red        
    }
  }
  else 
  {
    setColours(strip.Color(0, 0, 0), 0, 8);   //Set all the lights to off
  }
  return (temperature); 
}

//End of code from the Temp Sync block

//Start of code from the DHT11 sensor block

float measure_DHT_values () {

  Serial.println("###########################################");
  Serial.println("# Checking the temp");
  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("The temp is ");
  //Serial.println(DHT.temperature);

  Serial.print("# Temp = ");
  Serial.print(DHT.temperature);
  //Serial.print("Humidity = ");
  //Serial.println(DHT.humidity);
  temp_read = 'Y';

  int offset = int(DHT.temperature-17);     //Adjust the temprature ready for dislay on the neo-pixels (it starts at 17 dgrees)
  Serial.print(" - Offset = ");
  Serial.print(offset);
  //Serial.print("The lights should be on? - ");
  //Serial.println(lights_on);

  if (lights_on == 'Y') {
    //Serial.print(" - Lights on");
    Serial.print(" - Delay count = ");
    Serial.print(delayCount);
    if (delayCount < 1) {
      //Serial.println("Inside the delay block");
      //Serial.print("Debug mode is set to ");
      //Serial.println(debug);
      if (debug==3){
        Serial.print ("Inside the standard light setting routine.  delayCount = ");
        Serial.println (delayCount);
      }
      setColours(strip.Color(0, 0, 255), 0, 8);      //Set all the lights to blue
      int midlights = 7 - offset;                    //Calculate first of the middle (pink) lights.  7 in the strip - the lights to turn red
    
      Serial.print(" - Midpoint = ");
      Serial.println(midlights);

      if (midlights > 0){                            //If not all the lights are red turn some pink
        //Serial.print("Inside the midlights logic with an offset of ");
        //Serial.println(offset);
        if (offset > -17){
          for (int l=offset+midlights; l>offset; l--){
               //Serial.print("Looping the lights ");
               //Serial.print(l);
               //Serial.print(" of ");
               //Serial.println(offset);
               //delay(2000);
               int val = 255/(midlights+2);            //Calculate a shade of pink to start with
               int change = val*(l-offset);            //Adjust it by the light that is been lit (make to bluer nearer the blue light)
               int r = (255 - change);                 //Calculate the red value (making it less red)
               int b = (0 + change);                   //Calculate the blue value (making it more blue)
   
               setColours(strip.Color(r, 0, b), 0, l); //Set the value of the light based on the calculated value off r and b green is asways off

          }
        }  
        else
        {
          //The temp reading is 0.00 (or lower) as this is for inside we can assume that no readying has been taken
          setColours(strip.Color(0, 255, 0), 0, 8);
          for (int o = 0; o < 9; o++){
            setColours(strip.Color(0, 0, 0), 0, 8);
            setColours(strip.Color(0, 255, 0), 0, o);
            delay(5000);
          }  
        } 
      }
      if (offset > -1){
        setColours(strip.Color(255, 0, 0), 0, offset); //Set the remaining lights to red      
      }  
    }
  }
  else 
  {
    //Serial.print("The lights are off");
    setColours(strip.Color(0, 0, 0), 0, 8);   //Set all the lights to off
  }

  //Serial.print("About to return a temprature of ");
  //Serial.println(DHT.temperature);
  Serial.println("###########################################");
  return (DHT.temperature);
}

// End of code from the DHT11 sensor block
