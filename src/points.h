

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

class Points : public DisplayElement
{
public:
  Points(CRGB *firstPixel) : DisplayElement(firstPixel, 2)
  {
    className = __func__;
    Serial.print("Points : ");
    for (uint8_t i = 0; i < 2; i++)
    {
      Serial.printf("Pixel[%d]", firstPixel[i].red);
    }
    Serial.println();
  }
};

#endif
