
/* -- Includes ------------------------------------------------------------ */
#include <Servo.h>

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */
#define INTERVAL 249
#define SERVOPIN 3
#define ESCPIN 4

#define OUTMAXVALUE 2400
#define OUTMINVALUE 544
// center at 1500

#define EEPROMADDR_SERVOMIN 0
#define EEPROMADDR_SERVOMAX 2
#define EEPROMADDR_ESCMIN 4
#define EEPROMADDR_ESCMAX 6 

/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */
Servo myServo;
Servo myESC;
uint16_t _servoMin = 544;
uint16_t _servoMax = 2400;
uint16_t _escMin = 544;
uint16_t _escMax = 2400;

uint32_t _servoLastAction = 0;

/* -- Setter / Getter ----------------------------------------------------- */

/* -- Public Functions ---------------------------------------------------- */
uint8_t Servo_setup()
{  
  myServo.attach(SERVOPIN);//, _servoMin, _servoMax);
  myESC.attach(ESCPIN);//, _escMin, _escMax);

  return 0;  
}

uint8_t SetServo(uint16_t angle)
{ 
  myServo.write(angle);
  return 0;
}

uint8_t SetESC(uint16_t velocity)
{
  myESC.write(velocity);
  return 0;
}
/* -- Private Functions --------------------------------------------------- */
