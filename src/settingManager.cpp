

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
	ss = "\"mainColor\":\"" + String(m_mainColor) +" \",";
	ss = "\"dayIntensity\":\"" + String(m_dayIntensity) +" \",";
	ss = "\"nightIntensity\":\"" + String(m_nightIntensity) +" \",";
	ss = "\"dayHour\":\"" + String(m_dayHour) +" \",";
	ss = "\"nightHour\":\"" + String(m_nightHour) +" \",";
	ss += "\"mainAnnimation\":\"" +m_mainAnnimation + "\",";
	// dd-mm-yyyy hh:mm:ss
	ss += "\"alarm\":\"" +String(makeTime(m_alarm)) + "\"";
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
			m_dayIntensity   = doc[F("dayIntensity")];
			m_nightIntensity = doc[F("nightIntensity")];
			m_dayHour 		 = doc[F("dayHour")];
			m_nightHour 	 = doc[F("nightHour")];

			m_mainColor =  doc[F("mainColor")].as<uint32_t>();
			m_mainAnnimation = String(doc[F("mainAnnimation")].as<char*>());
			breakTime(doc[F("alarm")].as<uint32_t>(),m_alarm);
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
