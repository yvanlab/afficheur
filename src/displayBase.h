

#ifndef DisplayBase_h
#define DisplayBase_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include "FastLED.h"
#include "delayHelper.h"
#include <myTimer.h>

class DisplayBase
{
public:
  enum MODE_LED
  {
    STATIC = 0,
    CLiGONTANT = 1,
    PSYCHEDELIC = 2,
    K2000 = 3
  };
 
  DisplayBase()
  {
    m_mode = MODE_LED::STATIC;
    m_nbLeds = 1;
  }

  DisplayBase(CRGB *firstPixel)
  {
    m_mode = MODE_LED::STATIC;
    m_nbLeds = 1;
    m_firstPixel = firstPixel;
  }

  virtual void setMode(MODE_LED mode, int8_t iSelected = -1)
  {
    m_mode = mode;
  };

  virtual void setColorON(CRGB newcolor, int8_t iSelected = -1)
  {
    //DEBUGLOGF("setColorON - %s \n",className);
    m_colorON = newcolor;
  };

  virtual void setColorOFF(CRGB newcolor, int8_t iSelected = -1)
  {
    m_colorOFF = newcolor;
  };

  virtual void setState(boolean bON, int8_t iSelected = -1)
  {
    m_isOn = bON;
  };

  virtual void setValue(uint8_t value, int8_t iSelected = -1)
  {
    setState(value!=0);
  };

  void display(boolean bON)
  {
    if (m_isOn)  {
      if (bON )
        display(m_colorON);
      else
        display(m_colorOFF);
    } else
      display(m_colorSTOP);
      

  };

  void display(CRGB color)
  {
    for (uint8_t i = 0; i < getNbPixels(); i++)
    {
      m_firstPixel[i] = color;
    }
  };

  virtual uint8_t handleMode()
  {
    //DEBUGLOGF("Mode [%d]\n", m_mode);
    if (m_mode == MODE_LED::STATIC)
    {
      display(true);
    }
    else if (m_mode == MODE_LED::CLiGONTANT)
    {
      display(mtTimer.is250MSFrequence());
    }
    else if (m_mode == MODE_LED::PSYCHEDELIC)
    {
      if (m_isOn)
      {
        if (m_timer.isDone())
        {
          for (uint8_t i = 0; i < getNbPixels(); i++)
          {
            m_firstPixel[i] = CRGB(random(255), random(255), random(255));
          }
          m_timer.startDelay(100);
        }
      }
      else
        display(false);
    }
    else if (m_mode == MODE_LED::K2000)
    {
      if (m_isOn)
      {
        if (m_timer.isDone())
        {
          if (iK2000_incr == 1)
          {
            if (iK2000 >= getNbPixels() - 1)
            {
              iK2000_incr = -1;
            }
          }
          else
          {
            if (iK2000 <= 0)
            {
              iK2000_incr = +1;
            }
          }
          iK2000 += iK2000_incr;
          //DEBUGLOGF("iK2000 [%d], iK2000_incr[%d]\n ", iK2000, iK2000_incr);
          for (uint8_t i = 0; i < getNbPixels(); i++)
          {
            if (i == iK2000)
            {
              m_firstPixel[i] = m_colorON;
            }
            else
            {
              m_firstPixel[i] = m_colorOFF;
            }
            m_timer.startDelay(50);
          }
        }
        
      }else
          display(false);
    }
    return true;
  }

  static uint8_t getNbPixels() 
  {
    return 1;
  }

  const char* getClassName() {
    return className;
  } 

protected:
  CRGB *m_firstPixel;
  CRGB m_colorON = {255, 0, 0};
  CRGB m_colorOFF = {0, 0, 0};
  const CRGB m_colorSTOP = {0,0,0};
  uint8_t m_nbLeds;
  MODE_LED m_mode = MODE_LED::STATIC;
  boolean m_isOn = false;
  int8_t iK2000 = 0, iK2000_incr = 1;
  DelayHelper m_timer;

  const char* className;
};

#endif
