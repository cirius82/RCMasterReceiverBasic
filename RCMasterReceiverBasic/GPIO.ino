/**
 * high for testing 64-66 ~1meter (assume 62)
 * min 1mk/h -> 0,2777m/s -> 17,777 Impl/s -> 56ms-Impulse duration
 * max 59km/h -> 13,888m/s -> 888,888 Impl/s -> 1,125ms-Impl_dur
 * 1km/h ist mit micros in uint16_t nicht abbildbar!
 * TEST MIT MILLIS
 */

/* -- Includes ------------------------------------------------------------ */

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */

#define INTERVAL 249
#define R_INTERVAL_MILLIS 999 // max Impulse duration
//#define R_INTERVAL_MICROS 99999 // max Impuulse duration
#define FILTER_WIDTH 16
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
bool _prevState = false;

// speedmeter
uint32_t _revolutionTimer = 0;
uint32_t _filteravg = 0;
uint32_t _revolutions = 0;

/* -- Setter / Getter ----------------------------------------------------- */
uint16_t GPIOGetVoltage(){return _voltage;}   // mV
uint16_t GPIOGetCurrent(){return _current;}
uint16_t GPIOGetVelocity(){
  if (_revolutions > 0xFFFF) {
    _revolutions = 0xFFFF;
  }
  return (uint16_t)(_revolutions);
 }

/* -- Public Functions ---------------------------------------------------- */
uint8_t GPIO_setup()
{
  analogReadResolution(12); // max 13 , 12==4096
  analogWriteFrequency(23, 12000);
  pinMode(RPMPIN, INPUT_PULLUP);
  _gpioLastAction = millis();
  _gpioLastRevolutions = millis();
  return 0;  
}

bool GPIO_Cyclic()
{
  bool retVal = false; 
  if ((millis() - _gpioLastAction) > INTERVAL)
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
  uint32_t current = micros();
  
  if(buttonState != _prevState) {
    _prevState = buttonState;
    FilterCalculate(current - _gpioLastRevolutions);
    _gpioLastRevolutions = current;
  }
  
  if (_gpioLastRevolutions < ( current - 0xFFFF)) {
//    _gpioLastRevolutions = current;
    FilterCalculate(current - _gpioLastRevolutions);
        _gpioLastRevolutions = current;
  }
  
  return retVal;
}

/* -- Private Functions --------------------------------------------------- */

void FilterCalculate(uint32_t ms)
{
  _filteravg -= _filteravg / FILTER_WIDTH;
  _filteravg += ms;
  _revolutions = _filteravg / FILTER_WIDTH;
}
