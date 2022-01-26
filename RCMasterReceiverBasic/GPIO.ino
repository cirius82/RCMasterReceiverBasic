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
#define HIGHCURRENTPINA 22
#define HIGHCURRENTPINB 23

/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */
uint32_t _gpioLastAction = 0;
uint32_t _gpioLastRevolutions = 0;
uint16_t _voltage = 1;//12600; // mV
uint16_t _current = 2;//65500; // mA
uint16_t _velocity = 0;//65000; // m/H
uint32_t _revolutions = 0;
//uint16_t _revolutionsCounter = 0;
bool _prevState = false;

// new for speedmeter
uint32_t _revolutionTimer = 0;
uint32_t _filteravg = 0;
//uint8_t _filterWidth = 16; // ^2 would be compiler optimized....
//uint32_t _maxDifference = 4999; // micros

/* -- Setter / Getter ----------------------------------------------------- */
uint16_t GPIOGetVoltage(){return _voltage;}   // mV
uint16_t GPIOGetCurrent(){return _current;}
uint16_t GPIOGetVelocity(){return _revolutions;}

/* -- Public Functions ---------------------------------------------------- */
uint8_t GPIO_setup()
{
  analogReadResolution(12); // max 13 , 12==4096
  analogWriteFrequency(23, 12000);
  pinMode(RPMPIN, INPUT_PULLUP);
  _gpioLastAction = millis();
  _gpioLastRevolutions = millis();
  //analogWrite(HIGHCURRENTPINA, 200);
  //analogWrite(HIGHCURRENTPINB, 200);
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
  uint32_t current = millis();
  
  if(buttonState != _prevState) {
    _prevState = buttonState;
    FilterCalculate(current);
    _gpioLastRevolutions = current;
  }
  
  if (_gpioLastRevolutions < ( current - R_INTERVAL_MILLIS)) {
    _gpioLastRevolutions = current;
    FilterCalculate(current);
  }
  
  return retVal;
}

/* -- Private Functions --------------------------------------------------- */

void FilterCalculate(uint32_t ms_notUsed)
{
  uint32_t ms = micros();
  
  //uint32_t difference = ms - _revolutionTimer;
  _revolutionTimer = ms;
  _revolutions = ms - _revolutionTimer;
  //_revolutions = difference;
  
// Temporarly disable filter  
//  uint32_t tmp = _filteravg / FILTER_WIDTH;
//  _filteravg = (tmp * (FILTER_WIDTH - 1)) + difference;
//  _revolutions = tmp;
}
