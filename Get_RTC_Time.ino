
void dealwithlostPower () {
  //To invoke this reload the sketch to the clock to reset the time.
  //Serial.println("RTC lost power, lets set the time!");
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

String getthertcTime () {
  if (verbose==1){
      Serial.print("Getting the time now");
  }
    
  DateTime now = rtc.now();

  hours = int(now.hour());
  minutes=int(now.minute());
  seconds = int(now.second());

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

  
  
  if (verbose == 1){
    Serial.print ("The timeString is: ");
    Serial.println (timeString);
  }
  
    if (verbose==1){
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
    
      Serial.print(" since midnight 1/1/1970 = ");
      Serial.print(now.unixtime());
      Serial.print("s = ");
      Serial.print(now.unixtime() / 86400L);
      Serial.println("d");
    
      // calculate a date which is 7 days and 30 seconds into the future
      DateTime future (now + TimeSpan(7,12,30,6));
    
      Serial.print(" now + 7d + 30s: ");
      Serial.print(future.year(), DEC);
      Serial.print('/');
      Serial.print(future.month(), DEC);
      Serial.print('/');
      Serial.print(future.day(), DEC);
      Serial.print(' ');
      Serial.print(future.hour(), DEC);
      Serial.print(':');
      Serial.print(future.minute(), DEC);
      Serial.print(':');
      Serial.print(future.second(), DEC);
      Serial.println();
    
      Serial.println();
      delay(3000);
    }
    return timeString;
}
