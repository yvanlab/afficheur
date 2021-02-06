
#include "main.h"

String getJson()
{
	DEBUGLOG("getJson");
	//portENTER_CRITICAL_ISR(&wtimerMux);

	String tt("{\"module\":{");
	tt += "\"nom\":\"" + String(MODULE_NAME) + "\",";
	tt += "\"version\":\"" + String(HORLOGE_VERSION) + "\",";
	tt += "\"status\":\"" + String(STATUS_PERIPHERIC_WORKING) + "\",";
	tt += "\"uptime\":\"" + NTP.getUptimeString() + "\",";
	tt += "\"datetime\":{" + wfManager->getHourManager()->toDTString(JSON_TEXT) + "},";
	tt += "\"temp\":\"" + String((temprature_sens_read() - 32) / 1.8) + "\",";
	tt += "\"build_date\":\"" + String(__DATE__ " " __TIME__) + "\"},";

	tt += "\"setting\":{" + smManager->toString(JSON_TEXT) + "},";
	tt += "\"LOG\":[" + wfManager->log(JSON_TEXT) + "," + smManager->log(JSON_TEXT) + /*","+bmpMesure->log(JSON_TEXT)+*/ "]";
	tt += "}";

	//portEXIT_CRITICAL_ISR(&wtimerMux);
	return tt;
}

void saveConfiguration()
{
	smManager->writeData();
}

#ifdef OTA_FOR_ATOM
void OTAOnStart()
{
	digitalWrite(PIN_LED, LOW);
	mpPages->stopTimer();
	DEBUGLOG("My OTA");
	wfManager->OTAOnStart();
	digitalWrite(PIN_LED, HIGH);
}
#endif

void configPage()
{
	digitalWrite(PIN_LED, LOW);
	DEBUGLOG("config Page");
	wfManager->loadFromSpiffs("/config.json");
	digitalWrite(PIN_LED, HIGH);
}

void dataJson()
{
	digitalWrite(PIN_LED, LOW);
	//mpPages->stopTimer();
	wfManager->getServer()->send(200, "text/json", getJson());
	//mpPages->startTimer();
	digitalWrite(PIN_LED, HIGH);
}

void setData()
{
	bool changed = false;
	//String str;

	DEBUGLOG("SetData");
#ifdef MCPOC_TEST
	for (uint8_t i = 0; i < wfManager->getServer()->args(); i++)
	{
		DEBUGLOGF("[%s,%s]\n", wfManager->getServer()->argName(i).c_str(), wfManager->getServer()->arg(i).c_str());
	}
	DEBUGLOG("");
#endif

	int Year, Month, Day, Hour, Minute, Second;

	String str;

	// set time

	if ((str = wfManager->getServer()->arg("time")) != NULL)
	{
		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &Year, &Month, &Day, &Hour, &Minute, &Second);
		setTime(Hour, Minute, Second, Day, Month, Year);
	}
	if ((str = wfManager->getServer()->arg("mode")) != NULL)
	{
		smManager->m_mode = (SettingManager::MODE_HORLOGE)str.toInt();
	}



	/*
	"rebootColorOn" : "16777215",
   "rebootColorOff" : "0",
   "rebootAnimation" : "DisplayBase::K2000",
	*/

	if ((str = wfManager->getServer()->arg("rebootColorOn")) != NULL)
	{
		smManager->m_rebootColorOn = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("rebootColorOff")) != NULL)
	{
		smManager->m_rebootColorOff = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("rebootAnimation")) != NULL)
	{
		smManager->m_rebootAnimation = (DisplayBase::MODE_LED)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("rebootTransition")) != NULL)
	{
		smManager->m_rebootTransition = (DisplayBase::MODE_TRANSITION)str.toInt();
	}

	/*
 "alarmTrigger": "15:00:00",
   "alarmTriggerAnimation": "0",
   "alarmTriggerSound": "0",
   "alarmTriggerDuration": "20",
*/
	if ((str = wfManager->getServer()->arg("alarmTrigger")) != NULL)
	{
		//		#include <TimeLib.h>
		tmElements_t tm;
		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &tm.Year, &tm.Month, &tm.Day, &tm.Hour, &tm.Minute, &tm.Second);
		//smManager->m_alarm = tm;
	}

	/*

   "clockColorOn": "255",
   "clockColorOff": "0",
   "clockAnimation": "0",
   "clockIntensityNight": "10",
   "clockIntensityDay": "100",
   "clockHourDay": "06",
   "clockHourNight": "20",
   "clockSoundHour": "0",*
*/
	if ((str = wfManager->getServer()->arg("clockColorOn")) != NULL)
	{
		smManager->m_clockColorOn = str.toInt(); //str;
	}
	if ((str = wfManager->getServer()->arg("clockAnimation")) != NULL)
	{
		smManager->m_clockAnimation = (DisplayBase::MODE_LED)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockTransition")) != NULL)
	{
		smManager->m_clockTransition = (DisplayBase::MODE_TRANSITION)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockIntensityNight")) != NULL)
	{
		smManager->m_clockIntensityNight = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockIntensityDay")) != NULL)
	{
		smManager->m_clockIntensityDay = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockHourDay")) != NULL)
	{
		smManager->m_clockHourDay = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockHourNight")) != NULL)
	{
		smManager->m_clockHourNight = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("clockSoundHour")) != NULL)
	{
		smManager->m_clockSoundHour = str.toInt();
	}

	/*
 	"countdownColorOn": "16711680",
   "countdownColorOff": "20",
   "countdownAnimation": "0",
   "countdownAnimationEnd": "0",
   "countdownSoundStart": "0",
   "countdownSoundEnd": "0",
   "countdownDurationEnd": "5",
*/

	if ((str = wfManager->getServer()->arg("countdownCpt")) != NULL)
	{
		smManager->m_countdownCpt = str.toInt();
		smManager->m_mode = SettingManager::MODE_COUNTDOWN;
	}
	if ((str = wfManager->getServer()->arg("countdownColorOn")) != NULL)
	{
		smManager->m_countdownColorOn = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownColorOff")) != NULL)
	{
		smManager->m_countdownColorOff = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownAnimation")) != NULL)
	{
		smManager->m_countdownAnimation = (DisplayBase::MODE_LED)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownTransition")) != NULL)	
	{
		smManager->m_countdownTransition = (DisplayBase::MODE_TRANSITION)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownAnimationEnd")) != NULL)
	{
		smManager->m_countdownAnimationEnd = (DisplayBase::MODE_LED)str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownSoundStart")) != NULL)
	{
		smManager->m_countdownSoundStart = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownSoundEnd")) != NULL)
	{
		smManager->m_countdownSoundEnd = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("countdownDurationEnd")) != NULL)
	{
		smManager->m_countdownDurationEnd = str.toInt();
	}


		wfManager->getServer()->send(200, "text/html", "ok");
	//dataPage();
}

/*void dataPage()
{
	digitalWrite(PIN_LED, LOW);
	mpPages->stopTimer();

	DEBUGLOG("dataPage");
	delay(1000);
	wfManager->loadFromSpiffs("/index.html");
	//wfManager->getServer()->send_P ( 200, "text/html", HTML );

	phPresence->forceStatus(true);
	digitalWrite(PIN_LED, HIGH);
}*/
