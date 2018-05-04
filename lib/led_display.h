//This is the .h file for the library to display the digites on the LED display

class LEDDisplay
{
  public:
    Morse(int pin);
    void one();
    void two();
    void three();
    void four();
    void five();
    void six();
    void seven();
    void eight();
    void nine();
    void zero();
  private:
    int _pin;
};
