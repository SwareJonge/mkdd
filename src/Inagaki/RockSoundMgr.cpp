#include "Inagaki/GameAudioCamera.h"
#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
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

void RockSoundMgr::setRollSe(f32 rate) {
    if (_48) {
        return;
    }

    if (rate > 70.0f) {
        rate = 70.0f;
    }

    rate /= 70.0f; // maybe use a new temp?
    f32 volume = (rate * 0.5f + 0.5f);
    f32 pitch = (rate * 2.0f + 0.5f);
    JAISoundHandle *handle = startSoundCustom(0x4000a + _64, 0);
    if (handle && (*handle)) {
        u8 camNo = Main::getAudio()->getCamera()->_8;
        (*handle)->getAuxiliary().moveVolume(GA_MULTI_VOLUME[camNo - 1] * volume, 0);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }
}

void RockSoundMgr::setBoundSe(f32 rate) {
    if (_48) {
        return;
    }

    if (rate > 70.0f) {
        rate = 70.0f;
    }

    rate /= 70.0f; // maybe use a new temp?
    f32 volume = (rate * 0.8f + 0.2f);
    f32 pitch = (rate + 0.5f);
    JAISoundHandle *handle = startSound(0x40007 + _64);
    if (handle && (*handle)) {
        u8 camNo = Main::getAudio()->getCamera()->_8;
        (*handle)->getAuxiliary().moveVolume(GA_MULTI_VOLUME[camNo - 1] * volume, 0);
        (*handle)->getAuxiliary().movePitch(pitch, 0);
    }
}

void RockSoundMgr::setBrokenSe() {
    if (_48) {
        return;
    }

    JAISoundHandle *handle = startSound(0x4000d + _64);
    if (handle && (*handle)) {
        u8 camNo = Main::getAudio()->getCamera()->_8;
        (*handle)->getAuxiliary().moveVolume(GA_MULTI_VOLUME[camNo - 1], 0);
    }
}

}
