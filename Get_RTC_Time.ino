
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

  themonth = int(now.month());
  theday = int(now.dayOfTheWeek());
  monthday = int(now.day());
  hours = int(now.hour());
  minutes= int(now.minute());
  seconds = int(now.second());

  setmorningtime();

  if (GMT=="Y") {
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

