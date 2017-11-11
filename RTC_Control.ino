String getthertcTime () {
  if (verbose==1){
      Serial.print("Getting the time now");
  }
    
  DateTime now = rtc.now();

  theyear = int(now.year());
  themonth = int(now.month());
  theday = int(now.dayOfTheWeek());
  monthday = int(now.day());
  hours = int(now.hour());
  minutes= int(now.minute());
  seconds = int(now.second());

  clockchange();
  setmorningtime();

  if (debug==2){
    Serial.print("GMT at the time display ");
    Serial.print(GMT);
    Serial.print(" and the hours are ");
    Serial.print(hours);
  }
  if (GMT=='Y') {
    if (debug==4){
      Serial.println("Inside the adjust");
    }
    
    if (hours==0){
      hours = 23;
    }
    else {
      hours = hours - 1;  
    }
  }

  if (hours<10){
    timeString = '0' + String(hours);
  }
  else {
    timeString = String(hours);
  }
  
  if (minutes<10){
    timeString = timeString + '0' + String(minutes);
  }
  else {
    timeString = timeString + String(minutes);
  }
  
  return timeString;
}

void setmorningtime () {
  if (theday == 0 || theday == 6) {
    morningon = 900;
  }
  else {
    morningon = 625;
  }
}

void setrtcTime () {
  
  // A request has been made to reset the time on the rtc. Set it to the time that the sketch was compiled
  Serial.println("Setting the clock in 30 seconds");   // Confirm to the outside world that the time is being reset
  delay(30000);                                        // Delay for 30 seconds to ensure that the monitor has been started and the world is watching
    
  // Display the time that the clock will be set to
  Serial.print ("Setting the RTC time to ");
  Serial.print(__DATE__);
  Serial.print (" ");
  Serial.println(__TIME__);
  // End of the time confirmation block
    
  rtc.adjust(DateTime(__DATE__, __TIME__));            // This sets the RTC to the date & time this sketch was compiled
  setTime=0;                                           // Now switch off the reset so it isn't set again
}

