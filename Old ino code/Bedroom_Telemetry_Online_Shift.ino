// Include the libraries that are required for the temp and humidity sensor
#include <DHT.h>


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
boolean setTime = 0;  //Switch this on (1) if you want to reset the time on the RTC at restart.
int debug = 3;        //A value of 0 swithces off all debugging
                      //A value of 1 swicthes on the debugging of the help message
                      //A value of 2 switches on the Summer Time debugging
                      //A value of 3 switches on the Temp overrun debugging
                      //A vakue of 4 switches on the day and night debugging 

int Brightness = 500;
//#define DISPLAY_BRIGHTNESS  500  // Define the brightness of the 7 Seg LEDs. The higher the number the brighter the lights 2000 - 0
//int DISPLAY_BRIGHTNESS = 500;  // Define the brightness of the 7 Seg LEDs. The higher the number the brighter the lights 2000 - 0


// Set up the initial variables associated with the time functions
char GMT = 'Y';              // Start the programme assuming that the time on the RTC is set to GMT
                             // This assumes that the PC being used to set the time is running BST
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

//int counter2;
//int counter3;
//int hour1;                   
//int hour2;                   
//int hour2counter;
//int min1;
//int min2;

//long interval;
//long previousMillis = 0; // will store last time LED was updated
//long currentMillis; 

//boolean doYouWantColon = 1;
//boolean doYouWantBlinkyColon = 0; //set to 1 if you want the colon to blink
//boolean doYouWant24HRTime = 0;
//boolean doYouWantAMPM = 0;

//char whatDigitDoYouWantTheDecimal = 1;
//long interval1 = 500; // interval at which to blink (milliseconds)
//const int colon = 11;

//boolean Synced = 0;
//boolean AM;
//boolean PM;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // Set up and array with the days of week should you want to dislay the day

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
#define DHTPIN 5             // This is the pin that is used to meausre the readings through the DHT11 sensor.
#define DHTTYPE DHT11        // DHT 11
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

// Initialize DHT sensor.
//DHT dht(DHTPIN, DHTTYPE);

void setup () {
  
  // Open up the commnications channels
  Serial.begin(9600); // Open the serial port, for communicating to the terminal monitor
  Wire.begin();       // Open the interface for the rtc
  delay(3000);        // Give the inteface to the rtc chance to open

  Serial.println("Serial communicatons are open");  // Confirm that the channels are opened
  Serial.print("GMT has been initilised as ");      
  Serial.println (GMT);                             // Confirm the setting of the GMT indicator

  //Taken from the rtc example - this was removed as it seems to refer to a WiFi connection.
  //#ifndef ESP8266
      //while (!Serial); // for Leonardo/Micro/Zero
  //#endif

  if (! rtc.begin()) {                    // Need to make a change to set the LED to Help if the RTC can't be found
    Serial.println("Couldn't find RTC");  // Confirm that the rtc could not be found 
    displayHelp();                        // Cry for help, no rtc can be found
    while (1);                            // Loop forever, what else can I do.  I'm a clock with no time
  }
  else {
    Serial.println("Found the rtc");      // Confirm that the rtc has been found, all is good
  }

  if (setTime==1) {                       // Reset the time on the RTC if requested
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

  pinMode(temp, INPUT);    // Set the relevant pin to read the temprature
  writeTemp();             // Why do this, there is no temp set yet?

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  // Test.  If it works with this commented out it can be removed
  //#if defined (__AVR_ATtiny85__)
    //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  // End of trinket special code

  strip.begin();            // Initilise the communications to the LED strip
  strip.setBrightness(6);   // Set the brightness at which to display the LEDs
  strip.show();             // Initialize all pixels to 'off'

  //Turn off all the segments of the display
  off();

  getthertcTime(); //Set the time from the RTC
  summertime();    //Test for British Summer Time
}

//void (* resetFunc) (void) = 0; //Declare the reset function at address 0

void loop () {

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
    }
    else {
      temperature = getTemp();         // Get the current temprature from from the sensor
      inttemp = int(temperature);      // Convert it to an integer ready for display on the 7 segment LED
      displaytemp = String(inttemp);   // Move the ingerer value into a string to be displayed on the 7 segment LED
      displayTemp(displaytemp);        // Call the routine to display the temprature on the 7 segment LED   
      writeTemp();                     // Write the temprature out to the serial monitor 
    }
  }
}  
