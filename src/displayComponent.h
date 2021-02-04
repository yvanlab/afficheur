

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

  virtual void add(CRGB *firstPixel) = 0;
  /* {
     DEBUGLOGF("add(CRGB *firstPixel) : %s NE DOIT PAS ETRE APPELE]\n", className);
  }*/

  virtual void setColorON(CRGB newcolor, int8_t iSelected = -1)
  {
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

    if (iSelected < 0 || iSelected >= m_listComponent.size())
    {
      for (; itb != ite; itb++)
      {
        (*itb)->setColorON(newcolor);
      }
    }
    else
    {
      m_listComponent[iSelected]->setColorON(newcolor);
    }
    m_colorON = newcolor;
  };

  virtual void setColorOFF(CRGB newcolor, int8_t iSelected = -1)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    if (iSelected < 0 || iSelected >= m_listComponent.size())
    {
      std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
      const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

      for (; itb != ite; itb++)
      {
        (*itb)->setColorOFF(newcolor);
      }
    }
    else
    {
      m_listComponent[iSelected]->setColorOFF(newcolor);
    }
    m_colorOFF = newcolor;
  };

  virtual void setValue(uint8_t value, int8_t iSelected = -1)
  {
    //DEBUGLOGF("setValue[%d]\n", value);
    if (iSelected < 0 || iSelected >= m_listComponent.size())
    {

      std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
      const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

      for (; itb != ite; itb++)
      {
        (*itb)->setValue(value);
      }
    }
    else
    {
      m_listComponent[iSelected]->setValue(value);
    }
    m_isOn = value!=0;
    m_value = value;
  };

  virtual void setState(boolean bON, int8_t iSelected = -1)
  {
    if (iSelected < 0 || iSelected >= m_listComponent.size())
    {

      std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
      const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

      for (; itb != ite; itb++)
      {
        (*itb)->setState(bON);
      }
    }
    else
    {
      m_listComponent[iSelected]->setState(bON);
    }
    m_isOn = bON;
  };

  virtual uint8_t handleMode()
  {
    std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
    const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();
    boolean bRes=false;
    for (; itb != ite; itb++)
    {
      bRes |= (*itb)->handleMode();
    }
    return bRes;
  };

  void setMode(MODE_LED mode, int8_t iSelected = -1)
  {
    if (iSelected < 0 || iSelected >= m_listComponent.size())
    {

      std::vector<DisplayBase *>::const_iterator itb = m_listComponent.begin();
      const std::vector<DisplayBase *>::const_iterator ite = m_listComponent.end();

      for (; itb != ite; itb++)
      {
        (*itb)->setMode(mode);
      }
    }
    else
    {
      m_listComponent[iSelected]->setMode(mode);
    }
    m_mode = mode;
  };

protected:
  std::vector<DisplayBase *> m_listComponent;
};

#endif
