#ifndef GAMEAUDIOCOMMON_H
#define GAMEAUDIOCOMMON_H

#include <JSystem/JMath/JMath.h>

namespace GameAudio
{
    class Common
    {
    public:
        static f32 panDeform(f32, f32);
        static f32 setMultiPlayModePan(u8); // this is a getter, not a setter
        static u32 changeRandomId(u32, u32);
    };

    class Random
    {
    public:
        static void setSeed(u32 seed);
        static u32 getRandomU32();
        static u32 getSignalEngineRandomU32();

        static void *getMgr(); // Unused, however i assume it returned sRandom?

    private:
        static JMath::TRandom_<JMath::TRandom_fast_> sRandom;
        static JMath::TRandom_<JMath::TRandom_fast_> sSignalEngineRandom;
    };
}

#endif