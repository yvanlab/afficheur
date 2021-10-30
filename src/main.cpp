#include "FastLED.h"

#include <wifiManagerV2.h>
#include <myTimer.h>
#include <HTTPClient.h>
#include "main.h"
//#include <baseManager.h>
#include "displayHour.h"
#include "displayBase.h"
#include <fs.h>
#include "SPIFFS.h"

// https://github.com/ChrisVeigl/ESP8266Audio/commit/1ce70dcd14a70cfe5ebe899e761cfc4f9e3ec278

// #include "AudioFileSourceSPIFFS.h"
// #include "AudioFileSourceID3.h"
// #include "AudioGeneratorMP3.h"
// #include "AudioOutputI2S.h"
// #include "AudioOutputI2SNoDAC.h"

void startWiFiserver();
void displayManagement(void *pvParameters);

// Jaune 33
// Violet 32
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

//#define NUM_LEDS 86

//Afficheur aff(leds);

SettingManager *smManager; //(PIN_LED);
WifiManager *wfManager;    //(PIN_LED, &smManager);
HorlogeManager *horlogelMgr;
SoundManager *soundMgr;

DisplayHour *affManager;
CRGB *leds;
DelayHelper delayMode;

portMUX_TYPE wtimerMux = portMUX_INITIALIZER_UNLOCKED;
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.

void startWiFiserver()
{
  if (wfManager->begin(IPAddress(MODULE_IP), MODULE_NAME, MODULE_MDNS,
                       MODULE_MDNS_AP) == WL_CONNECTED)
  {
    wfManager->getServer()->on("/", dataPage);
    wfManager->getServer()->onNotFound(dataPage);
  }
  wfManager->getServer()->on("/status", dataJson);
  wfManager->getServer()->on("/setData", setData);
  wfManager->getServer()->on("/config", configPage);
  wfManager->getServer()->on("/save", saveConfiguration);
  wfManager->getServer()->on("/setting", dataPage);

#ifdef OTA_FOR_ATOM
  ArduinoOTA.onStart(OTAOnStart);
#endif

  Serial.println(wfManager->toString(STD_TEXT));
}

// AudioGeneratorMP3 *mp3;
// AudioFileSourceSPIFFS *file;
// AudioOutputI2S *out;
// AudioFileSourceID3 *id3;

//int SineValues[256];
TaskHandle_t Task1;

void setup()
{

  //delay(3000); // 3 second delay for recovery

  Serial.begin(115200); //delay(500);
  Serial.println("start debuging");
  DEBUGLOGF("Frq : %d \n", ESP.getCpuFreqMHz());
  DEBUGLOGF("Temp : %f \n", temperatureRead());

  smManager = new SettingManager(PIN_LED);
  wfManager = new WifiManager(PIN_LED, smManager);
  horlogelMgr = new HorlogeManager(smManager);
  soundMgr = new SoundManager(smManager);

  affManager = horlogelMgr->getDisplayHour();
  leds = affManager->getLeds();

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  else
  {
    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file)
    {
      DEBUGLOGF("files[%s]\n", file.name());

      file = root.openNextFile();
    }
  }
  soundMgr->begin();
  smManager->readData();
  DEBUGLOG(smManager->toString(STD_TEXT));
  startWiFiserver();

  mtTimer.begin(timerFrequence);
  mtTimer.setCustomMS(25);
  smManager->m_mode = SettingManager::MODE_CLOCK;
  horlogelMgr->setConfig();

  xTaskCreatePinnedToCore(
      displayManagement, /* Task function. */
      "Task1",           /* name of task. */
      10000,             /* Stack size of task */
      NULL,              /* parameter of the task */
      1,                 /* priority of the task */
      &Task1,            /* Task handle to keep track of created task */
      0);                /* pin task to core 0 */

  // audioLogger = &Serial;
  // file = new AudioFileSourceSPIFFS("/viola.mp3");
  // id3 = new AudioFileSourceID3(file);
  // out = new AudioOutputI2S(0, 1);
  // mp3 = new AudioGeneratorMP3();
  // //out->SetOutputModeMono(true);
  // mp3->begin(id3, out);
}

#ifdef MCPOC_TEST
int iLed = 0;
uint8_t iValue = 0;
uint8_t iColor = 0;
uint8_t iPoint = 0;
uint8_t iMode = 0;
uint8_t iSound = 0;
CRGB cc[] = {CRGB(255, 0, 0), CRGB(0, 255, 0), CRGB(0, 0, 255), CRGB(0, 255, 255), CRGB(255, 0, 255), CRGB(255, 255, 0), CRGB(255, 255, 255), CRGB(50, 50, 50)};

#endif

void loop()
{

  //DEBUGLOG("Task1 running on second core ");
  wfManager->handleClient();
  soundMgr->handle();

  if (smManager->m_soundToPlay != smManager->NO_SOUND)
  {
    soundMgr->playSound(smManager->m_soundToPlay);
    smManager->m_soundToPlay = smManager->NO_SOUND;
    DEBUGLOGF("start sound [%d]\n", esp_get_free_heap_size());
  };
  soundMgr->handle();
  delay(1);
}

void displayManagement(void *pvParameters)
{
  while (1)
  {

#ifdef MCPOC_TEST
    if (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      smManager->m_mode = SettingManager::MODE_TEST;
      if (c == 'n')
      {
        iLed++;
        if (iLed >= NUM_LEDS)
          iLed = 0;
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
       else if (c == 's')
      {
        iSound++;
        smManager->m_soundToPlay = iSound % 5;
      }
    }
#endif

    switch (smManager->m_mode)
    {
    case (SettingManager::MODE_REBOOT):
    {
      horlogelMgr->displayAfterReboot();
      break;
    }
    case (SettingManager::MODE_CLOCK):
    {
      //horlogelMgr->setConfig();
      horlogelMgr->displayHour();

      break;
    }
    case (SettingManager::MODE_ALARM):
    {

      break;
    }
    case (SettingManager::MODE_COUNTDOWN):
    {
      if (!horlogelMgr->displayCountdown())
      {
        smManager->m_mode = SettingManager::MODE_COUNTDOWN_END;
      }
      break;
    }
    case (SettingManager::MODE_COUNTDOWN_END):
    {
      if (!horlogelMgr->displayCountdownEnd())
      {
        smManager->m_mode = SettingManager::MODE_CLOCK;
      };
      break;
    case (SettingManager::MODE_TEST):
      break;
    }
    }
    horlogelMgr->handle();
  }
}
