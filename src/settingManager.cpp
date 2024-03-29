

#include "settingManager.h"
#include "main.h"
#include "SPIFFS.h"
#include <SD.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ESP32
extern portMUX_TYPE wtimerMux; // = portMUX_INITIALIZER_UNLOCKED;
#endif

SettingManager::SettingManager(unsigned char pinLed) : BaseSettingManager(pinLed)
{
}

String SettingManager::toStringCfg(boolean bJson)
{
	String ss;
	if (bJson == STD_TEXT)
		return BaseSettingManager::toString(bJson);

	ss = "\"mode\":\"" + String(m_mode) + "\",";
	
	ss += "\"rebootColorOn\":\"" + String(m_rebootColorOn) + "\",";
	ss += "\"rebootColorOff\":\"" + String(m_rebootColorOff) + "\",";
	ss += "\"rebootAnimation\":\"" + String(m_rebootAnimation) + "\",";
		ss += "\"rebootTransition\":\"" + String(m_rebootTransition) + "\",";

	ss += "\"clockColorOn\":\"" + String(m_clockColorOn) + "\",";
	ss += "\"clockColorOff\":\"" + String(m_clockColorOff) + "\",";
	ss += "\"clockAnimation\":\"" + String(m_clockAnimation) + "\",";
	ss += "\"clockTransition\":\"" + String(m_clockTransition) + "\",";
	ss += "\"clockIntensityNight\":\"" + String(m_clockIntensityNight) + "\",";
	ss += "\"clockIntensityDay\":\"" + String(m_clockIntensityDay) + "\",";
	ss += "\"clockHourDay\":\"" + String(m_clockHourDay) + "\",";
	ss += "\"clockHourNight\":\"" + String(m_clockHourNight) + "\",";
	ss += "\"clockSoundHour\":\"" + String(m_clockSoundHour) + "\",";
	
	ss += "\"countdownCpt\":\"" + String(m_countdownCpt) + "\",";
	ss += "\"countdownColorOn\":\"" + String(m_countdownColorOn) + "\",";
	ss += "\"countdownColorOff\":\"" + String(m_countdownColorOff) + "\",";
	ss += "\"countdownAnimation\":\"" + String(m_countdownAnimation) + "\",";
ss += "\"countdownTransition\":\"" + String(m_countdownTransition) + "\",";
	ss += "\"countdownAnimationEnd\":\"" + String(m_countdownAnimationEnd) + "\",";
	ss += "\"countdownSoundStart\":\"" + String(m_countdownSoundStart) + "\",";
	ss += "\"countdownSoundEnd\":\"" + String(m_countdownSoundEnd) + "\",";
	ss += "\"countdownDurationEnd\":\"" + String(m_countdownDurationEnd) + "\",";

	ss += "\"alarmTrigger\":\"" + String(m_alarmTrigger) + "\",";
	ss += "\"alarmTriggerAnimation\":\"" + String(m_alarmTriggerAnimation) + "\",";
ss += "\"alarmTriggerTransition\":\"" + String(m_alarmTriggerTransition) + "\",";
	ss += "\"alarmTriggerSound\":\"" + String(m_alarmTriggerSound) + "\",";
	ss += "\"alarmTriggerDuration\":\"" + String(m_alarmTriggerDuration) + "\"";

	/*ss = "\"mainColor\":\"" + String(m_mainColor) +" \",";
	ss = "\"dayIntensity\":\"" + String(m_dayIntensity) +" \",";
	ss = "\"nightIntensity\":\"" + String(m_nightIntensity) +" \",";
	ss = "\"dayHour\":\"" + String(m_dayHour) +" \",";
	ss = "\"nightHour\":\"" + String(m_nightHour) +" \",";
	ss += "\"mainAnnimation\":\"" +m_mainAnnimation + "\",";
	// dd-mm-yyyy hh:mm:ss
	ss += "\"alarm\":\"" +String(makeTime(m_alarm)) + "\"";*/
	return ss;
}

String SettingManager::toString(boolean bJson)
{
	String ss;
	if (bJson == STD_TEXT)
	{
		ss = BaseSettingManager::toString(bJson);
	}
	else
	{
		ss = toStringCfg(bJson);
	}
	return ss;
}

unsigned char SettingManager::readData()
{
	BaseSettingManager::readData();
	switchOn();

	String str = "";
	// Read configuartion file
	char cfgFileName[] = "/config.json";
	if (SPIFFS.exists(cfgFileName))
	{
		DynamicJsonDocument doc(3000);
		File file = SPIFFS.open(cfgFileName, FILE_READ);
		DeserializationError error = deserializeJson(doc, file);
		if (!error)
		{
			/*m_dayIntensity   = doc[F("dayIntensity")];
			m_nightIntensity = doc[F("nightIntensity")];
			m_dayHour 		 = doc[F("dayHour")];
			m_nightHour 	 = doc[F("nightHour")];

			m_mainColor =  doc[F("mainColor")].as<uint32_t>();
			m_mainAnnimation = String(doc[F("mainAnnimation")].as<char*>());
			breakTime(doc[F("alarm")].as<uint32_t>(),m_alarm);*/
		}
		else
		{
			DEBUGLOGF("Config file SART READING [%s]", error.c_str());
		}
		file.close();
	}
	else
	{
		DEBUGLOG("Config file does not exist");
	}

	switchOff();
	return m_iEEprom;
}

unsigned char SettingManager::writeData()
{
	BaseSettingManager::writeData();
	switchOn();

	//Write Spiff configuration
	String ss;
	ss = "{\"datetime\":{" + wfManager->getHourManager()->toDTString(JSON_TEXT) + "},";
	ss += toStringCfg(JSON_TEXT) + "}";
	File cfgFile = SPIFFS.open("/config.json", FILE_WRITE);
	if (cfgFile.print(ss) == ss.length())
	{
		DEBUGLOG("Configuration file saved");
	}
	else
	{
		DEBUGLOG("Configuration PROBLEM");
	};
	cfgFile.close();

	return m_iEEprom;
}
