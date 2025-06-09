#include "Inagaki/GameAudioAnimator.h"
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "types.h"

#include "JSystem/JAudio/JASFakeMatch4.h"

//JASGlobalInstance<JAWSpeAna>::sInstance // UNUSED
//JASGlobalInstance<JAWSeqEdit>::sInstance // UNUSED

namespace GameAudio {

CharacterSoundMgr::CharacterSoundMgr(Vec *pos, JKRHeap *heap, u8 p3, ECharID charID) : SoundMgr<CharacterSoundMgr>(pos, heap, 8) {
    mAnimator = new (heap, 0) CustomAnimator(this);
    mStarter = new (heap, 0) CustomAnimationSoundStarter();
    _C = 0x80; // struct size?
    _64 = p3;
    mCharID = charID;
    mKartSound = nullptr;
    getKartSound();
    init();
}

CharacterSoundMgr::~CharacterSoundMgr() {}

void CharacterSoundMgr::init() {
    _78 = 0;
    _6c = 0;
    _70 = 1.0f;
    _68 = 0x168;
    _65 = 0xff;
    mAnimator->startAnimation(nullptr, false, 0.0f, 0.0f);
}

void CharacterSoundMgr::loop() {
    JUT_ASSERT_MSG(mSoundPos, "CharacterSoundMgr::loop　disable中です！！！！！\n");
    if (_48)
        return;

    JAUDopplerSoundObject::process();
    checkEcho();
    if (_68 < 0x168) {
        _68++;
    }
}

void CharacterSoundMgr::frameWork(f32 p1, f32) {
    KartSoundMgr *kartSound = getKartSound();

    if (_48 != 0 || kartSound->_66 == 2) {
        return;
    }
    mAnimator->updateAnimation(p1, *mSoundPos, mStarter);
}

void CharacterSoundMgr::setVoice(u8) {

}

void CharacterSoundMgr::setSe(u32) {
    
}

bool CharacterSoundMgr::isSpeak() {
    if ((*this)[0]) {
        return true;
    }
    return false;
}

void CharacterSoundMgr::setVoicePitch(bool) {}

KartSoundMgr *CharacterSoundMgr::getKartSound() {
    if (mKartSound && KartSoundMgr::smStart) {
        return mKartSound;
    }
    KartSoundMgr *kartSound = KartSoundMgr::smStart;
    while (kartSound) {
        if (_64 == kartSound->_61) {
            mKartSound = kartSound;
            return mKartSound;
        }
        kartSound = kartSound->mNext;
    }
    return nullptr;
}

void CharacterSoundMgr::checkEcho() {}

CustomAnimationSoundStarter::CustomAnimationSoundStarter() : JAISoundStarter(false) {
    _4 = 4;
}

bool CustomAnimationSoundStarter::startSound(JAISoundID, JAISoundHandle *, const JGeometry::TVec3f *) {}

CustomAnimator::CustomAnimator(JAISoundHandles *handles) : JAUSoundAnimator(handles) {}

JAISoundHandle *CustomAnimator::getFreeHandle(const JAUSoundAnimationSound *) {}

void CustomAnimator::modifySoundParams(JASSoundParams *, const JAUSoundAnimationSound *, f32) {}


}
