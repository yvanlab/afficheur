

#ifndef ManageElements_h
#define ManageElements_h

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

class ManageElements
{
public:
    ManageElements(){};

    /*void init(DisplayElement **elts, uint8_t nbElts)
    {
        m_elt = elts;
        m_nbElts = nbElts;
    };*/

    void setValue(uint8_t iValue, int8_t iElt = -1)
    {
        if (iElt != -1)
        {
            m_elt[(uint8_t)iElt]->setValue(iValue);
        }
        else
        {
            for (uint8_t i = 0; i < m_nbElts; i++)
            {
                m_elt[i]->setValue(iValue);
            }
        }
    };

    void setColorON(CRGB newcolor, int8_t iElt = -1)
    {
        if (iElt == -1)
        {
            for (uint8_t i = 0; i < m_nbElts; i++)
            {
                m_elt[i]->setColorON(newcolor);
            }
        }
        else
        {
            m_elt[(uint8_t)iElt]->setColorON(newcolor);
        }
    };

    void setColorOFF(CRGB newcolor, int8_t iElt = -1)
    {
        if (iElt == -1)
        {
            for (uint8_t i = 0; i < m_nbElts; i++)
            {
                m_elt[i]->setColorOFF(newcolor);
            }
        }
        else
        {
            m_elt[(uint8_t)iElt]->setColorOFF(newcolor);
        }
    };

    void HandleMode()
    {
        //DEBUGLOGF("Mode [%d]\n", m_mode);
        for (uint8_t i = 0; i < m_nbElts; i++)
        {
            DEBUGLOGF("Class Name [%s]\n", m_elt[i]->getClassName());
            m_elt[i]->HandleMode();
        }
    }

    void setMode(DisplayElement::MODE_LED mode, int8_t iElt = -1)
    {
        if (iElt == -1)
        {
            for (uint8_t i = 0; i < m_nbElts; i++)
            {
                m_elt[i]->setMode(mode);
            }
        }
        else
        {
            m_elt[(uint8_t)iElt]->setMode(mode);
        }
    }

    void setState(boolean bON, int8_t iElt = -1)
    {
        if (iElt == -1)
        {
            for (uint8_t i = 0; i < m_nbElts; i++)
            {
                m_elt[i]->setState(bON);
            }
        }
        else
        {
            m_elt[(uint8_t)iElt]->setState(bON);
        }
    }

    const char *getClassName()
    {
        return className;
    }

protected:
    DisplayElement *m_elt[10];
    uint8_t m_nbElts = 0;

    const char *className;
};

#endif
