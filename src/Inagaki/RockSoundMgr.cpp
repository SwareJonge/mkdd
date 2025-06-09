
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

namespace GameAudio {

static const f32 GA_MULTI_VOLUME[4] = {
    1.0f, 0.8f, 0.6f, 0.6f
};

RockSoundMgr::RockSoundMgr(Vec *pos, f32 p2, JKRHeap *heap) : GameAudio::SoundMgr<RockSoundMgr>(pos, heap, 8) {
    _60 = p2;
    _5c = 0;
    
    if (_60 < 0.3f) {
        _64 = 0;
    }
    else if (_60 < 0.8f) {
        _64 = 1;
    }
    else {
        _64 = 2;
    }
}

RockSoundMgr::~RockSoundMgr() {}

void RockSoundMgr::init() {
    stopAllSounds();
}

void RockSoundMgr::loop() {
#line 119
    JUT_ASSERT_MSG(mSoundPos, "RockSoundMgr::loop　disable中です！！！！！\n");
    SoundMgr::loop();
    _5c = 0;
}

void RockSoundMgr::frameWork() {
    if (_48)
        return;
}

void RockSoundMgr::setRollSe(f32) {}

void RockSoundMgr::setBoundSe(f32) {}

void RockSoundMgr::setBrokenSe() {}

}
