

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
    DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
    CRGB *nextPixel = firstPixel;
    for (uint8_t i = 0; i < 7; i++)
    {
      m_listComponent.push_back(new DisplaySegment(nextPixel));
      nextPixel = &nextPixel[DisplaySegment::getNbPixels()];
    }
  }

  virtual void setValue(uint8_t value, int8_t iSelected = -1)
  {
    /*if (value>9){
      DEBUGLOGF("setValue[%d][%d]\n", value,DisplayDigitMapping[value]);
    }*/

    uint8_t mask = 0x00000001;
    /*std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();
    for (; itb != ite; itb++)
    {
      if ((DisplayDigitMapping[value] & mask) != 0)
        (*itb)->setValue(255, iSelected);
      else
        (*itb)->setValue(0, iSelected);*/
    for (uint8_t i = 0; i < 7; i++)
    {
      if ((DisplayDigitMapping[value] & mask) != 0)
        m_listComponent[i]->setValue(getValueForAnnimation(i), iSelected);
      else
        m_listComponent[i]->setValue(0, iSelected);
      mask = mask << 1;
    }

    //setState(value!=0);
    m_value = value;
  }

  uint8_t getValueForAnnimation(uint8_t index)
  {

    uint8_t iMaskTop = 0b11111111 >> (7 - m_iPos);    //0000 0001
    uint8_t iMaskBottom = 0b11111111 << (7 - m_iPos); //1000 0000
                                                      //DEBUGLOGF("handleMode [%d][%d]",iMask,iMask&0b111);
    switch (index)
    {
    case 0: //horizontale bottom
      if (iMaskTop & 0b0001)
        return 255;
      else
        return 0;
    case 1: //vertical left bottom
      return iMaskTop & 0b111;
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
      return (iMaskBottom >> 5) & 0b111;
    }
  }

  virtual uint8_t handleMode()
  {
    if (!m_timer.isDone())
      return false;
    m_timer.startDelay(1000);
    m_iPos++;
    DEBUGLOGF("moveUp[%d]\n", m_iPos);
    setValue(m_value);
    if (m_iPos == 8)
      m_iPos = 0;
    return true;

    //DEBUGLOGF("moveUp[%d]\n",m_mode);
    /* if (m_mode == DisplayBase::MOVE_UP_DIGIT)
    {
      //DEBUGLOG("moveUp");
      return moveUp();
    }
    else
    {
      return DisplayComponent::handleMode();
    }*/
  }

  uint8_t moveUp()
  {

    uint8_t iMaskTop = 0b000001;
    uint8_t iMaskBottom;

    if (!m_timer.isDone())
      return false;
    m_timer.startDelay(1000);
    DEBUGLOGF("moveUp[%d]\n", m_iPos);

    iMaskTop = 0b11111111 >> (7 - m_iPos);    //0000 0001
    iMaskBottom = 0b11111111 << (7 - m_iPos); //1000 0000
                                              //DEBUGLOGF("handleMode [%d][%d]",iMask,iMask&0b111);
    //First segments up (1,6)
    // 001  100
    // 011  11
    // 111  111

    //Vetricale
    m_listComponent[1]->setValue(iMaskTop & 0b111);
    m_listComponent[1]->handleMode();

    m_listComponent[6]->setValue((iMaskBottom >> 5) & 0b111);
    m_listComponent[6]->handleMode();

    m_listComponent[2]->setValue((iMaskTop >> 4) & 0b111);
    m_listComponent[2]->handleMode();

    m_listComponent[4]->setValue((iMaskBottom >> 1) & 0b111);
    m_listComponent[4]->handleMode();

    //horizontale
    if (iMaskTop & 0b0001)
    {
      m_listComponent[0]->setValue(255);
    }
    else
    {
      m_listComponent[0]->setValue(0);
    }
    m_listComponent[0]->handleMode();

    if (iMaskTop & 0b1000)
    {
      m_listComponent[5]->setValue(255);
    }
    else
    {
      m_listComponent[5]->setValue(0);
    }
    m_listComponent[5]->handleMode();

    if (iMaskTop & 0b10000000)
    {
      m_listComponent[3]->setValue(255);
    }
    else
    {
      m_listComponent[3]->setValue(0);
    }
    m_listComponent[3]->handleMode();

    m_iPos++;

    if (m_iPos == 8)
      m_iPos = 0;
    return true;
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
