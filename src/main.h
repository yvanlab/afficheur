
#ifndef main_h
#define main_h

#include <Arduino.h>


#include <wifiManagerV2.h>
#include <myTimer.h>
#include "settingManager.h"
#include "networkUI.h"
#include "horlogeManager.h"
#include "soundManager.h"
//#include "displayHour.h"
//#include <FlashLED.h>




#define PIN_LED 2

#define MODULE_NAME     HORLOGE_NAME
#define MODULE_MDNS     HORLOGE_MDNS
#define MODULE_MDNS_AP  HORLOGE_MDNS_AP
#define MODULE_IP       HORLOGE_IP


extern SettingManager     *smManager;
extern WifiManager        *wfManager;
//extern DislayHour         *affManager;



#ifdef MCPOC_TELNET
//extern RemoteDebug          Debug;
#endif

#ifdef ESP32
extern portMUX_TYPE 		wtimerMux;// = portMUX_INITIALIZER_UNLOCKED;
#endif

#ifdef ESP32
extern "C"
{
  uint8_t temprature_sens_read();
}
#endif
#ifdef ESP8266
uint8_t temprature_sens_read()
{
  return 0;
}
#endif



#endif
