#include "Inagaki/GameAudioCommon.h"
#include "Inagaki/GameSoundMgr.h"
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
    if (_48)
        return;
    _68 = 1;
}

void ObjectSoundMgr::setSe(u32 seID) {
    if (_48)
        return;

    JAISoundHandle *handle = startSoundCustom(Common::changeRandomId(seID, 0), 0);
    setInitialEcho(handle);
}

void ObjectSoundMgr::setSeWithVelocityVP(u32, f32, f32) {

}

void ObjectSoundMgr::setSeWithVelocityV(u32, f32, f32) {}

void ObjectSoundMgr::setSeWithVelocityP(u32, f32) {}


}
