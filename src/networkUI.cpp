
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
		DEBUGLOGF("[%s,%s]", wfManager->getServer()->argName(i).c_str(), wfManager->getServer()->arg(i).c_str());
	}
	DEBUGLOG("");
#endif

		int Year, Month, Day, Hour, Minute, Second ;


	String str;

	// set time
	
	if ((str = wfManager->getServer()->arg("time")) != NULL)
	{
		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &Year, &Month, &Day, &Hour, &Minute, &Second);
		setTime(Hour, Minute, Second, Day, Month, Year);
	}
	if ((str = wfManager->getServer()->arg("cptRebours")) != NULL)
	{
		smManager->m_compteARebour = str.toInt();
		smManager->m_mode = SettingManager::MODE_COMPTEAREBOURS;
	}
	if ((str = wfManager->getServer()->arg("hourAlarm")) != NULL)
	{
//		#include <TimeLib.h>
		tmElements_t tm;
		sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &tm.Year, &tm.Month, &tm.Day, &tm.Hour, &tm.Minute, &tm.Second);
		smManager->m_alarm = tm;
	}
	if ((str = wfManager->getServer()->arg("mainColor")) != NULL)
	{
		smManager->m_mainColor = str.toInt();//str;
	}
	if ((str = wfManager->getServer()->arg("mainAnnimation")) != NULL)
	{
		smManager->m_mainAnnimation = str;
	}
	if ((str = wfManager->getServer()->arg("dayIntensity")) != NULL)
	{
		smManager->m_dayIntensity = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("nightIntensity")) != NULL)
	{
		smManager->m_nightIntensity = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("dayHour")) != NULL)
	{
		smManager->m_dayHour = str.toInt();
	}
	if ((str = wfManager->getServer()->arg("nightHour")) != NULL)
	{
		smManager->m_nightHour = str.toInt();
	}

/*	else if (wfManager->getServer()->hasArg("prgHour") || wfManager->getServer()->hasArg("prgPage") || wfManager->getServer()->hasArg("prgDuration") )
	{
		changed = manageProg();
	}
	else if (wfManager->getServer()->hasArg("page"))
	{
		changed = managePage();
	}

	phPresence->forceStatus(true);*/
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


