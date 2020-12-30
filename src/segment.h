

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


class Segment : public DisplayElement
{
  public:
    Segment(CRGB *firstPixel) : DisplayElement(firstPixel) {
      ;
    }

    void display() {
        for (uint8_t i; i<getNbPixels(); i++) {
            m_firstPixel[i] = m_color;
        }
    };

    static uint8_t getNbPixels() {
      return 3;
    }

};


#endif
