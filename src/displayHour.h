

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
#define NUM_LEDS 21 + 21 + 2 + 21 + 21 + 2 + 21 + 21
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
        return &m_leds[0];
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

    /* virtual uint8_t handleMode() {
         pride();
         return true;
     }*/
    
    /*void pride()
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

        for (uint16_t i = 0; i < NUM_LEDS; i++)
        {
            
            hue16 += hueinc16;
            uint8_t hue8 = hue16 / 256;

            brightnesstheta16 += brightnessthetainc16;
            uint16_t b16 = sin16(brightnesstheta16) + 32768;

            uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
            uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
            bri8 += (255 - brightdepth);

            CRGB newcolor = CHSV(hue8, sat8, bri8);

            uint16_t pixelnumber = i;
            pixelnumber = (NUM_LEDS - 1) - pixelnumber;
            nblend(m_leds[pixelnumber], newcolor, 64);
            
        }
    }*/

public:
    CRGB m_leds[NUM_LEDS];

    
};

#endif