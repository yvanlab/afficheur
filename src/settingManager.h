
#ifndef SettingManager_h
#define SettingManager_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif

#include "ArduinoJson.h"

#include <EEPROM.h>
#include <baseManager.h>
#include <BaseSettingManager.h>
//#include <pixeltypes.h>
#include "displayBase.h"



class SettingManager : public BaseSettingManager
{

public:
	enum MODE_HORLOGE
	{
		MODE_CLOCK = 0,
		MODE_ALARM = 1,
		MODE_COUNTDOWN = 2,
		MODE_COUNTDOWN_END = 3,
		MODE_REBOOT = 4,
		MODE_TEST = 5
	};
	typedef  uint8_t SOUND_TYPE;
	const uint8_t NO_SOUND = 0;
/*	enum SOUND_TYPE
    {
        NO_SOUND = 0,
        SOUND_BIP = 1,
        SOUND_END = 2,
        SOUND_ALARM = 3,
        SOUND_CAMEL = 4,
        SOUND_LAST = 5
    };
*/

	SettingManager(unsigned char pinLed);
	virtual uint8_t readData();
	virtual uint8_t writeData();

	void writePage();
	void readPage();

	String getClassName() { return "SettingManager"; }

	String toString(boolean bJson);
	String toStringCfg(boolean bJson);

	uint32_t m_rebootColorOn = 16777215;
	uint32_t m_rebootColorOff = 0;
	DisplayBase::MODE_LED m_rebootAnimation = DisplayBase::K2000;
	DisplayBase::MODE_TRANSITION m_rebootTransition = DisplayBase::TRANSITION_NONE;
	
	
	uint32_t m_clockColorOn = 255;
	uint32_t m_clockColorOff = 0;
	DisplayBase::MODE_LED m_clockAnimation = DisplayBase::STATIC;
	DisplayBase::MODE_TRANSITION m_clockTransition = DisplayBase::TRANSITION_NONE;
	uint8_t m_clockIntensityNight = 10;
	uint8_t m_clockIntensityDay = 100;
	uint8_t m_clockHourDay = 06;
	uint8_t m_clockHourNight = 20;
	SOUND_TYPE m_clockSoundHour = NO_SOUND;

	uint32_t m_countdownColorOn = 16711680;
	uint32_t m_countdownColorOff = 20;
	DisplayBase::MODE_LED m_countdownAnimation = DisplayBase::STATIC;
	DisplayBase::MODE_LED m_countdownAnimationEnd = DisplayBase::PSYCHEDELIC;
	DisplayBase::MODE_TRANSITION m_countdownTransition = DisplayBase::TRANSITION_NONE;
	SOUND_TYPE m_countdownSoundStart = NO_SOUND;
	SOUND_TYPE m_countdownSoundEnd = NO_SOUND;
	uint8_t m_countdownDurationEnd = 5;

	String m_alarmTrigger = "15:00:00";
	DisplayBase::MODE_LED m_alarmTriggerAnimation = DisplayBase::PSYCHEDELIC;
	DisplayBase::MODE_TRANSITION m_alarmTriggerTransition = DisplayBase::TRANSITION_NONE;
	SOUND_TYPE m_alarmTriggerSound = NO_SOUND;
	uint8_t m_alarmTriggerDuration = 20;

	uint32_t m_countdownCpt = 0;
	SOUND_TYPE  m_soundToPlay = NO_SOUND;
	MODE_HORLOGE m_mode;
};

#endif
