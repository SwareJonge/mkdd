#ifndef GAMEAUDIOMAIN_H
#define GAMEAUDIOMAIN_H

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "types.h"

namespace GameAudio {
    class Main {
    public:
        void init(JKRSolidHeap *, u32, void *, void *, u32);
        void startSystemSe(u32);
        void initRaceSound();
        static Main *getAudio() { return msBasic; };

        static Main * msBasic; 
    };
}

#endif