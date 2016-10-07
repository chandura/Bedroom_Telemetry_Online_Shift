// Fill the dots one after the other with a color
void setColours(uint32_t c, uint8_t wait, int setNumber) {
  for(uint16_t i=0; i<setNumber; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
