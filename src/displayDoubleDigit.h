

#ifndef DisplayDoubleDigit_h
#define DisplayDoubleDigit_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "displayBase.h"
#include "displayComponent.h"
#include "displayDigit.h"
#include <iterator>

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

class DisplayDoubleDigit : public DisplayComponent
{
public:
  enum DOUBLE_DIGIT
  {
    DIZAINE = 0,
    UNITE = 1,
    LAST_DIGIT_ELT = 2
  };
  enum DOUBLE_DIGIT_COMMAND
  {
    DASH_BOTH    = 100,
    DASH_DIZAINE = 101,
    DASH_UNITE   = 102,
    UNITY_ONLY   = 110, // + the vqlue to displqy
  };

  DisplayDoubleDigit(CRGB *firstPixel) : DisplayComponent(firstPixel)
  {
    className = __func__;
    m_nbLeds = DisplayDoubleDigit::getNbPixels();
    add(firstPixel);
  };


  static uint8_t getNbPixels() 
  {
    return 2*DisplayDigit::getNbPixels();
  }

  virtual void add(CRGB *firstPixel)
  {
    //DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
    CRGB *nextPixel = firstPixel;
    for (uint8_t i = 0; i < 2; i++)
    {
      m_listComponent.push_back(new DisplayDigit(nextPixel));
      nextPixel = &nextPixel[DisplayDigit::getNbPixels()];
    }
  }

  virtual void setValue(uint8_t value , int8_t iSelected = -1)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    if (value == DASH_BOTH) {
      m_listComponent[DIZAINE]->setValue(DisplayDigit::DASH_MIDLE);
      m_listComponent[UNITE]->setValue(DisplayDigit::DASH_MIDLE);
    } else if (value == DASH_DIZAINE) {
      m_listComponent[DIZAINE]->setValue(DisplayDigit::DASH_MIDLE);
    } else  if (value == DASH_UNITE) {
      m_listComponent[UNITE]->setValue(DisplayDigit::DASH_MIDLE);
    } else if  (value>=UNITY_ONLY) {
        m_listComponent[DIZAINE]->setState(false);
        m_listComponent[UNITE]->setValue(value-UNITY_ONLY);
    } else {
      if (iSelected == -1) {
        uint8_t dizaine = value / 10;
        uint8_t unite = value % 10;
        m_listComponent[DIZAINE]->setValue(dizaine, iSelected);
        m_listComponent[UNITE]->setValue(unite,iSelected);
      } else {
        m_listComponent[iSelected]->setValue(value);
      }
    }
    m_value = value;
  }

private:
};

#endif
