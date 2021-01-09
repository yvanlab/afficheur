

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
#include "manageElements.h"

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

/*#define DIZAINE 0
#define UNITE 1*/

class DoubleDigit : public DisplayElement, public ManageElements
{
  public:
    enum DIGIT {DIZAINE=0, UNITE=1, LAST_DIGIT_ELT=2 };
    DoubleDigit(CRGB *firstPixel)  {
        //className = __func__;
        m_elt[DIZAINE] = new Digit(firstPixel);
        m_elt[UNITE] = new Digit(&firstPixel[m_elt[DIZAINE]->getNbPixels()]);
        m_nbElts = 2;
        //init(m_elt,LAST_DIGIT_ELT);
    };

    void setValue(uint8_t iValue, int8_t iElt = -1){
      uint8_t dizaine = iValue / 10;
      uint8_t unite = iValue % 10;
      ManageElements::setValue(dizaine, DIZAINE);
      ManageElements::setValue(unite, UNITE);
      /*m_elt[DIZAINE]->setValue(dizaine); 
      m_elt[UNITE]->setValue(unite);*/

    };

    /*void  setState(boolean bON) {
      m_elt[DIZAINE]->setState(bON);
      m_elt[UNITE]->setState(bON);  
    }
    
    void HandleMode() {
      m_elt[DIZAINE]->HandleMode();
      m_elt[UNITE]->HandleMode();
    }

    void setMode(MODE_LED mode){
       m_elt[DIZAINE]->setMode(mode);
        m_elt[UNITE]->setMode(mode);
    };
*/

    /*void setColor(CRGB newcolor) {
      m_elt[DIZAINE]->setColor(newcolor); 
      m_elt[UNITE]->setColor(newcolor); 
    };*/
  

  private:
  //  Digit *m_elt[2];
};


#endif
