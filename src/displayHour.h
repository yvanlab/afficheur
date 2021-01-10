

#ifndef DisplayHour_h
#define DisplayHour_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif
#include <time.h>
#include "FastLED.h"
//#include "displayElement.h"
#include "displayDoubleDigit.h"
#include "displayPoints.h"

/*
            HEURE         POINT_HR       MINUTE      POINT_MN       SECONDE

      *****         *****           *****       *****           *****       *****
     *     *       *     *         *     *     *     *         *      *    *     *
     *     *       *     *    *    *     *     *     *   *     *      *    *     *
       ****         *****           *****       *****           ******      *****
     *     *       *     *    *    *     *     *     *    *    *      *    *     *
     *     *       *     *         *     *     *     *         *      *    *     *
      *****         *****           *****       *****           ******      *****

*/

class DisplayHour : public DisplayComponent
{
public:
    enum HOUR_ELT
    {
        HOUR = 0,
        POINT_HR = 1,
        MINUTE = 2,
        /*POINT_MN=3, SECONDE=4,*/ LAST_HOUR_ELT = 3
    };

    DisplayHour(CRGB *firstPixel) : DisplayComponent(firstPixel)
    {
        className = __func__;
        m_nbLeds = DisplayHour::getNbPixels();
        add(firstPixel);
        //DisplayHour::init(m_elt,(int8_t)LAST_ELT);
        /*nextPixel += m_minute->getNbPixels() * sizeof(CRGB);
        m_point_minute = new Points(nextPixel);
        nextPixel += m_point_minute->getNbPixels() * sizeof(CRGB);
        m_seconde = new DoubleDigit(nextPixel);*/
    };

    static uint8_t getNbPixels() 
    {
        return DisplayDoubleDigit::getNbPixels()*2+DisplayPoints::getNbPixels();
    }


    virtual void add(CRGB *firstPixel)
    {
        DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
        CRGB *nextPixel = firstPixel;
        DEBUGLOG("DisplayDoubleDigit");
        m_listComponent.push_back(new DisplayDoubleDigit(nextPixel));
        nextPixel = &nextPixel[DisplayDoubleDigit::getNbPixels()];
        DEBUGLOG("DisplayPoints");
        m_listComponent.push_back(new DisplayPoints(nextPixel));
        nextPixel = &nextPixel[DisplayPoints::getNbPixels()];
        DEBUGLOG("DisplayDoubleDigit");
        m_listComponent.push_back(new DisplayDoubleDigit(nextPixel));
    }

    void setValue(uint8_t iValue, HOUR_ELT elt)
    {
        m_listComponent[(uint8_t)elt]->setValue(iValue);
    };
};

#endif
