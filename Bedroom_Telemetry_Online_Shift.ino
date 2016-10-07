// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>  
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

//Define the DS3231 as rtc - the Real Time Clock
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Set up for the shift register
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 7;
////Pin connected to DS of 74HC595
int dataPin = 6;

// Declare the variables that are required to get the time

#define YES 1;
#define NO 0;

boolean Synced = 0;

boolean AM;
boolean PM;

// Define whether to print out the time details
boolean verbose = 0;

int counter2;
int counter3;
int hour1;
int hour2;
int hour2counter;
int min1;
int min2;

int hours;
int minutes;
int seconds;

long interval;
long previousMillis = 0; // will store last time LED was updated
long currentMillis; 

boolean doYouWantColon = 1;
boolean doYouWantBlinkyColon = 0; //set to 1 if you want the colon to blink
boolean doYouWant24HRTime = 0;
boolean doYouWantAMPM = 0;

char whatDigitDoYouWantTheDecimal = 1;

long interval1 = 500; // interval at which to blink (milliseconds)

const char digit1 = A0; //Digit 1
const char digit2 = A1; //Digit 2
const char digit3 = A2; //Digit 3
const char digit4 = A3; //Digit 4

const char temp = A6;   //Input for the input from the temp sensor

//const int aseg = 3;     //Segment A top
//const int bseg = 4;     //Segment B top right
//const int cseg = 5;     //Segmnet C bottom right
//const int dseg = 6;     //Segment D bottom
//const int eseg = 7;     //Segment E bottom left
//const int fseg = 8;     //Segment F top left
//const int gseg = 9;     //Segment G middle
//const int dp = 10;      //Decimal point
const int colon = 11;
int Count;              //Declare the counter to read the temp sensor many times
float temperature;
int inttemp;
String displaytemp;
String timeString;
long SensorAverage = 0;
int sensorVal = 0;
float voltage = 0;

#define Count 1000

void setup () {
  
  Serial.begin(9600); //Open the serial port
  Wire.begin(); //Open the interface for the rtc
  delay(3000); // wait for console opening
  if (verbose == 1){
    Serial.println("Serial communicatons are open");
  }

  //Taken from the rtc example
  #ifndef ESP8266
      while (!Serial); // for Leonardo/Micro/Zero
  #endif

  if (! rtc.begin()) {
    if (verbose==1) {
        Serial.println("Couldn't find RTC");  
    }
    displayHelp();
    while (1);
  }
  else {
    if (verbose==1){
      Serial.println("Found the rtc");
    }
  }

  if (rtc.lostPower()) {
    displayHelp();
    //To invoke this reload the sketch to the clock to reset the time.
    dealwithlostPower();
    while(1);
  }

  setVariables();

  // Set the relevent pins for to control the LED segments
  for (int pinNumber = 3; pinNumber <13; pinNumber++)
  {
    pinMode(pinNumber, OUTPUT);
    //digitalWrite(pinNumber, LOW);
  }

  //int numbertoDisplay(1);
  //digitalWrite(11, LOW);
  //digitalWrite(latchPin, LOW);
  //shiftOut(dataPin, clockPin, MSBFIRST, numbertoDisplay);
  //digitalWrite(latchPin, HIGH);
  
  // Set the relevant pins to control the LED digits
  pinMode(digit1, OUTPUT); //Digit 1
  pinMode(digit2, OUTPUT); //Digit 2
  pinMode(digit3, OUTPUT); //Digit 3
  pinMode(digit4, OUTPUT); //Digit 4

  // Set the relevant pin to read the temprature
  pinMode(temp, INPUT); //Temp sensor
  
  writeTemp();

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.setBrightness(25);
  strip.show(); // Initialize all pixels to 'off'
}

void loop () {

  // Sync time from the externl source as there is no RTC
  previousMillis = millis();
 
  //Turn off all the segments of the display
  off();

  for (int hold = 100; hold>0; hold--){
    // If it is an odd minute display the temp instead
    if (seconds < 30 || seconds > 45){
      //timeString = digitalClockDisplay();
      timeString = getthertcTime();
      displayTime(timeString);
    }
    else {
      temperature = getTemp();
      inttemp = int(temperature);
      displaytemp = String(inttemp);
      displayTemp(displaytemp);
      currentMillis = millis();
      if (verbose == 1){
          Serial.print ("Looking for previous time being less that the current time - 1 minute ");
          Serial.print ("Previous time : ");
          Serial.print (previousMillis);
          Serial.print (" Current time : ");
          Serial.print (currentMillis);
          Serial.print (" Current time less 1 minute : ");
      }
        
      currentMillis = currentMillis - (1000.00 * 60.00 * 10.00);
        
      if (verbose == 1){
          Serial.println (currentMillis);
      }
        
      //Decide whether to write out the temp to the database - every 10 mins
      //This needs reinstating - but using the RTC and not the sync'ed time
        
      //if (previousMillis < (millis() - (1000.00 * 60.00 * 10))) {
          writeTemp();
          //previousMillis = millis();
          //if (verbose == 1){
            //Serial.print ("Previous time reset to ");
            //Serial.println (previousMillis);
          //}
      //}
      //else {
        //if (verbose == 1)
          //Serial.println ("No database write this time");
        //}
      //}
    }
  }
}  

