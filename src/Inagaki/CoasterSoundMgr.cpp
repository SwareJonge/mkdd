
#include "Inagaki/GameMapSoundMgr.h"
#include "Inagaki/GameSoundMgr.h"

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

namespace GameAudio {

CoasterSoundMgr::CoasterSoundMgr(Vec *pos, JKRHeap *heap) : SoundWithEchoMgr<CoasterSoundMgr>(pos, heap, 2) {
    _68 = pos->y;
    _6c = 0;
    _70 = 0;
}

CoasterSoundMgr::~CoasterSoundMgr() {}

void CoasterSoundMgr::frameWork(f32 p1) {
    if (_48)
        return;

    checkScream(p1);
    setEngineSound(p1);
    checkEcho();
    loop();
}

void CoasterSoundMgr::setEngineSound(f32 p1) {
    if (_48)
        return;

    if (p1 < 1.0f)
        return;

    if (p1 > 100.0f) {
        p1 = 100.0f;
    }
    f32 pitch = (p1 / 100.0f) * 0.5f + 1.0f;

    JAISoundHandle *handle = startLevelSound(0x40028);
    if (handle) {
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }
}

void CoasterSoundMgr::checkScream(f32 p1) {
    f32 yDiff = _68 - mSoundPos->y;
    if (yDiff >= 30.0f) {
        _70++;
    }
    else if (yDiff <= 0.0f) {
        _70 = 0;
        _6c = 0;
    }

    if (_70 > 4 && !_6c) {
        _6c = 1;
        JAISoundHandle *handle = startSound(0x40029);
        if (handle && (*handle)) {
            if (p1 > 100.0f) {
                p1 = 100.0f;
            }
            p1 = (p1 / 100.0f) * 1.25f + 0.1f;
            (*handle)->getAuxiliary().movePitch(p1, 0);
        }
    }
    _68 = mSoundPos->y;
}

}
