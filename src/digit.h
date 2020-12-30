

#ifndef digit_h
#define digit_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "segment.h"

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

class Digit : public DisplayElement
{
public:

  //
  Digit(CRGB *firstPixel) : DisplayElement(firstPixel)  {
    CRGB *nextPixel = firstPixel;
    for (uint s=0; s<7 ; s++) {
      m_seg[s] = new Segment(nextPixel);
      nextPixel = nextPixel + Segment::getNbPixels()*sizeof(CRGB);
      m_color = CRGB(255,0,0);
    }
  };

  
  void displayValue(uint8_t value){
    uint8_t mask = 0x00000001;
    for (uint8_t i=0 ; i<7;i++) {
      if (digitMapping[value]  &  mask) {
        m_seg[i]->setColor(m_color);
      } else {
        m_seg[i]->setColor(CRGB(0,0,0));
      }
      m_seg[i]->display();
      mask = mask << 1;
    }

  };

  static uint8_t getNbPixels() {
    return  7*Segment::getNbPixels();
  }

private:
  Segment *m_seg[7];
  //CRGB m_color ={255,0,0};
  const uint8_t digitMapping[10] = {
      (uint8_t)0x01011111, //0
      (uint8_t)0x00110000, //1
      (uint8_t)0x00111011, //2
      (uint8_t)0x01111001, //3
      (uint8_t)0x01100010, //4
      (uint8_t)0x01101100, //5
      (uint8_t)0x01101111, //6
      (uint8_t)0x01011000, //7
      (uint8_t)0x01111111, //8
      (uint8_t)0x01111101  //9
  };
};

#endif
