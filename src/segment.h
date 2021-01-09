

#ifndef segment_h
#define segment_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "displayElement.h"


/*
struct MY_CRGB {
  CRGB crgb;
  uint8_t iPixel;
}
*/


class Segment : public DisplayElement
{
  public:
    Segment(CRGB *firstPixel) : DisplayElement(firstPixel,3) {
      className = __func__;
      for (uint8_t i=0; i<3;i++) {
        Serial.printf("Pixel[%d]",firstPixel[i].red);
      }
    }

    /*void display() {
        for (uint8_t i=0; i<getNbPixels(); i++) {
            m_firstPixel[i] = m_color;
        }
    };*/

   

};


#endif
