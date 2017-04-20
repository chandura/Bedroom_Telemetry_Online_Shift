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



