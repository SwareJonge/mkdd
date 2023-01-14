#ifndef KARTSOUNDMGR_H
#define KARTSOUNDMGR_H

#include "types.h"
#include "Inagaki/SoundMgr.h"

namespace GameAudio
{
    class KartSoundMgr : SoundMgr<KartSoundMgr>
    {
    public:
        KartSoundMgr();
        virtual void dispose();
        virtual ~KartSoundMgr();
        virtual void init();
        virtual void setSe(u32 id);
    };
}

#endif