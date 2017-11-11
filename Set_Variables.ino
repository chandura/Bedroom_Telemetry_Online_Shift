int setVariables()
{
//Serial.println ("Setting the variables ... ");

/* to set time, write the digits below to the current time (or next minute) and upload, if the current time is 9:40, I would write
9:42 into the digits below (example provided) and upload it. After it uploads, disconnect power and  plug it in at exactly 9:42.
If the time is past 1:00, you must set the hour1 to 1 AND the hour2 to whatever hour it is in 24 hour time
so if it is 1:34 PM, i would set hour1 to 1, hour2 to 13, and the minutes regularly
*/

// WARNING FOR 24HR TIME!! If you are using 24hr time, if hour1 = 1 it
//effectively adds 10hrs to the time!!!!!!
//hour1 = 1; // first digit of hour, only can be 1 or 0, unless using 24 hour time (defaulting to off state)
//hour2 = 1;
//hour2counter = 23; //second hour digit, contains value between 1 and 24 (please put in the hours as it would be 12 hour time.
//This is pretty easy, just use 12 + timein12HRformat if the time is pas 1:00PM
//min1 =  5; // contains value between 0 and 5
//min2 =  9; // contains value between 0 and 9

//interval = 59923; // set to 60 (or 30) for testing your display

//doYouWantColon = YES; // if you do not want colon (for example using 24 hour time
                     // and you want 2400 rather than 24:00) set to NO

//doYouWantBlinkyColon = YES; //set to YES if you want the colon to blink

//interval1 = 500; // interval at which to blink the colon (milliseconds)
                 // set interval1 to 1000 if you want a longer blink (defaults to 500)

//doYouWantAMPM = NO; // set to NO if you dont want AM/PM display support
                    // The time will be AM if the decimal is on at the bottom of the screen
                    // The time is PM if the apostrophe is on at the top
                    // MUST BE set to NO if you have 24 hour time on

//whatDigitDoYouWantTheDecimal = 3; // what digit do you want the AM display to be after
//doYouWant24HRTime = NO; // set to YES if you want 24 hour time
}
