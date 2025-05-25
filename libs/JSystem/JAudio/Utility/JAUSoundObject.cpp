#include "JSystem/JAudio/JAUSoundObject.h"

#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUSoundObject::JAUSoundObject(JGeometry::TVec3f *pos, u8 numHandles, JKRHeap *heap) : JAISoundHandles(new (heap, 0) JAISoundHandle[numHandles], numHandles) {
#line 22
    JUT_ASSERT(handles_);
    _C = 0;
    _10 = numHandles;
    mSoundPos = pos;
}

JAUSoundObject::~JAUSoundObject() {
    dispose();
    delete[] handles_;
}

void JAUSoundObject::dispose() {
    for (u32 i = 0; i < numHandles_; i++) {
        if (handles_[i]) {
            if (stopOK(handles_[i])) {
                handles_[i]->stop();
            }
            else {
                handles_[i].releaseSound();
            }
        }
    }
}

bool JAUSoundObject::stopOK(JAISoundHandle &) {
    return true;
}

void JAUSoundObject::process() {
    setPos(*mSoundPos);
}

JAISoundHandle *JAUSoundObject::startSound(JAISoundID soundID) {
    JAISoundStarter *starter = JAISoundStarter::getInstance();
    JAISoundHandle *handle = getHandleSoundID(soundID);
    if (handle == NULL) {
        handle = getFreeHandleNotReserved();
    }
    if (handle == NULL) {
        handle = getLowPrioSound(soundID);
    }

    if (handle) {
        starter->startSound(soundID, handle,mSoundPos);
    }
    return handle;    
}

JAISoundHandle *JAUSoundObject::startLevelSound(JAISoundID soundID) {
    JAISeMgr *seMgr = JAISeMgr::getInstance();
    JAISoundHandle *handle = getHandleSoundID(soundID);

    if (handle) {
#line 91
        JUT_ASSERT_F((*handle), "%s", "handle exist, but (*handle) is not exist!!\n");
        (*handle)->updateLifeTime(1);
    }
    else {
        handle = getFreeHandleNotReserved();
        if (!handle) {
            handle = getLowPrioSound(soundID);
        }
        if (handle) {
            seMgr->startSound(soundID, handle, mSoundPos);
            if (*handle) {
                (*handle)->setLifeTime(1, false);
            }
        }
    }
    return handle;
}

JAISoundHandle *JAUSoundObject::getLowPrioSound(JAISoundID soundID) {
    JAISoundInfo *soundInfo = JAISoundInfo::getInstance();
    u32 curHighestPrio = 0xffff;
    JAISoundHandle *ret = NULL;
    for (s32 i = 0; i < numHandles_; i++) {
        if (_C & (1 << i))
            continue;

        JAISoundHandle &handle = (*this)[i]; // what the fuck
        u32 prio = soundInfo->getPriority(handle->soundID_); // getter?
        if (prio < curHighestPrio) {
            curHighestPrio = prio;
            ret = &handle;
        }
    }

    if (soundInfo->getPriority(soundID) >= curHighestPrio) {
        return ret;
    }
    return NULL;
}

JAISoundHandle *JAUSoundObject::getFreeHandleNotReserved() {
    for (s32 i = 0; i < numHandles_; i++) {
        if (_C & (1 << i))
            continue;

        JAISoundHandle &handle = (*this)[i]; // what the fuck
        if (!handle) {
            return &handle;
        }
    }
    return NULL;
}

JAUDopplerSoundObject::JAUDopplerSoundObject(JGeometry::TVec3f *pos, u8 p2, JKRHeap *heap) : JAUSoundObject(pos, p2, heap) {
    _18 = *mSoundPos;
    _24 = *mSoundPos;
}

JAUDopplerSoundObject::~JAUDopplerSoundObject() {}

void JAUDopplerSoundObject::process() {
    _18 = _24;
    setPos(*mSoundPos);
    _24 = *mSoundPos;
}

JAISoundHandle *JAUDopplerSoundObject::startSound(JAISoundID soundID) {
    JAISoundStarter *starter = JAISoundStarter::getInstance();
    JAISoundHandle *handle = getHandleSoundID(soundID);
    if (handle == NULL) {
        handle = getFreeHandleNotReserved();
    }
    if (handle == NULL) {
        handle = getLowPrioSound(soundID);
    }

    if (handle) {
        starter->startSound(soundID, handle, NULL);
        if (*handle) {
            handle->getSound()->newAudible(*mSoundPos, &_18, 0, NULL);
        }
    }

    return handle;
}

JAISoundHandle *JAUDopplerSoundObject::startLevelSound(JAISoundID soundID) {
    JAISeMgr *seMgr = JAISeMgr::getInstance();
    JAISoundHandle *handle = getHandleSoundID(soundID);
    if (handle) {
#line 209
        JUT_ASSERT_F((*handle), "%s", "handle exist, but (*handle) is not exist!!\n");
        (*handle)->updateLifeTime(1);
    }
    else {
        handle = getFreeHandleNotReserved();
        if (!handle) {
            handle = getLowPrioSound(soundID);
        }
        if (handle) {
            seMgr->startSound(soundID, handle, NULL);
            if (*handle) {
                (*handle)->newAudible(*mSoundPos, &_18, 0, NULL);
                (*handle)->setLifeTime(1, false);
            }
        }
    }
    return handle;
}
