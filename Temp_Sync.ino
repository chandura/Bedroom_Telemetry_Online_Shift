float getTemp (){  
  // Get the temprature
  
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
       // The lenght of time that the tempreture has been out of range is short.  Switch all of the lights off.
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
      //setColours(strip.Color(255, 255, 255), 0, delayOffset -1);             // Switch all the lights below that to white
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
    // delayCount = 0;

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
      //setColours(strip.Color(255, 255, 255), 0, delayOffset -1);    // Switch all the lights below that to white
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
  
  //int timeset = hours * 100;
  //timeset = timeset + minutes;

  //Serial.print("Timset ");
  //Serial.println(timeset);

  //night_and_day();                                   // Check whether the clock should be in night or day mode 
  
  //if (timeset < 2235 && timeset > morningon) {     //Only light up the neo-pixels between the hours of 7:00 am and 11:00pm 
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
      //lights_on = 'Y';
    }
  }
  else 
  {
    //Serial.print("Inside the else");
    setColours(strip.Color(0, 0, 0), 0, 8);   //Set all the lights to off
    //lights_on = 'N';
  }
  return (temperature); 
}
       
