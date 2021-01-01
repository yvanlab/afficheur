

#ifndef points_h
#define points_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"


class Points  : public DisplayElement
{
  public:
    Points(CRGB *firstPixel) : DisplayElement(firstPixel) {
       
    }

    void display() {
        for (uint8_t i=0; i<getNbPixels(); i++) {
            m_firstPixel[i] = m_color;
        }
    };

    static uint8_t getNbPixels() {
      return 2;
    }

};


#endif
