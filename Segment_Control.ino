//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

  //zero;
  switch (numberToDisplay){

  case 0:
    zero();
    break;

  case 1:
    one();
    break;

  case 2:
    two();
    break;

  case 3:
    three();
    break;

  case 4:
    four();
    break;

  case 5:
    five();
    break;

  case 6:
    six();
    break;

  case 7:
    seven();
    break;

  case 8:
    eight();
    break;

  case 9:
    nine();
    break;

  case 10:
    off();
    break;
  
  default:
    nine();
    break;
  }
}

void one()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 6);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, LOW);
  //digitalWrite( fseg, LOW);
  //digitalWrite( gseg, LOW);
}

void two()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 91);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, LOW);
  //digitalWrite( gseg, HIGH);
}

void three()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 79);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, LOW);
  //digitalWrite( fseg, LOW);
  //digitalWrite( gseg, HIGH);
}

void four()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 102);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, LOW);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void five()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 109);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, LOW);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void six()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 125);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void seven()
{
  //Display a number 7
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 7);
  digitalWrite(latchPin, HIGH);
}

void eight()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 127);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void nine()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 111);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, LOW);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void zero()
{
  if (verbose==1){
    Serial.println("This is a zero");
  }
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 63);
  digitalWrite(latchPin, HIGH);
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, LOW);
}

void off()
{
  //Switch off all segments
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void degs()
{
  //Display degress sign
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 99);
  digitalWrite(latchPin, HIGH);

}

void cee()
{
  //Display a capital C
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 57);
  digitalWrite(latchPin, HIGH);
}

void eee()
{
  //Display a capital E
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void aitch()
{
  //Display a capital H
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, HIGH);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}

void ell()
{
  //Display a capital L
  //digitalWrite( aseg, LOW);
  //digitalWrite( bseg, LOW);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, HIGH);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, LOW);
}

void pee()
{
  //Display a capital L
  //digitalWrite( aseg, HIGH);
  //digitalWrite( bseg, HIGH);
  //digitalWrite( cseg, LOW);
  //digitalWrite( dseg, LOW);
  //digitalWrite( eseg, HIGH);
  //digitalWrite( fseg, HIGH);
  //digitalWrite( gseg, HIGH);
}
