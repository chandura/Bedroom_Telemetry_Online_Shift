////////////////////////////////////////////////////////////////////////////////
//The code in this tab moves all of the debugging text away from the main code//
////////////////////////////////////////////////////////////////////////////////

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

void debug_help () {
  // Show that the help message is to be displayed 
  
  if (debug == 1){
    Serial.println ("HELP SETTING Debugging - Display the help message");
  }
}

void debug_temp_overrun_led_values (){
    // Show the debugging message associated with temprature measurment 
   
    if (debug == 3) {
      //Serial.print ("TEMP OVERRUN SETTING Debugging - Display the values used to set the LEDs - ");
      //Serial.print ("Setting green ");
      //Serial.print (delayCount);
      //Serial.print (" adjust ");
      //Serial.print(delayAdjust);
      //Serial.print(" green adjust ");
      //Serial.print (green_adjust);
      //Serial.print(" Adjusted ");
      //Serial.print(delayCount - green_adjust);
      //Serial.print(" Offset ");
      //Serial.println(delayOffset);
    }
}

