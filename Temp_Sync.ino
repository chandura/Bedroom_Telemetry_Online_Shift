float getTemp (){  
  // Get the temprature
  
  SensorAverage = 0;  //Using an average mechinisum for accuracy.  Start by settimg the value to 0.  
  int i;              // Create variable for the counting of the sensor values

  for(i=0; i<Count; i++)
  {
    sensorVal = analogRead(temp); // Get the voltage measured at the analogue input
    SensorAverage = SensorAverage + sensorVal;
    
    timeString = getthertcTime();        //Get the time from the realtime clock
    if (seconds < 30 || seconds > 45){   //It is not between 30 and 45 seconds on the clock.  
        displayTime(timeString);         //Display the time
      }
     else{                               //It is between 30 and 45 seconds on the clock
       inttemp = int(temperature);       //Get the temperature value ready for display
       displaytemp = String(inttemp);    //Convert it to a string
       displayTemp(displaytemp);         //Display the temperature
     }
  }  
  
  //Calculate the temp from the from the reading
  sensorVal = SensorAverage / Count;     //Find the average value that was calculated
  voltage = (sensorVal/1024.0) * 5.0;    //Calculate the voltage from the average value calculated
  temperature = (voltage - 0.5) * 100;   //Calculate the tempreture from the voltage calculated
 
  int offset = int(temperature-17);     //Adjust the temprature ready for dislay on the neo-pixels (it starts at 17 dgrees)
  int timeset = hours * 100;
  timeset = timeset + minutes;

  //Serial.print("Timset ");
  //Serial.println(timeset);
  if (timeset < 2235 && timeset > morningon) {        //Only light up the neo-pixels between the hours of 7:00 am and 11:00pm 
    setColours(strip.Color(0, 0, 255), 0, 8);   //Set all the lights to blue
    int midlights = 7 - offset;                 //Calculate first of the middle (pink) lights.  7 in the strip - the lights to turn red
    
    if (midlights > 0){                         //If not all the lights are red turn some pink
      for (int l=offset+midlights; l>offset; l--){
           int val = 255/(midlights+2);          //Calculate a shade of pink to start with
           int change = val*(l-offset);          //Adjust it by the light that is been lit (make to bluer nearer the blue light)
           int r = (255 - change);               //Calculate the red value (making it less red)
           int b = (0 + change);                 //Calculate the blue value (making it more blue)
   
           setColours(strip.Color(r, 0, b), 0, l); //Set the value of the light based on the calculated value
      }
    }
    setColours(strip.Color(255, 0, 0), 0, offset); //If it is between 11:00pm and 7:00 am switch all the neo-pixals off
  }
  else 
  {
    //Serial.print("Inside the else");
    setColours(strip.Color(0, 0, 0), 0, 8);   //Set all the lights to off
  }
  return (temperature); 
}
