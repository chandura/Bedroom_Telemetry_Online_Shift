#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

String digitalClockDisplay(){
  // digital clock display of the time

  if (verbose == 2){
    Serial.print ("The raw values are ");
    Serial.print (hours);
    Serial.print (":");
    Serial.print (minutes);
    Serial.print(":");
    Serial.print(seconds);
  }
  
  if (verbose == 2){
    Serial.print(" ");
    Serial.print(theday);
    Serial.print(" ");
    Serial.print(themonth);
    Serial.print(" ");
    Serial.print(theyear); 
    Serial.println();
  } 
    
  if (hours < 10){
    timeString = "0" + String(hours);
  }
  else {
    timeString = String(hours);
  }
  
  if (minutes < 10) {
    timeString = timeString + "0" + String(minutes);
  }
  else {
    timeString = timeString + String(minutes);
  }
  
  if (verbose == 1){
    Serial.print ("The timeString is: ");
    Serial.println (timeString);
  }
  
  return timeString;
}

void processSyncMessage() {
  // If time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      //setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
  
  //if(timeStatus() == timeSet){
    //Synced = 1;
  //}  
}
