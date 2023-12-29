#ifndef GAMEAUDIOARCLOADER_H
#define GAMEAUDIOARCLOADER_H

#include "types.h"
#include <JSystem/JAudio/JAUAudioArchive.h>

/*
    baac header:
    u32 sectionCnt;
    u32 sectionOffsets[sectionCnt];
*/

namespace GameAudio
{
    class CustomArcLoader : public JAUAudioArcLoader
    {
    public:
        CustomArcLoader(JAUSection *section);
        virtual ~CustomArcLoader();
        virtual bool readCommandMore(u32 cmd);

    private:
        const void *mData; // points to SceneJPN.baac
    };
}

#endif
