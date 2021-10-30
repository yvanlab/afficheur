

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
        m_affManager->setMode(m_smManager->m_rebootAnimation);
        m_affManager->setTransition(m_smManager->m_rebootTransition);
        m_affManager->setColorON(CRGB(m_smManager->m_rebootColorOn));
        m_affManager->setColorOFF(CRGB(m_smManager->m_rebootColorOff));
        displayHour();
    }

    void displayHour()
    {
        if (!myDelay.isDone()) return;
        myDelay.startDelay(1000);
        //m_smManager->m_soundToPlay = 1;
        m_affManager->setMode(m_smManager->m_clockAnimation);
        m_affManager->setTransition(m_smManager->m_clockTransition);
        
        m_affManager->setColorON(CRGB(m_smManager->m_clockColorOn));
        m_affManager->setColorOFF(CRGB(m_smManager->m_clockColorOff));

        uint64_t heure = now();
        //m_affManager->setMode(mLed);
        if (hour(heure)<10) {
            m_affManager->setValue(hour(heure)+DisplayDoubleDigit::UNITY_ONLY, DisplayHour::HOUR);
        } else {
             m_affManager->setValue(hour(heure), DisplayHour::HOUR);
        }  
        m_affManager->setValue(minute(heure), DisplayHour::MINUTE);
        m_affManager->setValue(second(heure), DisplayHour::SECONDE);
        uint8_t iPoint = 255;
        if (mtTimer.is250MSPeriod())
            iPoint = 0;
        m_affManager->setValue(iPoint, DisplayHour::POINT_HR);
        m_affManager->setValue(iPoint, DisplayHour::POINT_MN);

    }

    uint8_t displayCountdown()
    {
        //if (millis()-m_previousTimeStamp < 1000) return true;
        if (!myDelay.isDone()) return true;
        myDelay.startDelay(1000);

        m_affManager->setMode(m_smManager->m_countdownAnimation);
        m_affManager->setColorON(CRGB(m_smManager->m_countdownColorOn));
        m_affManager->setColorOFF(CRGB(m_smManager->m_countdownColorOff));
        m_previousTimeStamp = millis();
        if (m_smManager->m_countdownCpt > 0)
        {
            m_smManager->m_countdownCpt--;
        }
        tmElements_t tm;
        breakTime(m_smManager->m_countdownCpt, tm);
        //DEBUGLOGF("h[%d]m[%d]s[%d]\n",tm.Hour,tm.Minute,tm.Second)
        if (tm.Hour>0) {
            m_affManager->setValue(tm.Hour, DisplayHour::HOUR);
        } else {
            m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::HOUR);
        }
        if (tm.Minute>0) {
           m_affManager->setValue(tm.Minute, DisplayHour::MINUTE);
        } else {
            m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::MINUTE);
        }      
        m_affManager->setValue(tm.Second, DisplayHour::SECONDE);
        if (m_smManager->m_countdownCpt==0) {
            myDelay.startDelay(m_smManager->m_countdownDurationEnd*1000);
            return false;
        }
        return true;
    }


    uint8_t displayCountdownEnd()
    {
        m_affManager->setMode(m_smManager->m_countdownAnimationEnd);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::HOUR);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::MINUTE);
        m_affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::SECONDE);
        m_affManager->setValue(255, DisplayHour::POINT_HR);
        m_affManager->setValue(255, DisplayHour::POINT_MN);
        return !myDelay.isDone();
    }

    void setConfig() {
        /*m_affManager->setMode(m_smManager->m_modeLed);
        m_affManager->setColorON(CRGB(m_smManager->m_mainColor));*/
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
    DelayHelper     myDelay;
};

#endif
