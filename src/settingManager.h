
#ifndef SettingManager_h
#define SettingManager_h

#if defined(ARDUINO) && ARDUINO >= 100

#include Arduino.h

#else

#include WProgram.h

#endif

#include ArduinoJson.h

#include <EEPROM.h>
#include <baseManager.h>
#include <BaseSettingManager.h>
//#include <pixeltypes.h>
#include displayBase.h

class SettingManager : public BaseSettingManager
{

public:
	enum MODE_HORLOGE
	{
		MODE_HEURE = 0,
		MODE_ALARME = 1,
		MODE_COUNTDOWNS = 2,
		MODE_COUNTDOWN_END = 3,
		MODE_REBOOT = 4,
		MODE_TEST = 5
	};
	SettingManager(unsigned char pinLed);
	virtual uint8_t readData();
	virtual uint8_t writeData();

	void writePage();
	void readPage();

	String getClassName() { return SettingManager; }

	//void sortPages();

	String toString(boolean bJson);
	String toStringCfg(boolean bJson);

	/*uint32_t m_mainColor = 0xFF0000;
	String m_mainAnnimation;

	uint16_t m_dayIntensity;
	uint16_t m_nightIntensity;
	uint8_t m_nightHour;
	uint8_t m_dayHour;
	tmElements_t m_alarm;

	DisplayBase::MODE_LED m_modeLed = DisplayBase::STATIC;
	DisplayBase::MODE_LED m_modeFinCOUNTDOWNs = DisplayBase::PSYCHEDELIC;
	DisplayBase::MODE_LED m_modeAfterReboot = DisplayBase::CLiGONTANT;*/

	uint32_t m_rebootColorOn = 16777215;
	uint32_t m_rebootColorOff = 0;
	DisplayBase::MODE_LED m_rebootAnimation = DisplayBase::K2000;
	
	
	uint32_t m_clockColorOn = 255;
	uint32_t m_clockColorOff = 0;
	DisplayBase::MODE_LED m_clockAnimation = DisplayBase::STATIC;
	uint8_t m_clockIntensityNight = 10;
	uint8_t m_clockIntensityDay = 100;
	uint8_t m_clockHourDay = 06;
	uint8_t m_clockHourNight = 20;
	uint8_t m_clockSoundHour = 0;

	uint32_t m_countdownColorOn = 16711680;
	uint32_t m_countdownColorOff = 20;
	DisplayBase::MODE_LED m_countdownAnimation = DisplayBase::STATIC;
	DisplayBase::MODE_LED m_countdownAnimationEnd = DisplayBase::PSYCHEDELIC;
	uint8_t m_countdownSoundStart = 0;
	uint8_t m_countdownSoundEnd = 0;
	uint8_t m_countdownDurationEnd = 5;

	String m_alarmTrigger = "15:00:00";
	DisplayBase::MODE_LED m_alarmTriggerAnimation = DisplayBase::PSYCHEDELIC;
	uint8_t m_alarmTriggerSound = 0;
	uint8_t m_alarmTriggerDuration : 20;

	uint32_t m_COUNTDOWN = 0;
	MODE_HORLOGE m_mode;
};

#endif
