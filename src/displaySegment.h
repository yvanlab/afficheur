

#ifndef Display_Segment_h
#define Display_Segment_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "displayBase.h"

/*
struct MY_CRGB {
  CRGB crgb;
  uint8_t iPixel;
}
*/

class DisplaySegment : public DisplayComponent
{
public:
  DisplaySegment(CRGB *firstPixel) : DisplayComponent(firstPixel)
  {
    className = __func__;
    m_nbLeds = DisplaySegment::getNbPixels();
    add(firstPixel);
  }

  static uint8_t getNbPixels() 
  {
    return 3*DisplayBase::getNbPixels();
  }


  virtual void setValue(uint8_t value, int8_t iSelected)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();
    uint8_t mask = 0b00000001;
    for (; itb != ite; itb++)
    {
      (*itb)->setState(((value & mask) != 0), iSelected);
      mask = mask << 1;
    }
    setState(value!=0);
    m_value = value;
  }

  virtual void add(CRGB *firstPixel)
  {
    DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
    CRGB *nextPixel = firstPixel;
    for (uint8_t i = 0; i < m_nbLeds; i++)
    {
      m_listComponent.push_back(new DisplayBase(nextPixel));
      nextPixel = &nextPixel[DisplayBase::getNbPixels()];
    }
  }
};

#endif
