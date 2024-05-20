#include "JSystem/JAudio/Interface/JAISound.h"

// .text ordering is incorrect here(debug only), luckily it doesn't matter

#ifdef DEBUG
#pragma sym on
#endif

JAISoundStarter::JAISoundStarter(bool p1) : JASGlobalInstance<JAISoundStarter>(p1) {}

JAISoundStarter::~JAISoundStarter() {}

bool JAISoundStarter::startLevelSound(JAISoundID soundID, JAISoundHandle *handlePtr, const JGeometry::TVec3f *pos)
{
    // UNUSED
#line 20
    JUT_ASSERT(handlePtr)

    if (*handlePtr) {
        if ((u32)soundID == (u32)(*handlePtr)->getID())
        {
            (*handlePtr)->updateLifeTime(1);

            if(pos) {
                (*handlePtr)->setPos(*pos);
            }
        }

        return true;
    }

    bool ret = startSound(soundID, handlePtr, pos);
    if (ret && (*handlePtr))
        (*handlePtr)->setLifeTime(1, false);
    return ret;
}

#include "JSystem/JAudio/JASFakeMatch.h"

#ifdef DEBUG
#pragma sym off
#endif


