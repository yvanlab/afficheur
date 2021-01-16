

#ifndef DisplayDigit_h
#define DisplayDigit_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "displayBase.h"
#include "displayComponent.h"
#include "displaySegment.h"
#include <iterator>

/*
        3
      *****
     *2    *4
     *  5  *
       ****
     *1    *6
     *   0 *
       ****

*/

class DisplayDigit : public DisplayComponent
{
public:
  //
  DisplayDigit(CRGB *firstPixel) : DisplayComponent(firstPixel)
  {
    className = __func__;
    m_nbLeds = DisplayDigit::getNbPixels();
    add(firstPixel);
  };

  static uint8_t getNbPixels() 
  {
    return 7*DisplaySegment::getNbPixels();
  }


  virtual void add(CRGB *firstPixel)
  {
    DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
    CRGB *nextPixel = firstPixel;
    for (uint8_t i = 0; i < 7; i++)
    {
      m_listComponent.push_back(new DisplaySegment(nextPixel));
      nextPixel = &nextPixel[DisplaySegment::getNbPixels()];
    }
  }

  virtual void setValue(uint8_t value, int8_t iSelected = -1)
  {
    DEBUGLOGF("setValue[%d]\n", value);
    uint8_t mask = 0x00000001;
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      if ((DisplayDigitMapping[value] & mask) != 0)
        (*itb)->setValue(255, iSelected);
      else
        (*itb)->setValue(0, iSelected);
      mask = mask << 1;
    }
  }

private:
  const uint8_t DisplayDigitMapping[10] = {
      0b01011111, //0
      0b01010000, //1
      0b00111011, //2
      0b01111001, //3
      0b01100100, //4
      0b01101101, //5
      0b01101111, //6
      0b01011000, //7
      0b01111111, //8
      0b01111101  //9
  };
};

#endif
