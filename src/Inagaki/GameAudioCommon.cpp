#include "Inagaki/GameAudioCommon.h"
#include "Inagaki/GameAudioMain.h"

namespace GameAudio
{
    JMath::TRandom_<JMath::TRandom_fast_> Random::sRandom(0);
    JMath::TRandom_<JMath::TRandom_fast_> Random::sSignalEngineRandom(0);

    f32 Common::panDeform(f32 x, f32 y) 
    {
        if(x > 0.5f) {
            f32 ret = 0.5f + (x - 0.5f) * y;
            if(ret > 1.0f)
                ret = 1.0f;
            return ret;
        } 
        else {
            f32 ret = 0.5f - (0.5f - x) * y;
            if (ret < 0.0f)
                ret = 0.0f;
            return ret;
        }
    }

    f32 Common::setMultiPlayModePan(u8 p1) { return (p1 & 1); }

    u32 Common::changeRandomId(u32 id1, u32 id2)
    {
        u32 swBit = GetGameAudioMain()->getSoundTable()->getSwBit(id1);

        if (swBit & 0x70000000) {
            swBit = ((swBit & 0x70000000) >> 0x1c) + 1;
            u32 rnd = Random::getRandomU32();
            u32 randId = id1 + ((rnd >> 0xc) % swBit);
            if (randId == id2 && (++randId == id1 + swBit))
            {
                randId = id1;
            }
            return randId;
        }
        return id1;
    }

    void Random::setSeed(u32 seed)
    {
        sRandom.setSeed(seed);
        sSignalEngineRandom.setSeed(seed);
    }

    u32 Random::getRandomU32() { return 4294967295.0f * sRandom.get_ufloat_1(); }
    u32 Random::getSignalEngineRandomU32() { return 4294967295.0f * sSignalEngineRandom.get_ufloat_1(); }
}

#include <JSystem/JAudio/JASFakeMatch4.h>