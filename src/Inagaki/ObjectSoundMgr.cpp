#include "Inagaki/GameAudioCommon.h"
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Course/CrsArea.h"

#include "JSystem/JAudio/JASFakeMatch4.h"

namespace GameAudio {

ObjectSoundMgr::ObjectSoundMgr(Vec *pos, JKRHeap *heap) : GameAudio::SoundWithEchoMgr<ObjectSoundMgr>(pos, heap, 4) {
    _68 = 0;
    mObjArea = new CrsArea();
    _6c = 0.0f;
}

ObjectSoundMgr::~ObjectSoundMgr() {}

void ObjectSoundMgr::loop() {
#line 44
    JUT_ASSERT_MSG(mSoundPos, "ObjectSoundMgr::loop　disable中です！！！！！\n");
    SoundMgr::loop();
    SoundWithEchoMgr::checkEcho();
    _68 = 0;
}

void ObjectSoundMgr::frameWork() {
    if (mKillSw)
        return;
    _68 = 1;
}

void ObjectSoundMgr::setSe(u32 seID) {
    if (mKillSw)
        return;

    JAISoundHandle *handle = startSoundCustom(Common::changeRandomId(seID, 0), 0);
    setInitialEcho(handle);
}

void ObjectSoundMgr::setSeWithVelocityVP(u32 soundID, f32 p2, f32 p3) {
    if (mKillSw)
        return;

    JAISoundHandle *handle = startSoundCustom(soundID, 0);
    if (handle && (*handle)) {
        if (p2 > 1.0f) {
            p2 = 1.0f;
        }
        else if (p2 < 0.0f) {
            p2 = 0.0f;
        }
        (*handle)->getAuxiliary().moveVolume((1.0f - p3) * p2 + p3, 0);
        (*handle)->getAuxiliary().movePitch((1.0f - p3) * p2 + p3, 0);
    }
    setInitialEcho(handle);
}

void ObjectSoundMgr::setSeWithVelocityV(u32 soundID, f32 p2, f32 p3) {
    if (mKillSw)
        return;

    if (p2 < 0.1f)
        return;

    JAISoundHandle *handle = startSoundCustom(soundID, 0);
    if (handle && (*handle)) {
        if (p2 > 1.0f) {
            p2 = 1.0f;
        }

        (*handle)->getAuxiliary().moveVolume((1.0f - p3) * p2 + p3, 0);
    }
    setInitialEcho(handle);
}

void ObjectSoundMgr::setSeWithVelocityP(u32 soundID, f32 p2) {
    if (mKillSw)
        return;

    if (p2 < 0.01f)
        return;

    JAISoundHandle *handle = startSoundCustom(soundID, 0);
    if (handle && (*handle)) {
        if (p2 > 1.0f) {
            p2 = 1.0f;
        }

        (*handle)->getAuxiliary().movePitch((p2 + 0.5f), 0);
    }
    setInitialEcho(handle);
}
}
