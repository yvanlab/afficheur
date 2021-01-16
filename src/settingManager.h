
#ifndef SettingManager_h
#define SettingManager_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif

#include "ArduinoJson.h"

#include <EEPROM.h>
#include <baseManager.h>
#include <BaseSettingManager.h>
//#include "matrixPages.h"


class SettingManager : public BaseSettingManager
{
public:
	SettingManager(unsigned char pinLed);
	virtual uint8_t readData();
	virtual uint8_t writeData();

	void writePage();
	void readPage();

	String getClassName() { return "SettingManager"; }

	//void sortPages();

	String toString(boolean bJson);
	String toStringCfg(boolean bJson);

	String m_mainColor;
	String m_mainAnnimation;

};

#endif
