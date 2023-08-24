#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

namespace GameAudio {
    class Main {
    public:
        void init(JKRSolidHeap *, u32, void *, void *, u32);
        void startSystemSe(u32);
        void initRaceSound();
        void bootDSP();
        void setBgmVolume(f32);
        bool isActive();
        void frameWork();

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

#endif