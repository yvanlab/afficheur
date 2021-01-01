

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
      Serial.printf("Seg[%d] : ",s);
      m_seg[s] = new Segment(nextPixel);
      nextPixel = &nextPixel[Segment::getNbPixels()] ;
      Serial.println();
    }
    m_color = CRGB(255,0,0);
   
  };

  
  void displayValue(uint8_t value){
    Serial.printf("displayValue[%d]\n",value);
    uint8_t mask = 0x00000001;
    for (uint8_t i=0 ; i<7;i++) {
      //Serial.printf("MASK[%x][%x]",mask,digitMapping[value]);
      if ((digitMapping[value]  &  mask) != 0) {
        m_seg[i]->setColor(m_color);
        //Serial.printf("Seg[%d]:D]",i);
      } else {
        m_seg[i]->setColor(CRGB(0,0,0));
        //Serial.printf("Seg[%d]:N]",i);
      }
      m_seg[i]->display();
      Serial.println();
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
