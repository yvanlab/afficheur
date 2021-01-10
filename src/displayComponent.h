

#ifndef DisplayComponent_h
#define DisplayComponent_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "displayBase.h"
#include <iterator>
#include "FastLED.h"
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

class DisplayComponent : public DisplayBase
{
public:
  //
  DisplayComponent(CRGB *firstPixel) : DisplayBase()
  {
    className = __func__;
  };

  virtual void add(CRGB *firstPixel) =0;
  /* {
     DEBUGLOGF("add(CRGB *firstPixel) : %s NE DOIT PAS ETRE APPELE]\n", className);
  }*/



  virtual void setColorON(CRGB newcolor)
  {
    //DEBUGLOGF("setColorON[%s]\n", className);
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->setColorON(newcolor);
    }
  };

  virtual void setColorOFF(CRGB newcolor)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->setColorOFF(newcolor);
    }
  };

  virtual void setValue(uint8_t value)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->setValue(value);
    }
  };

  virtual void setState(boolean bON)
  {
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->setState(bON);
    }
  };

  virtual void handleMode()
  {
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->handleMode();
    }
  };

  void setMode(MODE_LED mode)
  {
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    for (; itb != ite; itb++)
    {
      (*itb)->setMode(mode);
    }
  };

protected:
  std::vector<DisplayBase *> m_listComponent;
};

#endif
