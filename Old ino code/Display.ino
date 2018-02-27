
///////////////////////////////////////////////////////////////////////////////////////////////////////
//The code in this tab is used to control both the time and temprature displayed on the 7 Segment LED//
///////////////////////////////////////////////////////////////////////////////////////////////////////

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

// Notes on how the display functions work
// Given a number, we display 10:22
// After running through the 4 numbers, the display is left turned off

// Display brightness
// Each digit is on for a certain amount of microseconds
// Then it is off until we have reached a total of 20ms for the function call
// Let's assume each digit is on for 1000us
// If each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
// That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
// Let's define a variable called brightness that varies from:
// 5000 blindingly bright (15.7mA current draw per digit)
// 2000 shockingly bright (11.4mA current draw per digit)
// 1000 pretty bright (5.9mA)
// 500 normal (3mA)
// 200 dim but readable (1.4mA)
// 50 dim but readable (0.56mA)
// 5 dim but readable (0.31mA)
// 1 dim but readable in dark (0.28mA)
