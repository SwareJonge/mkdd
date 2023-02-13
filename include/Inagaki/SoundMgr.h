#ifndef SOUNDMGR_H
#define SOUNDMGR_H

#include "types.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JMath/JMath.h"

namespace GameAudio
{
    template <typename T>
    class SoundMgr : JAUDopplerSoundObject, JKRDisposer
    {
        SoundMgr(Vec *, JKRHeap *, u8);
        virtual ~SoundMgr();
        virtual void init();
        virtual void setSe(u32 id);
        virtual void loop(u32);
    };


    namespace Random 
    {
        void setSeed(u32);
        u32 getRandomU32();
        u32 getSignalEngineRandomU32();

        extern JMath::TRandom_fast_ sRandom;
        extern JMath::TRandom_fast_ sSignalEngineRandom;
    }

}

#endif