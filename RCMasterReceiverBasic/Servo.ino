
#include <Servo.h>

///---DEFINES ---///
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
///--- TYPES ---///

///--- VARIABLES ---///
uint32_t _servoLastAction = 0;

Servo myServo;
Servo myESC;
uint16_t _servoMin = 544;
uint16_t _servoMax = 2400;
uint16_t _escMin = 544;
uint16_t _escMax = 2400;

///--- Setter/Getter ---///

///--- METHODS ---///

uint8_t Servo_setup()
{
  // read values from eeprom
  _servoMin = Read_UInt16(EEPROMADDR_SERVOMIN);
  _servoMax = Read_UInt16(EEPROMADDR_SERVOMAX);
  _escMin = Read_UInt16(EEPROMADDR_ESCMIN);
  _escMax = Read_UInt16(EEPROMADDR_ESCMAX);

//1st run:
//Save_UInt16(_servoMin, 0);
//Save_UInt16(_servoMax, 2);
//Save_UInt16(_escMin, 4);
//Save_UInt16(_escMax, 6);
  
  myServo.attach(SERVOPIN);//, _servoMin, _servoMax);
  myESC.attach(ESCPIN);//, _escMin, _escMax);

  return 0;  
}

uint8_t Servo_Cyclic(bool *hasWorked)
{
  return 0;
}
uint8_t SetServo(uint16_t angle)
{ 
//  if (angle >= 0 && angle <= 180) {
    myServo.write(angle);
//  } else {
//    return 1;
//  }
  return 0;
}

uint8_t SetESC(uint16_t velocity)
{
//  if (velocity >= 0 && velocity <= 180) {
    myESC.write(velocity);
//  } else {
//    return 1;
//  }
  return 0;
}

uint8_t MoveServo(int val)
{
  _servoMin += val;
  if (_servoMin < OUTMINVALUE) {
    _servoMin = OUTMINVALUE;
  }
  _servoMax += val;
  if (_servoMax > OUTMAXVALUE) {
    _servoMax = OUTMAXVALUE;
  }
  Save_UInt16(_servoMin, 0);
  Save_UInt16(_servoMax, 2);
  UpdateServo();
  
  return 0;
}

uint8_t ChangeServoRange(int val) // increase if positive, decrease vice versa
{
  _servoMin -= val;
  if (_servoMin < OUTMINVALUE) {
    _servoMin = OUTMINVALUE;
  }
  _servoMax += val;
  if (_servoMax > OUTMAXVALUE) {
    _servoMax = OUTMAXVALUE;
  }
  Save_UInt16(_servoMin, 0);
  Save_UInt16(_servoMax, 2);
  UpdateServo();
  
  return 0;
}

uint8_t MoveEsc(int val)
{
  _escMin += val;
  if (_escMin < OUTMINVALUE) {
    _escMin = OUTMINVALUE;
  }
  _escMax += val;
  if (_escMax > OUTMAXVALUE) {
    _escMax = OUTMAXVALUE;
  }
  Save_UInt16(_escMin, 4);
  Save_UInt16(_escMax, 6);
  UpdateESC();
  
  return 0;
}

uint8_t ChangeEscRange(int val)
{
  _escMin -= val;
  if (_escMin < OUTMINVALUE) {
    _escMin = OUTMINVALUE;
  }
  _escMax += val;
  if (_escMax > OUTMAXVALUE) {
    _escMax = OUTMAXVALUE;
  }
  Save_UInt16(_escMin, 4);
  Save_UInt16(_escMax, 6);
  UpdateESC();
  
  return 0;
}

uint16_t Read_UInt16(uint8_t addr)
{
  uint16_t val =0;

  byte byte1 = EEPROM.read(addr);
  byte byte2 = EEPROM.read(addr + 1);
  val = (uint16_t)((byte1 << 8) + byte2);
  return val;
}

void Save_UInt16(uint16_t val, uint8_t addr)
{
  EEPROM.write(addr, highByte(val));
  EEPROM.write(addr + 1, lowByte(val));
}

void UpdateServo()
{
  myServo.detach();
  myServo.attach(SERVOPIN, _servoMin, _servoMax);
}

void UpdateESC()
{
  myESC.detach();
  myESC.attach(SERVOPIN, _servoMin, _servoMax);
}
