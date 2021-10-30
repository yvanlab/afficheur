

#ifndef SoundManager_h
#define SoundManager_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"
//#include "WProgram.h"
#else
#include "Arduino.h"
#include "WProgram.h"
#endif

#include <myTimer.h>
//#include "SettingManager.h"
//#include "main.h"

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioOutputI2SNoDAC.h"
#include <fs.h>

class SoundManager
{
public:
    SoundManager(SettingManager *smManager)
    {
        m_smManager = smManager;
        audioLogger = &Serial;
        m_file = new AudioFileSourceSPIFFS();
        m_id3 = new AudioFileSourceID3(m_file);
        m_out = new AudioOutputI2S(0, 1);
        m_mp3 = new AudioGeneratorMP3();
        
    }

    void begin() {
        //load mp3
        File root = SPIFFS.open("/");
        File file = root.openNextFile();
        uint8_t idFile = 0;
        String filename;
        while (file)
        {
            filename = String(file.name());
            //DEBUGLOGF("files[%s]\n", filename.c_str());
            if (filename.indexOf(".mp3") > 0)
            {
                m_soundMapping[idFile] = filename;
                DEBUGLOGF("mp3[%s]\n", m_soundMapping[idFile].c_str());
                idFile++;
            }
            file = root.openNextFile();
        }

    }

    void playSound(SettingManager::SOUND_TYPE soundType)
    {
        if (m_mp3->isRunning())
        {
            m_mp3->stop();
            delay(250);
        }
        m_file->open(m_soundMapping[soundType].c_str());
        m_mp3->begin(m_id3, m_out);
        m_out->SetGain(1);
    }

    void handle()
    {
        if (m_mp3->isRunning())
        {
            if (!m_mp3->loop()) {
                m_out->SetGain(0);
                m_mp3->stop();
                
            }
        }
    }

protected:
    SettingManager *m_smManager;

    AudioGeneratorMP3 *m_mp3;
    AudioFileSourceSPIFFS *m_file;
    AudioOutputI2S *m_out;
    AudioFileSourceID3 *m_id3;
    String m_soundMapping[20];
};
#endif
