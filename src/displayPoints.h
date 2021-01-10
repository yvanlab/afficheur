

#ifndef DisplayPoints_h
#define DisplayPoints_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "displaySegment.h"


/*
struct MY_CRGB {
  CRGB crgb;
  uint8_t iPixel;
}
*/


class DisplayPoints : public DisplaySegment
{
  public:
    DisplayPoints(CRGB *firstPixel) : DisplaySegment(firstPixel) {
      className = __func__;
      m_nbLeds = DisplayPoints::getNbPixels();
      add(firstPixel);
    }

  static uint8_t getNbPixels() 
  {
    return 2*DisplayBase::getNbPixels();
  }


};


#endif
