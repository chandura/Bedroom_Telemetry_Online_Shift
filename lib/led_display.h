//This is the .h file for the library to display the digites on the LED display

class LEDDisplay
{
  public:
    Morse(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};
