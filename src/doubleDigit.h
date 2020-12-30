

#ifndef doubledigit_h
#define doubledigit_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "digit.h"

/*
         1            2

      *****         *****
     *     *       *     *
     *     *       *     * 
       ****         *****
     *     *       *     *
     *     *       *     *
      *****         *****

*/

#define DIZAINE 0
#define UNITE 1

class DoubleDigit : public DisplayElement
{
  public:
    DoubleDigit(CRGB *firstPixel) : DisplayElement(firstPixel) {
        m_digits[DIZAINE] = new Digit(firstPixel);
        m_digits[UNITE] = new Digit(firstPixel+m_digits[0]->getNbPixels()*sizeof(CRGB));
    };

    void displayValue(uint8_t value) {
      uint8_t dizaine = value / 10;
      uint8_t unite = value % 10;
      m_digits[DIZAINE]->displayValue(dizaine); 
      m_digits[UNITE]->displayValue(unite);

    };

    void setColor(CRGB newcolor) {
      m_digits[DIZAINE]->setColor(newcolor); 
      m_digits[UNITE]->setColor(newcolor); 
    };
    
    static uint8_t getNbPixels() {
      return  2*Digit::getNbPixels();
    }

  private:
    Digit *m_digits[2];
};


#endif
