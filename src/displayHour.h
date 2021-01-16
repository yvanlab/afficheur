

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
#define NUM_LEDS 21+21+2+21+21+2+21+21
#define DATA_PIN 2
//#define CLK_PIN   4
#define LED_TYPE SK6812
#define COLOR_ORDER RGB

#define BRIGHTNESS 255


class DisplayHour : public DisplayComponent
{
public:
    enum HOUR_ELT
    {
        HOUR = 0,
        POINT_HR = 1,
        MINUTE = 2,
        POINT_MN = 3,
        SECONDE = 4,
        LAST_HOUR_ELT = 3
    };

    DisplayHour() : DisplayComponent(m_leds)
    {
        className = __func__;
        m_nbLeds = DisplayHour::getNbPixels();

        FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(m_leds, NUM_LEDS)
            .setCorrection(TypicalLEDStrip)
            .setDither(BRIGHTNESS < 255);

        // set master brightness control
        FastLED.setBrightness(BRIGHTNESS);

        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            m_leds[i].red = i;
        }
        FastLED.show();

        add(m_leds);
    };

    CRGB *getLeds()
    {
        return m_leds;
    }

    static uint8_t getNbPixels()
    {
        return DisplayDoubleDigit::getNbPixels() * 2 + DisplayPoints::getNbPixels();
    }

    virtual void add(CRGB *firstPixel)
    {
        DEBUGLOGF("add(CRGB *firstPixel) : %s \n", className);
        CRGB *nextPixel = firstPixel;
        DEBUGLOG("DisplayDoubleDigit");
        m_listComponent.push_back(new DisplayDoubleDigit(nextPixel));

        DEBUGLOG("DisplayPoints");
        nextPixel = &nextPixel[DisplayDoubleDigit::getNbPixels()];
        m_listComponent.push_back(new DisplayPoints(nextPixel));

        DEBUGLOG("DisplayDoubleDigit");
        nextPixel = &nextPixel[DisplayPoints::getNbPixels()];
        m_listComponent.push_back(new DisplayDoubleDigit(nextPixel));

        DEBUGLOG("DisplayPoints");
        nextPixel = &nextPixel[DisplayDoubleDigit::getNbPixels()];
        m_listComponent.push_back(new DisplayPoints(nextPixel));

        DEBUGLOG("DisplayDoubleDigit");
        nextPixel = &nextPixel[DisplayPoints::getNbPixels()];
        m_listComponent.push_back(new DisplayDoubleDigit(nextPixel));
    }

    void nsetValue(uint8_t iValue, HOUR_ELT elt)
    {
        m_listComponent[(uint8_t)elt]->setValue(iValue);
    };

    void setColorON(CRGB iValue, HOUR_ELT elt)
    {
        m_listComponent[(uint8_t)elt]->setColorON(iValue);
    };

public:
    CRGB m_leds[NUM_LEDS];
};

#endif