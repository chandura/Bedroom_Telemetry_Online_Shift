void displayTime(String toDisplay) {
  #define DISPLAY_BRIGHTNESS  500

  long beginTime = millis();

  String displayString = String(toDisplay);
  
  for(int digit = 4 ; digit > 0 ; digit--) {

    int minusOne = digit -1;
    String theDigit = displayString.substring(minusOne, digit);
    
    int displayDigit = theDigit.toInt();
    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, LOW);
      lightNumber(displayDigit);
      break;
    case 2:
      digitalWrite(digit2, LOW);
      lightNumber(displayDigit);
      break;
    case 3:
      digitalWrite(digit3, LOW);
      lightNumber(displayDigit);
      break;
    case 4:
      digitalWrite(digit4, LOW);
      lightNumber(displayDigit);
      break;
    }
    
    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off(); 

    //Turn off all digits
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ; //Wait for 20ms to pass before we paint the display again
}

void displayTemp(String toDisplay) {
  #define DISPLAY_BRIGHTNESS  500

  long beginTime = millis();

  String displayString = String(toDisplay);
  
  for(int digit = 4 ; digit > 0 ; digit--) {

    int minusOne = digit -1;
    String theDigit = displayString.substring(minusOne, digit);
    
    int displayDigit = theDigit.toInt();
    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, LOW);
      lightNumber(displayDigit);
      break;
    case 2:
      digitalWrite(digit2, LOW);
      lightNumber(displayDigit);
      break;
    case 3:
      digitalWrite(digit3, LOW);
      degs();
      break;
    case 4:
      digitalWrite(digit4, LOW);
      cee();
      break;
    }
    
    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off(); 

    //Turn off all digits
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ; //Wait for 20ms to pass before we paint the display again
}

void displayHelp() {
  
  if (verbose == 1){
    Serial.println ("Display the help message");
  }
  
  #define DISPLAY_BRIGHTNESS  500

  long beginTime = millis();

  //String displayString = String(toDisplay);
  
  for(int digit = 4 ; digit > 0 ; digit--) {

    //int minusOne = digit -1;
    //String theDigit = displayString.substring(minusOne, digit);
    
    //int displayDigit = theDigit.toInt();
    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, LOW);
      aitch();
      break;
    case 2:
      digitalWrite(digit2, LOW);
      eee();
      break;
    case 3:
      digitalWrite(digit3, LOW);
      ell();
      break;
    case 4:
      digitalWrite(digit4, LOW);
      pee();
      break;
    }
    
    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    off(); 

    //Turn off all digits
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
  }

  while( (millis() - beginTime) < 10) ; //Wait for 20ms to pass before we paint the display again
}

//Notes on how the display functions work
//Given a number, we display 10:22
//After running through the 4 numbers, the display is left turned off

//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//If each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)
