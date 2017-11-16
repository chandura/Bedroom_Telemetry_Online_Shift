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
        GMT = "Y";
      }
    }
  }
  if (themonth == 3) {
    if (theday == 0) {
      if (monthday + 7 > 31) {
        GMT = "N";
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
  if (timeset < 2235 && timeset > morningon) {                 
    lights_on = 'Y';                                  // Ensure that the on/off swicth is set to on if the time is between the morningon time and the nightoff time
  }
  else 
  {
    if (lights_on=='Y'){                              // The light switch is being set to 'N'
      setColours(strip.Color(0, 0, 0), 0, 8);         // Set all the lights to off                               
    }
    lights_on = 'N';                                  // Ensure that the on/off swicth is set to off if the time is between the nightoff time and the morningon time
  } 

      if (debug==4){
        Serial.print ("Lights on value ");
        Serial.println (lights_on);
    }
}



