

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
#include "displayElement.h"
#include "doubleDigit.h"
#include "points.h"
#include "manageElements.h"

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
/*
#define HEURE 0
#define POINT_HR 1
#define MINUTE 2
#define POINT_MN 3
#define SECONDE 4
*/

class Afficheur : public ManageElements
{
public:
    enum AFFICHEUR_ELT
    {
        HOUR = 0,
        POINT_HR = 1,
        MINUTE = 2,
        /*POINT_MN=3, SECONDE=4,*/ LAST_AFFICHEUR_ELT = 3
    };
    Afficheur(CRGB *firstPixel) 
    {
        className = __func__;
        
        //m_elt = new DisplayElement[LAST_AFFICHEUR_ELT];
        m_nbElts = LAST_AFFICHEUR_ELT;
        CRGB *nextPixel = firstPixel;
        m_elt[HOUR] = new DoubleDigit(nextPixel);
        nextPixel = &nextPixel[m_elt[HOUR]->getNbPixels()];
        m_elt[POINT_HR] = new Points(nextPixel);
        nextPixel = &nextPixel[m_elt[POINT_HR]->getNbPixels()];
        m_elt[MINUTE] = new DoubleDigit(nextPixel);
        //Afficheur::init(m_elt,(int8_t)LAST_ELT);
        /*nextPixel += m_minute->getNbPixels() * sizeof(CRGB);
        m_point_minute = new Points(nextPixel);
        nextPixel += m_point_minute->getNbPixels() * sizeof(CRGB);
        m_seconde = new DoubleDigit(nextPixel);*/
    };

    /*void setValue(uint8_t iValue, AFFICHEUR_ELT elt)
    {
        m_elt[(uint8_t)elt]->setValue(iValue);
    };

    void setColorON(CRGB newcolor, AFFICHEUR_ELT elt)
    {
        if (elt == LAST_ELT)
        {
            for (uint8_t i = 0; i < LAST_ELT; i++)
            {
                m_elt[i]->setColorON(newcolor);
            }
        }
        else
        {
            m_elt[(uint8_t)elt]->setColorON(newcolor);
        }
    };

    void setColorOFF(CRGB newcolor, AFFICHEUR_ELT elt)
    {
        if (elt == LAST_ELT)
        {
            for (uint8_t i = 0; i < LAST_ELT; i++)
            {
                m_elt[i]->setColorOFF(newcolor);
            }
        }
        else
        {
            m_elt[(uint8_t)elt]->setColorOFF(newcolor);
        }
    };

    void HandleMode()
    {
        DEBUGLOGF("Mode [%d]\n", m_mode);
        for (uint8_t i = 0; i < LAST_ELT; i++)
        {
            DEBUGLOGF("Class Name [%s]\n", m_elt[i]->getClassName());
            m_elt[i]->HandleMode();
        }
    }

    void setMode(MODE_LED mode, AFFICHEUR_ELT elt)
    {
        if (elt == LAST_ELT)
        {
            for (uint8_t i = 0; i < LAST_ELT; i++)
            {
                m_elt[i]->setMode(mode);
            }
        }
        else
        {
            m_elt[(uint8_t)elt]->setMode(mode);
        }
    }

    void setState(boolean bON, AFFICHEUR_ELT elt)
    {
        if (elt == LAST_ELT)
        {
            for (uint8_t i = 0; i < LAST_ELT; i++)
            {
                m_elt[i]->setState(bON);
            }
        }
        else
        {
            m_elt[(uint8_t)elt]->setState(bON);
        }
    }
*/
private:
    //DisplayElement *m_elt[AFFICHEUR_ELT::LAST_ELT];
  //  DoubleDigit *m_hour;
  //  Points *m_point_hour;
  //  DoubleDigit *m_minute;
  //  Points *m_point_minute;
  //  DoubleDigit *m_seconde;
};

#endif
