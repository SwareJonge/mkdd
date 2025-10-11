#include "Inagaki/GameAudioCamera.h"
#include "Inagaki/GameAudioAudience.h"
#include "Inagaki/GameSoundMgr.h"
#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/JAUAudience.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Course/CrsArea.h"

#include "JSystem/JAudio/JASFakeMatch.h"

#ifdef MATCHING // TODO

template <>
JASMemPool< JAUDopplerAudible<4> > JASPoolAllocObject< JAUDopplerAudible<4> >::memPool_;
template <>
JASMemPool<JAISe> JASPoolAllocObject<JAISe>::memPool_;

#endif

namespace GameAudio {

CameraMgr::CameraMgr(u32 p1, CustomAudience<4> *audience) {
    _4 = p1;
    mSceneMax = 1;
    mCamInfo = new (JASDram, 0) CameraInfo[_4];
    mAudience = audience;

    for (int i = 0; i < 4; i++) {
        mAudience->init(i);
    }
}

void CameraMgr::setCameraInfo(u32 cameraNo, Vec v, Mtx m,  CrsArea *pArea) {
    // The camera number is invalid
#line 34
    JUT_ASSERT_MSG(cameraNo < _4, "GameAudioCamera : カメラナンバーが不正です。\n");
    u8 areaKind = 0;
    if (!pArea->isKind2()) {
        CrsArea area;
        area.search(2, v);
        pArea = &area;
    }
    
    mCamInfo[cameraNo].setCam(v, m);

    if (cameraNo < mSceneMax) {
        mAudience->setAudienceMtx(m, cameraNo, false);
        if (ShoreSoundMgr::smStart) {
            ShoreSoundMgr::getAudience()->setAudienceMtx(m, cameraNo, false);
        }
        if (pArea) {
            mAudience->setEchoRate(cameraNo, pArea->getEchoRate() * 0.3f);
            mCamInfo[cameraNo].mEchoRate = pArea->getEchoRate();
        }
    }
    
}

void CameraMgr::setSceneMax(u32 numPlayers) {
    mSceneMax = numPlayers;
    JAISeMgr::getInstance()->stopAudibleSoundsSync();
    mAudience->setMaxChannels(numPlayers);
    ShoreSoundMgr::getAudience()->setMaxChannels(numPlayers);
}

}
