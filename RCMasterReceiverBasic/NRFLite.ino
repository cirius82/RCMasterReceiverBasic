
#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 0;
const static uint8_t DESTINATION_RADIO_ID = 1;
const static uint8_t PIN_RADIO_CE = 15; // blau
const static uint8_t PIN_RADIO_CSN = 14; // gruen
//const static uint8_t PIN_RED_LED = 13; // n/a SCK pin
uint32_t _timer = 0; 

//class _spi = SPIClass;

SPIClass mySPI = SPI;

enum RadioPacketType
{
    AcknowledgementData, // Produced by the primary receiver and provided to the transmitter via an acknowledgement data packet.
    Heartbeat,    // Sent by the primary transmitter.
    BeginGetData, // Sent by the primary transmitter to tell the receiver it should load itself with an acknowledgement data packet.
    EndGetData    // Sent by the primary transmitter to receive the acknowledgement data packet from the receiver.
};

struct RadioPacket
{
    RadioPacketType PacketType;
    uint8_t FromRadioId;
    uint8_t DataType;
    uint16_t Value1;
    uint16_t Value2;
    uint8_t Value3;
};

NRFLite _radio;
uint8_t _outDataType = 0;
uint16_t _outDataValue1 = 0;
uint16_t _outDataValue2 = 0;
uint8_t _outDataValue3 = 0;

uint8_t _inDataType = 0;
uint16_t _inDataValue1 = 0;
uint16_t _inDataValue2 = 0;
uint8_t _inDataValue3 = 0;

///--- Setter/Getter ---///
void SetOutData(uint8_t type, uint16_t value1, uint16_t value2, uint8_t value3)
{
  _outDataType = type;
  _outDataValue1 = value1;
  _outDataValue2 = value2;
  _outDataValue3 = value3;
}

void GetOutData(uint8_t *type, uint16_t *value1, uint16_t *value2, uint8_t *value3)
{
  *type = _outDataType;
  *value1 = _outDataValue1;
  *value2 = _outDataValue2;
  *value3 = _outDataValue3;
}

void GetIntData(uint8_t *type, uint16_t *value1, uint16_t *value2, uint8_t *value3)
{
  *type = _inDataType;
  *value1 = _inDataValue1;
  *value2 = _inDataValue2;
  *value3 = _inDataValue3;
}

uint8_t NRFLite_setup()
{
  uint8_t retVal = 0;
  _timer = millis();
    if (!_radio.init(&mySPI, RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))//, &_mySPI))
    {
        Serial.println("Cannot communicate with radio");
        retVal = 1;
    }
    return retVal;
}

uint8_t NRFLite_Cyclic(bool *dataReceived, bool *dataSend)
{
  uint8_t retVal = 0;
  
    while (_radio.hasData())
    {
      *dataReceived = true;
      _timer = millis();
        RadioPacket radioData;
        _radio.readData(&radioData);

        if (radioData.PacketType == Heartbeat)
        {
          _inDataType = radioData.DataType;
          _inDataValue1 = radioData.Value1;
          _inDataValue2 = radioData.Value2;
          _inDataValue3 = radioData.Value3;
        }
        else if (radioData.PacketType == BeginGetData)
        {
          _inDataType = radioData.DataType;
          _inDataValue1 = radioData.Value1;
          _inDataValue2 = radioData.Value2;
          _inDataValue3 = radioData.Value3;

            RadioPacket ackData;
            ackData.PacketType = AcknowledgementData;
            ackData.FromRadioId = RADIO_ID;

            ackData.DataType = _outDataType;
            ackData.Value1 = _outDataValue1;
            ackData.Value2 = _outDataValue2;
            ackData.Value3 = _outDataValue3;

            // Add the data to send back to the transmitter into the radio.
            // The next packet we receive will be acknowledged with this data.
            _radio.addAckData(&ackData, sizeof(ackData));
            *dataSend = true;
        }
        else if (radioData.PacketType == EndGetData)
        {
            // The transmitter hopefully received the acknowledgement data packet at this point.
        }
    }
    return retVal;
}
