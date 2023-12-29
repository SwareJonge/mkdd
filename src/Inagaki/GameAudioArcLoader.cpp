#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameAudioArcLoader.h"
#include "JSystem/JAudio/JASFakeMatch.h"

namespace GameAudio
{
    CustomArcLoader::CustomArcLoader(JAUSection *section) : JAUAudioArcLoader(section)
    {
        mData = nullptr;
    }

    CustomArcLoader::~CustomArcLoader() {}

    bool CustomArcLoader::readCommandMore(u32 cmd)
    {
        switch (cmd)
        {
        case 'baac': // probably abbreviation for Binary Audio Archive Custom?
        {
            u32 beginOffset = readU32_();
            u32 endOffset = readU32_();
            mData = GetGameAudioMain()->getSectionHeap()->newCopy(getContent_(beginOffset), endOffset - beginOffset, 0x20);
            return true;
        }
        }
        return false;
    }
}
