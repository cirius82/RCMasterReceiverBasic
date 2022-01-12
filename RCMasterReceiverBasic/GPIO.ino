
/* -- Includes ------------------------------------------------------------ */

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */

#define INTERVAL 249
#define BATTPIN 16
#define CURRENTPIN 21
#define RPMPIN 20

/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */
uint32_t _gpioLastAction = 0;
uint16_t _voltage = 1;//12600; // mV
uint16_t _current = 2;//65500; // mA
uint16_t _velocity = 0;//65000; // m/H

/* -- Setter / Getter ----------------------------------------------------- */
uint16_t GPIOGetVoltage(){return _voltage;}   // mV
uint16_t GPIOGetCurrent(){return _current;}
uint16_t GPIOGetVelocity(){return _velocity;}

/* -- Public Functions ---------------------------------------------------- */
uint8_t GPIO_setup()
{

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
/* -- Private Functions --------------------------------------------------- */
