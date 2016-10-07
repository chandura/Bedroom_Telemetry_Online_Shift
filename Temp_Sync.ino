float getTemp (){  
  // Get the temprature
  // Get the voltage measured at the analogue input
  int i;

  SensorAverage = 0;

  for(i=0; i<Count; i++)
  {
    sensorVal = analogRead(temp);
    SensorAverage = SensorAverage + sensorVal;

    if (verbose==1) {
      Serial.print("The seconds value is set to ");
      Serial.println (seconds);
    }
    timeString = getthertcTime();
    if (seconds < 30 || seconds > 45){
        //timeString = digitalClockDisplay();
        //timeString = getthertcTime();
        displayTime(timeString);
      }
     else{
       inttemp = int(temperature);
       displaytemp = String(inttemp);
       displayTemp(displaytemp);
     }
  }  
  
  //Calculate the temp from the from the reading
  sensorVal = SensorAverage / Count;
  voltage = (sensorVal/1024.0) * 5.0;
  temperature = (voltage - 0.5) * 100;
 
 if (verbose == 1){
    Serial.print ("sensorVal ");
    Serial.print (sensorVal);
    Serial.print (" voltage ");
    Serial.print (voltage);
    Serial.print (". Temperture to display ");
    Serial.print (temperature);
    Serial.println (" degrees C");
  } 
  //for(int i=0; i<strip.numPixels()+1; i++){
    int offset = int(temperature-17);
    if (verbose == 1){
      Serial.print ("Off set ");
      Serial.println(offset);
      Serial.print ("The hours reads ");
      Serial.println(hours);
    }
    if (hours < 22 && hours > 6) {
      setColours(strip.Color(0, 0, 255), 0, 8);
      int midlights = 7 - offset;
      if (verbose==1){
        Serial.print ("The value of midlights is ");
        Serial.println (midlights);
      }
      if (midlights > 0){
        for (int l=offset+midlights; l>offset; l--){
            int val = 255/(midlights+2);
            int change = val*(l-offset);
            int r = (255 - change);
            int b = (0 + change);
            if (verbose==1){
              Serial.print("The value of l is ");
              Serial.println(l);
              Serial.print("The value of val is ");
              Serial.print(val);
              Serial.print(" the value of change is ");
              Serial.print(change);
              Serial.print(" the value of r is ");
              Serial.print(r);
              Serial.print(" the value of b is ");
              Serial.println(b);
            }  
            setColours(strip.Color(r, 0, b), 0, l); 
        }
      }
      setColours(strip.Color(255, 0, 0), 0, offset); 
    }
    //delay(1000);
  //}
  
  return (temperature); 
}
