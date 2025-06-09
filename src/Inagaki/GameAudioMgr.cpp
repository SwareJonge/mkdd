
#include "Inagaki/GameAudioMgr.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JAudio/JAUSoundMgr.h"
#include "JSystem/JUtility/JUTAssert.h"

#include "JSystem/JAudio/JASFakeMatch5.h"

namespace GameAudio {

CustomMgr::CustomMgr() {
    _80c = 0;
    _80d = 0;
    _80e = 0;
}

bool CustomMgr::startSound(JAISoundID soundID, JAISoundHandle *handlePtr, const JGeometry::TVec3f *posPtr) {
    CustomSoundTable *table = Main::getAudio()->getSoundTable();
    if (table->getLevelFlag(soundID) != 0) {
#line 56
        JUT_ASSERT(handlePtr);

        if (handlePtr->isSoundAttached()) {
            if ((*handlePtr)->getID() == soundID) {
                (*handlePtr)->updateLifeTime(1);
                if (posPtr) {
                    (*handlePtr)->setPos(*posPtr);
                }
                return true;
            }
        }
        bool ret = JAUSoundMgr::startSound(soundID, handlePtr, posPtr);
        if (ret && (*handlePtr)) {
            (*handlePtr)->setLifeTime(1, false);
        }

        return ret;
    }
    else {
        return JAUSoundMgr::startSound(soundID, handlePtr, posPtr);
    }
}

}
