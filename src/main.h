
#ifndef main_h
#define main_h

#include <Arduino.h>


#include <wifiManagerV2.h>
#include <myTimer.h>
#include <FlashLed.h>




#define PIN_LED 2

#define MODULE_NAME     LA_MATRIX2_NAME
#define MODULE_MDNS     LA_MATRIX2_MDNS
#define MODULE_MDNS_AP  LA_MATRIX2_MDNS_AP
#define MODULE_IP       LA_MATRIX2_IP



#define LAPORTE_OUVERTURE_LABEL  1 //"current"


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
