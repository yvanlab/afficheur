

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

 static uint16_t sPseudotime;
  static  uint16_t sLastMillis;
    static uint16_t sHue16;

class DisplayBase
{
public:
  enum MODE_LED
  {
    STATIC = 0,
    CLiGONTANT = 1,
    PSYCHEDELIC = 2,
    K2000 = 3,
    PSYCHEDELIC_2=4,
    MOVE_UP_DIGIT=5,
    MODE_LED_LAST =6,  
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
    m_value = value;
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
            m_timer.startDelay(250);
          }
        }
        
      }else
          display(false);
    } else if (m_mode == PSYCHEDELIC_2) {
      if (m_isOn) pride();
      else display(false);


    }
    return true;
  }


  void pride()
    {

       
        
        uint8_t sat8 = beatsin88(87, 220, 250);
        uint8_t brightdepth = beatsin88(341, 96, 224);
        uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
        uint8_t msmultiplier = beatsin88(147, 23, 60);

        uint16_t hue16 = sHue16; //gHue * 256;
        uint16_t hueinc16 = beatsin88(113, 1, 3000);

        uint16_t ms = millis();
        uint16_t deltams = ms - sLastMillis;
        sLastMillis = ms;
        sPseudotime += deltams * msmultiplier;
        sHue16 += deltams * beatsin88(400, 5, 9);
        uint16_t brightnesstheta16 = sPseudotime;

        for (uint16_t i = 0; i <getNbPixels(); i++)
        {
            
            hue16 += hueinc16;
            uint8_t hue8 = hue16 / 256;

            brightnesstheta16 += brightnessthetainc16;
            uint16_t b16 = sin16(brightnesstheta16) + 32768;

            uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
            uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
            bri8 += (255 - brightdepth);

            CRGB newcolor = CHSV(hue8, sat8, bri8);

            /*uint16_t pixelnumber = i;
            pixelnumber = (NUM_LEDS - 1) - pixelnumber;*/
            nblend(m_firstPixel[i], newcolor, 64);
            
        }
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
  uint8_t m_value = 0;
  int8_t iK2000 = 0, iK2000_incr = 1;
  DelayHelper m_timer;

  const char* className;


  
};

#endif
