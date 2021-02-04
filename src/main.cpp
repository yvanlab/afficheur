#include "FastLED.h"

#include <wifiManagerV2.h>
#include <myTimer.h>
#include <HTTPClient.h>
#include "main.h"
//#include <baseManager.h>
#include "displayHour.h"
#include "displayBase.h"
#include "horlogeManager.h"

// Jaune 33
// Violet 32
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif


//#define NUM_LEDS 86

//Afficheur aff(leds);

SettingManager  *smManager; //(PIN_LED);
WifiManager     *wfManager;	   //(PIN_LED, &smManager);
HorlogeManager  *hlManager;

DisplayHour     *affManager;
CRGB *leds;
DelayHelper      delayMode;

portMUX_TYPE wtimerMux = portMUX_INITIALIZER_UNLOCKED;
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.



void startWiFiserver()
{
	if (wfManager->begin(IPAddress(MODULE_IP), MODULE_NAME, MODULE_MDNS,
						 MODULE_MDNS_AP) == WL_CONNECTED)
	{
/*		wfManager->getServer()->on("/", dataPage);
		wfManager->getServer()->onNotFound(dataPage);*/
	}
	wfManager->getServer()->on("/status", dataJson);
	wfManager->getServer()->on("/setData", setData);
	wfManager->getServer()->on("/config", configPage);
  wfManager->getServer()->on("/save", saveConfiguration);  

#ifdef OTA_FOR_ATOM
	ArduinoOTA.onStart(OTAOnStart);
#endif

	Serial.println(wfManager->toString(STD_TEXT));
}



void setup()
{

  //delay(3000); // 3 second delay for recovery

  Serial.begin(115200); //delay(500);
  Serial.println("start debuging");
  DEBUGLOGF("Frq : %d \n", ESP.getCpuFreqMHz());
	DEBUGLOGF("Temp : %f \n", temperatureRead());

  
  smManager = new SettingManager(PIN_LED);
	wfManager = new WifiManager(PIN_LED, smManager);
  hlManager = new HorlogeManager(smManager);
  
  affManager = hlManager->getDisplayHour(); 
  leds = affManager->getLeds();

	if (!SPIFFS.begin(true))
	{
		Serial.println("SPIFFS Mount Failed");
		return;
	}
	else
		Serial.println("SPIFFS Mount OK");

	smManager->readData();
	DEBUGLOG(smManager->toString(STD_TEXT));
	startWiFiserver();

  mtTimer.begin(timerFrequence);
  mtTimer.setCustomMS(25);
  smManager->m_mode = SettingManager::MODE_CLOCK;
  hlManager->setConfig();
}

#ifdef MCPOC_TEST
int iLed = 0;
uint8_t iValue = 0;
uint8_t iColor = 0;
uint8_t iPoint = 0;
uint8_t iMode = 0;
CRGB cc[] = {CRGB(255, 0, 0), CRGB(0, 255, 0), CRGB(0, 0, 255), CRGB(0, 255, 255), CRGB(255, 0, 255), CRGB(255, 255, 0), CRGB(255, 255, 255), CRGB(50, 50, 50)};

#endif

void loop()
{
wfManager->handleClient();


#ifdef MCPOC_TEST
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.print(c);
    smManager->m_mode=SettingManager::MODE_TEST;
    if (c == 'n')
    {
      iLed++;
      if (iLed>=NUM_LEDS)
        iLed =0;
      Serial.printf("iLed [%d]\n", iLed);
      leds[iLed] = CRGB(0, 255, 0);
      FastLED.show();
    }
    else if (c == 'a')
    {
      for (uint8_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = cc[iColor % 8];
      }
      iColor++;
      FastLED.show();
    }
    else if (c == 'c')
    {
      for (uint8_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB(0, 0, 0);
      }
      affManager->setState(false);
      //p->setState(false);
      iLed = 0;
      FastLED.show();
    }
    else if (c == 'v')
    {
      affManager->setColorON(CRGB(0, 0, 255));
      affManager->setColorOFF(CRGB(0, 255, 255));
      Serial.printf("value[%d]\n", iValue % 100);
      affManager->setValue(iValue % 100, DisplayHour::HOUR);
      affManager->setValue(iValue % 100, DisplayHour::MINUTE);
      affManager->setValue(iValue % 100, DisplayHour::SECONDE);
     
      //affManager->display(true, Afficheur::LAST_ELT);
      iValue++;
      FastLED.show();
    }

    else if (c == 'p')
    {
      affManager->setValue(iPoint, DisplayHour::POINT_HR);
      affManager->setValue(iPoint, DisplayHour::POINT_MN);
      affManager->handleMode();
      iPoint = iPoint ^ 255;
      FastLED.show();
    }
    else if (c == 'm')
    {
      affManager->setMode((DisplayBase::MODE_LED)(iMode % DisplayBase::MODE_LED_LAST));
      iMode++;
      FastLED.show();
    }
    else if (c == 'h')
    {
     smManager->m_mode = SettingManager::MODE_CLOCK;
    }
    else if (c == 'd')
    {
      affManager->setColorON(CRGB(0, 0, 125));
      affManager->setColorOFF(CRGB(0, 0, 0));
      affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::HOUR);
      affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::MINUTE);
      affManager->setValue(DisplayDoubleDigit::DASH_BOTH, DisplayHour::SECONDE);
    }
  }

  switch(smManager->m_mode ) {
    case (SettingManager::MODE_REBOOT) : {
      hlManager->displayAfterReboot();
      break;
    }
    case (SettingManager::MODE_CLOCK) : {
      //hlManager->setConfig();
      hlManager->displayHour();
      
      break;
    }
    case (SettingManager::MODE_ALARM) : {

      break;
    }
    case (SettingManager::MODE_COUNTDOWN) : {
      if (!hlManager->displayCountdown()) {
        smManager->m_mode = SettingManager::MODE_COUNTDOWN_END;
      }
      break;
    }
    case (SettingManager::MODE_COUNTDOWN_END) : {
      if (!hlManager->displayCountdownEnd()) {
        smManager->m_mode = SettingManager::MODE_CLOCK;
      };
      break;
      case (SettingManager::MODE_TEST) : break;
    }
   



  }
    hlManager->handle();
 

#endif
}

  //pride();
  /*for (uint8_t i=0;i<NUM_LEDS; i++) {
    leds[i] = CRGB(255,255,255);
    delay(500); 
    FastLED.show();  
  }
  for (uint8_t i=0;i<NUM_LEDS; i++) {
    leds[i] = CRGB(0,0,0);
    delay(500);
    FastLED.show();
  }
  for (uint8_t i=0;i<NUM_LEDS; i++) {
    leds[i] = CRGB(255,0,0);
    if (i>0) leds[i-1] = CRGB(0,0,0);
    delay(500);
    FastLED.show();
  }
  leds[NUM_LEDS-1] = CRGB(0,0,0);*/

  //aff.displayValue(now());

