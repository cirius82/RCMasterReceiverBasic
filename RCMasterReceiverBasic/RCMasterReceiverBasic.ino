/**
 * Teensy 3.2
 * Test um daten mit einem Request zu übertragen
 * Datum 12.01.2022
 * pin-out:
 * 3 - Servo1
 * 4 - Servo2
 * 5 - Servo3
 * 6 - Servo4
 * 23 - LED1
 * 22 - LED2
 * pin-in:
 * 21 - S1A (S1 näher an servo Stecker)
 * 20 - S1B
 * 19 - S2A (S2 näher an HighCurrentOutput)
 * 18 - S2B // RPM pin
 * 
 * 21 - current meter (analog in)
 * 20 - Reflex IR für rpm (digital in)
 * 16 - voltage (analog in)
 */

 /* -- Includes ------------------------------------------------------------ */
#include <EEPROM.h>

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */
#define SIGNEDOFFSET 32768
#define FAILSAFE 500

/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */
uint8_t _transmitIdCounter = 0;
unsigned long _lastReceived = 0;
uint16_t _failSaveX1 = 1500;
uint16_t _failSaveY2 = 1500;

/* -- Setter / Getter ----------------------------------------------------- */

/* -- Public Functions ---------------------------------------------------- */
void setup()
{
  Serial.begin(115200);
  if(NRFLite_setup() != 0) {
    Serial.println("nRF24 error");
  }
  Servo_setup();
  GPIO_setup();
  _lastReceived = millis();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis -_lastReceived >= FAILSAFE) {
    SetServo(_failSaveX1);
    SetESC(_failSaveY2);
  }
  
  //bool gpio = false;
  if (GPIO_Cyclic()){
    uint16_t voltage = GPIOGetVoltage();
    uint16_t current = GPIOGetCurrent();
    uint16_t revolutions = GPIOGetVelocity();
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.print(", Current: ");
    Serial.print(current);
    Serial.print(", Revolutions ");
    Serial.println(revolutions);
  }

  if (GPIO_RPM_Cyclic()) {
    Serial.print("Revolutions ");
    Serial.println(GPIOGetVelocity());
  }
  
  bool dataReceived = false;
  bool dataSend = false;
  if (NRFLite_Cyclic(&dataReceived, &dataSend) == 0)
  {
    if(dataReceived)
    {
      _lastReceived = currentMillis;
    // werte abrufen
      uint8_t inDataType = 0;
      uint16_t inDataValue1 = 0;
      uint16_t inDataValue2 = 0;
      uint8_t inDataValue3 = 0;
  
      // nur wenn auch wirklich Daten geholt wurden
      GetIntData(&inDataType, &inDataValue1, &inDataValue2, &inDataValue3);
      //uint16_t servoValue = map(inDataValue1, 1, 1, 255, 180);

//      Serial.print("Received ");
//      Serial.print(inDataType);
//      Serial.print(", Val1: ");
//      Serial.print(inDataValue1);
//      Serial.print(", Val2: ");
//      Serial.print(inDataValue2);
//      Serial.print(", Val3: ");
//      Serial.println(inDataValue3 + ", ");
      
      if (inDataType == 0) {          // normal signal
        SetServo(inDataValue1);
        SetESC(inDataValue2);
      } else if (inDataType == 1) {   // setup servo
        int servoMoveValue = inDataValue1 - SIGNEDOFFSET;
        if (servoMoveValue != 0) {
          MoveServo(inDataValue1);
        }
        int servoRangeValue = inDataValue2 - SIGNEDOFFSET;
        if (servoRangeValue != 0) {
          ChangeServoRange(servoRangeValue);
        }
      } else if (inDataType == 2) {   // setup esc
        int escMoveValue = inDataValue1 - SIGNEDOFFSET;
        if (escMoveValue != 0) {
          MoveEsc(escMoveValue);
        }
        int escRangeValue = inDataValue2 - SIGNEDOFFSET;
        if (escRangeValue != 0) {
          ChangeEscRange(escRangeValue);
        }
      }
    }
    
    if (dataSend) {
      //uint8_t inDataType = 0;
      uint16_t inDataValue1 = 0;
      uint16_t inDataValue2 = 0;
      uint8_t inDataValue3 = 0;

      if (_transmitIdCounter == 0) {
        inDataValue1 = GPIOGetVoltage();
      } else if (_transmitIdCounter == 1) {
        inDataValue1 = GPIOGetCurrent();
      } else if (_transmitIdCounter == 2) {
        inDataValue1 = GPIOGetVelocity();
      } else {
        inDataValue1 = 0;
      }
      _transmitIdCounter++;
      if (_transmitIdCounter > 2) {
        _transmitIdCounter = 0;
      }
      SetOutData(_transmitIdCounter, inDataValue1, inDataValue2, inDataValue3); 
    }
  } else {
//    Serial.println("receive error");
  }


  // Servo
  bool Servo_hasWorked = false;
  if (Servo_Cyclic(&Servo_hasWorked) != 0) {
    // fail
  } else if (Servo_hasWorked) {
    
  }
}

/* -- Private Functions --------------------------------------------------- */
