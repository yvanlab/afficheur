

#ifndef afficheur_h
#define afficheur_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif
#include <time.h>
#include "FastLED.h"
#include "doubleDigit.h"
#include "points.h"

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

#define HEURE 0
#define POINT_HR 1
#define MINUTE 2
#define POINT_MN 3
#define SECONDE 4

class Afficheur
{
public:
    Afficheur(CRGB *firstPixel)
    {
        CRGB *nextPixel = firstPixel;
        m_hour = new DoubleDigit(nextPixel);
        nextPixel += DoubleDigit::getNbPixels() * sizeof(CRGB);
        m_point_hour = new Points(nextPixel);
        nextPixel += Points::getNbPixels() * sizeof(CRGB);
        m_minute = new DoubleDigit(nextPixel);
        nextPixel += DoubleDigit::getNbPixels() * sizeof(CRGB);
        m_point_minute = new Points(nextPixel);
        nextPixel += Points::getNbPixels() * sizeof(CRGB);
        m_seconde = new DoubleDigit(nextPixel);
    };

    void displayValue(time_t timetoDisplay){
        setColor(CRGB(255,255,255));
        m_hour->displayValue(hour (timetoDisplay));
        m_minute->displayValue(minute (timetoDisplay));
        uint8_t ss = second (timetoDisplay);
        m_seconde->displayValue(second (ss));
        if (ss % 2) {
            m_point_hour ->setColor(CRGB(0,0,0));
            m_point_minute ->setColor(CRGB(0,0,0));
        } else {
            m_point_hour ->setColor(CRGB(0,255,0));
            m_point_minute ->setColor(CRGB(0,255,0));
        }
    };

    void setColor(CRGB newcolor){
        m_hour->setColor(newcolor);
        m_point_hour->setColor(newcolor);
        m_minute->setColor(newcolor);
        m_point_minute->setColor(newcolor);
        m_seconde->setColor(newcolor);
    };

private:
    DoubleDigit *m_hour;
    Points *m_point_hour;
    DoubleDigit *m_minute;
    Points *m_point_minute;
    DoubleDigit *m_seconde;
};

#endif
