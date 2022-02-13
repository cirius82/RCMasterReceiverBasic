/* -- Includes ------------------------------------------------------------ */
#include <EEPROM.h>

/* -- Private Function Prototypes ----------------------------------------- */

/* -- Private Defines ----------------------------------------------------- */
#define EEPROMADDR_SAVESERVO 2
#define EEPROMADDR_SAVEESC 4
/* -- Private Types ------------------------------------------------------- */

/* -- Private Global Variables -------------------------------------------- */

/* -- Setter / Getter ----------------------------------------------------- */

/* -- Public Functions ---------------------------------------------------- */
void SaveFailsaveValueESC(uint16_t val)
{
  Save_UInt16(val, EEPROMADDR_SAVEESC);
}
void SaveFailsaveValueServo(uint16_t val)
{
  Save_UInt16(val, EEPROMADDR_SAVESERVO);
}
uint16_t LoadFailsaveValueESC()
{
  return Read_UInt16(EEPROMADDR_SAVEESC);
}
uint16_t LoadFailsaveValueServo()
{
  return Read_UInt16(EEPROMADDR_SAVESERVO);
}

/* -- Private Functions --------------------------------------------------- */

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
