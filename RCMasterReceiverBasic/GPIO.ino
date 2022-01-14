
/* -- Includes ------------------------------------------------------------ */

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */

#define INTERVAL 249
#define REVOLUTIONS_INTERVAL 999 // high for testing
#define CURRENTPIN 21
#define BATTPIN 20
#define RPMPIN 18

/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */
uint32_t _gpioLastAction = 0;
uint32_t _gpioLastRevolutions = 0;
uint16_t _voltage = 1;//12600; // mV
uint16_t _current = 2;//65500; // mA
uint16_t _velocity = 0;//65000; // m/H
uint16_t _revolutions = 0;
uint16_t _revolutionsCounter = 0;
bool _prevState = false;

/* -- Setter / Getter ----------------------------------------------------- */
uint16_t GPIOGetVoltage(){return _voltage;}   // mV
uint16_t GPIOGetCurrent(){return _current;}
uint16_t GPIOGetVelocity(){return _revolutions;}

/* -- Public Functions ---------------------------------------------------- */
uint8_t GPIO_setup()
{
  analogReadResolution(12); // max 13 , 12==4096
  pinMode(RPMPIN, INPUT_PULLUP);
  _gpioLastAction = millis();
  _gpioLastRevolutions = millis();

  return 0;  
}

bool GPIO_Cyclic()
{
  bool retVal = false; 
  if ((_gpioLastAction + INTERVAL) < millis())
  {
    retVal = true;
    _gpioLastAction = millis();
    _voltage = analogRead(BATTPIN);
    _current = analogRead(CURRENTPIN);
  }
return retVal;
}
bool GPIO_RPM_Cyclic()
{
  bool retVal = false; 
  bool buttonState = digitalRead(RPMPIN);
  if(buttonState != _prevState) {
    _prevState = buttonState;
    _revolutionsCounter++;
  }
  uint32_t current = millis();
  if ((_gpioLastRevolutions + REVOLUTIONS_INTERVAL) < current)
  {
    _gpioLastRevolutions = current;
    _revolutions = _revolutionsCounter;
    _revolutionsCounter = 0;
    retVal = true;
  }
  return retVal;
}
/* -- Private Functions --------------------------------------------------- */
