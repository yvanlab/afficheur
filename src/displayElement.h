

#ifndef DisplayElement_h
#define DisplayElement_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"


class DisplayElement 
{
  public:
    DisplayElement(CRGB *firstPixel) {
        m_firstPixel = firstPixel;
    }

    void setColor(CRGB newcolor){
        m_color = newcolor;
    };


  protected:
    CRGB *m_firstPixel;
    CRGB m_color ={255,0,0};
};


#endif
