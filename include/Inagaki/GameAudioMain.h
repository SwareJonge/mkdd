#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

namespace GameAudio {
    class Main {
    public:
        enum WS_ID
        {
            STREAM0,
            STREAM1,
            STREAM2,
            STREAM3
        };

        void init(JKRSolidHeap *, u32, void *, void *, u32);
        void startSystemSe(u32 id);
        void startRaceSystemSe(u32 id);
        void initRaceSound();
        void bootDSP();
        void setBgmVolume(f32);
        bool isActive();
        bool isWaveLoaded(WS_ID);
        void frameWork();
        void setMasterVolume(s8);
        void setOutputMode(u32);
        void resetAudio(u32);
        void resumeAudio();
        f32 getMasterVolumeValue();
        f32 getTHPOptionVolume() { return getMasterVolumeValue() / 2; };

        static Main *getAudio() { return msBasic; };

        static Main *msBasic; 
    };
    namespace Parameters {
        extern u8 getDemoMode();
        extern void setDemoMode(u8);
    }
}

#define GetGameAudioMain() GameAudio::Main::getAudio()
#define GameAudioMain GameAudio::Main
#define GameAudioParameters GameAudio::Parameters

#endif