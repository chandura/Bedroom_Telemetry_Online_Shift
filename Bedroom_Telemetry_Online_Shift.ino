// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>  
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12
char GMT = 'Y';


Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

//Define the DS3231 as rtc - the Real Time Clock
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Set up for the shift register
int latchPin = 8;  //Pin connected to ST_CP of 74HC595
int clockPin = 7;  //Pin connected to SH_CP of 74HC595
int dataPin = 6; //Pin connected to DS of 74HC595

// Declare the variables that are required to get the time
#define YES 1;
#define NO 0;

//Declare everthing required to measure the temprature
const char temp = A6;   //Input for the input from the temp sensor
int Count;              //Declare the counter to read the temp sensor many times
#define Count 1000      // The sensor will sample the temp 1000 times for accuracy
// Set up a counter to delay the reset of the Arduino if it is over temprature
int delayCount = 0;


boolean Synced = 0;

boolean AM;
boolean PM;
boolean verbose = 0;
boolean setTime = 0;  //Switch this on (1) if you want to reset the time on the RTC at restart.

// Define whether to print out the time details
int debug = 0; //A value of 1 swithces on the debugging


int counter2;
int counter3;
int hour1;
int hour2;
int hour2counter;
int min1;
int min2;

int theyear;
int themonth;
int theday;
int monthday;
int hours;
int minutes;
int seconds;
int morningon;
int testday;

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

// const int colon = 11;

float temperature;
int inttemp;
String displaytemp;
String timeString;
long SensorAverage = 0;
int sensorVal = 0;
float voltage = 0;

void setup () {

  
  Serial.begin(9600); //Open the serial port
  Wire.begin(); //Open the interface for the rtc
  delay(3000); // wait for console opening
  if (verbose == 1){
    Serial.println("Serial communicatons are open");
  }
  Serial.print("GMT has been initilised as ");
  Serial.println(GMT);

  //Taken from the rtc example
  #ifndef ESP8266
      while (!Serial); // for Leonardo/Micro/Zero
  #endif

  // Need to make a change to set the LED to Help if the RTC can't be found
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

  //Reset the time on the RTC if required
  if (setTime==1) {
    // This sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
    // Now switch off the reset so it isn't set again
    setTime=0;
  }

  //setVariables();

  // Set the relevent pins for to control the LED segments
  for (int pinNumber = 3; pinNumber <13; pinNumber++)
  {
    pinMode(pinNumber, OUTPUT);
    //digitalWrite(pinNumber, LOW);
  }
  
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
  strip.setBrightness(6);
  strip.show(); // Initialize all pixels to 'off'

  //Turn off all the segments of the display
  off();

  getthertcTime(); //Set the time from the RTC
  summertime();    //Test for British Summer Time

}

void (* resetFunc) (void) = 0; //Declare the reset function at address 0

void loop () {
 
  //for (int hold = 100; hold>0; hold--){
    // If it is an odd minute display the temp instead
    if (seconds < 30 || seconds > 45){
      timeString = getthertcTime();
      displayTime(timeString);
    }
    else {
      temperature = getTemp();
      inttemp = int(temperature);
      displaytemp = String(inttemp);
      displayTemp(displaytemp);
      //currentMillis = millis();
        
      //currentMillis = currentMillis - (1000.00 * 60.00 * 10.00);
        
      writeTemp();
    }
  //}
}  

