#include "FastLED.h"

#include <wifiManagerV2.h>
#include <myTimer.h>
#include <HTTPClient.h>
#include "main.h"
//#include <baseManager.h>
#include "afficheur.h"
#include "digit.h"
#include "doubleDigit.h"
#include "points.h"
#include <sdkconfig.h>

// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 2
//#define CLK_PIN   4
#define LED_TYPE SK6812
#define COLOR_ORDER RGB
#define NUM_LEDS 86
#define BRIGHTNESS 255

CRGB leds[NUM_LEDS];
//Afficheur aff(leds);

/*Digit *d;
DoubleDigit *dd;
Points *p;*/
Afficheur *aff;
portMUX_TYPE wtimerMux = portMUX_INITIALIZER_UNLOCKED;
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; //gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void setup()
{

  delay(3000); // 3 second delay for recovery

  Serial.begin(115200); //delay(500);
  Serial.println("start debuging");
  /*DEBUGLOGF("Frq : %d \n", ESP.getCpuFreqMHz());
	DEBUGLOGF("Temp : %f \n", temperatureRead());*/

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip)
      .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i].red = i;
  }
  FastLED.show();
  aff = new Afficheur(leds);
  /*dd = new DoubleDigit(leds);
  p = new Points(&leds[dd->getNbPixels()]);*/
  //ddd = new DoubleDigit(&leds[dd->getNbPixels()+p->getNbPixels()]);

  

  mtTimer.begin(timerFrequence);
  mtTimer.setCustomMS(25);
}

#ifdef MCPOC_TEST
uint8_t iLed = 0;
uint8_t iValue = 0;
uint8_t iColor = 0;
uint8_t iPoint = 0;
uint8_t iMode = 0;
CRGB cc[] = {CRGB(255, 0, 0), CRGB(0, 255, 0), CRGB(0, 0, 255), CRGB(0, 255, 255), CRGB(255, 0, 255), CRGB(255, 255, 0), CRGB(255, 255, 255), CRGB(50, 50, 50)};

#endif

void loop()
{

#ifdef MCPOC_TEST
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.print(c);
    if (c == 'n')
    {
      iLed++;
      iLed = iLed % NUM_LEDS;
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
      aff->setState(false);
      //p->setState(false);
      iLed = 0;
      FastLED.show();
    }
    else if (c == 'v')
    {
      aff->setColorON(CRGB(0, 0, 255));
      Serial.printf("value[%d]\n", iValue % 100);
      aff->setValue(iValue % 100, Afficheur::HOUR);
      aff->setValue(iValue % 100, Afficheur::MINUTE);
     
      //aff->display(true, Afficheur::LAST_ELT);
      iValue++;
      FastLED.show();
    }

    else if (c == 'p')
    {
      aff->setValue(iPoint % 2, Afficheur::POINT_HR);
      aff->HandleMode();
      iPoint++;
      FastLED.show();
    }
    else if (c == 'm')
    {
      aff->setMode((DisplayElement::MODE_LED)(iMode % 4));
      iMode++;
      FastLED.show();
    }
  }
   aff->HandleMode();
  FastLED.show();

#endif
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
}
