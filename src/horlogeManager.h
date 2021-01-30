

#ifndef HorlogeManager_h
#define HorlogeManager_h

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
#include "main.h"
#include "displayHour.h"

class HorlogeManager
{
public:
    HorlogeManager(SettingManager *smManager)
    {
        m_smManager = smManager;
        m_affManager = new DisplayHour();
    }

    DisplayHour *getDisplayHour()
    {
        return m_affManager;
    }

    void displayAfterReboot()
    {
        m_affManager->setMode(m_smManager->m_modeAfterReboot);
        displayHour();
    }

    void displayHour()
    {
        uint64_t heure = now();
        //m_affManager->setMode(mLed);
        m_affManager->setValue(hour(heure), DisplayHour::HOUR);
        m_affManager->setValue(minute(heure), DisplayHour::MINUTE);
        m_affManager->setValue(second(heure), DisplayHour::SECONDE);
        uint8_t iPoint = 255;
        if (mtTimer.is1SFrequence())
            iPoint = 0;
        m_affManager->setValue(iPoint, DisplayHour::POINT_HR);
        m_affManager->setValue(iPoint, DisplayHour::POINT_MN);

    }

    uint8_t displayCompteArebours()
    {
        
        if (millis()-m_previousTimeStamp < 1000) return true;
        m_previousTimeStamp = millis();
        if (m_smManager->m_compteARebour > 0)
        {
            m_smManager->m_compteARebour--;
        }
        uint8_t minute = m_smManager->m_compteARebour / 60;
        uint8_t seconde = m_smManager->m_compteARebour % 60;

        m_affManager->setState(DisplayDoubleDigit::DASH_BOTH, DisplayHour::HOUR);
        m_affManager->setValue(minute, DisplayHour::MINUTE);
        m_affManager->setValue(seconde, DisplayHour::SECONDE);
        return m_smManager->m_compteARebour!=0;
    }


    void displayCompteAreboursFin()
    {
        m_affManager->setMode(m_smManager->m_modeFinCompteaRebours);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::HOUR);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::MINUTE);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::SECONDE);
        m_affManager->setValue(255, DisplayHour::POINT_HR);
        m_affManager->setValue(255, DisplayHour::POINT_MN);
    }

    void setConfig() {
        m_affManager->setMode(m_smManager->m_modeLed);
        m_affManager->setColorON(CRGB(m_smManager->m_mainColor));
        //m_affManager->setColorOFF(mLed);
    }
    void handle()
    {
        /*if ) {
            
        }*/
        m_affManager->handleMode();
        FastLED.show();

    }

protected:
    DisplayHour     *m_affManager;
    SettingManager  *m_smManager;

    uint32_t        m_previousTimeStamp;
};

#endif
