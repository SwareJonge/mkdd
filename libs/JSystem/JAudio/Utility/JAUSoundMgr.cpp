#include "JSystem/JAudio/JAUSoundMgr.h"

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/JASFakeMatch5.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUSoundMgr::JAUSoundMgr() : JAISoundStarter(true), mSeMgr(true), mSeqMgr(true), mStreamMgr(true) {}

bool JAUSoundMgr::startSound(JAISoundID soundID, JAISoundHandle *handlePtr, const JGeometry::TVec3f *pos) {
    u32 soundType = JAISoundInfo::getInstance() ? JAISoundInfo::getInstance()->getSoundType(soundID) : soundID.mId.mBytes.mSectionId;
    switch (soundType) {
    case 0:
        return mSeMgr.startSound(soundID, handlePtr, pos);
    case 1:
        return mSeqMgr.startSound(soundID, handlePtr, pos);
    case 2:
        return mStreamMgr.startSound(soundID, handlePtr, pos);
    default:
#line 39
        JUT_WARNING_F2("%s", "Unknown Sound-Type\n");
        return false;
    }    
}

void JAUSoundMgr::calc() {
    mSeMgr.calc();
    mSeqMgr.calc();
    mStreamMgr.calc();
}

void JAUSoundMgr::mixOut() {
    mSeMgr.mixOut();
    mSeqMgr.mixOut();
    mStreamMgr.mixOut();
}
