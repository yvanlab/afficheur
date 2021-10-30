

#ifndef DisplayDigit_h
#define DisplayDigit_h

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
#include "displaySegment.h"
#include <iterator>

/*
        3
        ***
      *     *  
    ^ *2    *4 
      *  5  *
        ***
      *     * 
    ^ *1    *6
      *  0  *
        ***


*/

class DisplayDigit : public DisplayComponent
{
public:
  enum DIGIT_COMMAND
  {
    DASH_MIDLE = 10,
    DASH_TOP = 11,
    DASH_BOTTOM = 12
  };

  //
  DisplayDigit(CRGB *firstPixel) : DisplayComponent(firstPixel)
  {
    className = __func__;
    m_nbLeds = DisplayDigit::getNbPixels();
    add(firstPixel);
  };

  static uint8_t getNbPixels()
  {
    return 7 * DisplaySegment::getNbPixels();
  }

  virtual void add(CRGB *firstPixel)
  {
    //DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
    CRGB *nextPixel = firstPixel;
    for (uint8_t i = 0; i < 7; i++)
    {
      m_listComponent.push_back(new DisplaySegment(nextPixel));
      nextPixel = &nextPixel[DisplaySegment::getNbPixels()];
    }
  }

  virtual void setValue(uint8_t value, int8_t iSelected = -1)
  {
    uint8_t mask = 0x00000001;

    if ( (m_value != value) && (m_transition == DisplayBase::TRANSITION_MOVE_UP)) {
      DEBUGLOGF("value[%d] m_value[%d]\n", value, m_value);
      m_iPos = 0;
    }

    for (uint8_t i = 0; i < 7; i++)
    {
      if ((DisplayDigitMapping[value] & mask) != 0)
      {
        uint8_t v = getValueForAnnimation(i);
        //DEBUGLOGF("moveUp p[%d] v[%d] i[%d] m[%d]\n", m_iPos, m_value, i, v);
        m_listComponent[i]->setValue(v, iSelected);
      }
      else
      {
        m_listComponent[i]->setValue(0, iSelected);
      }
      mask = mask << 1;
    }

    m_isOn  = (value!=0);
    m_value = value;
  }

  uint8_t getValueForAnnimation(uint8_t index)
  {

    if (m_transition != DisplayBase::TRANSITION_MOVE_UP || (m_iPos == 8) )
      return 255;

    uint8_t iMaskTop = 0b11111111 >> (7 - m_iPos);    //0000 0001
    uint8_t iMaskBottom = 0b11111111 << (7 - m_iPos); //1000 0000
                                                      //DEBUGLOGF("handleMode [%d][%d]",iMask,iMask&0b111);
    switch (index)
    {
    case 0: //horizontal bottom
      if (iMaskTop & 0b0001)
        return 255;
      else
        return 0;
    case 1: //vertical left bottom
      return (iMaskTop>>1) & 0b111;
    case 2: ////vertical left top
      return (iMaskTop >> 4) & 0b111;
    case 3: //Hori top
      if (iMaskTop & 0b10000000)
        return 255;
      else
        return 0;
    case 4: // vert rigth top
      return (iMaskBottom >> 1) & 0b111;
    case 5: // hori middle
      if (iMaskTop & 0b1000)
        return 255;
      else
        return 0;
    case 6: // vert right bottom
      return (iMaskBottom >> 4) & 0b111;
    }
    return 255;
  }

  virtual uint8_t handleMode()
  {
    //DEBUGLOGF("moveUp[%d]\n",m_mode);
    if (m_transition == DisplayBase::TRANSITION_MOVE_UP && (m_iPos < 8))
    {
      //if  return DisplayComponent::handleMode(); 
      //DEBUGLOG("moveUp");
      if (!m_timer.isDone())
        return false;
      m_timer.startDelay(25);
      //moveUp();
      setValue(m_value);
      m_iPos++;
      //DEBUGLOGF("moveUp[%d][%d]\n", m_iPos,m_value);
    }
    return DisplayComponent::handleMode();
  }



public:
private:
  const uint8_t DisplayDigitMapping[13] = {
      0b01011111, //0
      0b01010000, //1
      0b00111011, //2
      0b01111001, //3
      0b01100100, //4
      0b01101101, //5
      0b01101111, //6
      0b01011000, //7
      0b01111111, //8
      0b01111101, //9
      0b00100000, //-
      0b00001000, //-
      0b00000001  //-
  };

  uint8_t m_iPos = 0;
};

#endif
