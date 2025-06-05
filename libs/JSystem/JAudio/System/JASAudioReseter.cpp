#include "JSystem/JAudio/System/JASAudioThread.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JAudio/System/JASAudioReseter.h"

JASAudioReseter::JASAudioReseter() {
    _0 = 0;
    mDoneFlag = true;
    mThreadStopFlag = false;
    mDSPLevel = JASDriver::getDSPLevel();
}

JASAudioReseter::~JASAudioReseter() {
    JUT_ASSERT(mDoneFlag);
}

bool JASAudioReseter::start(u32 param_0, bool stop) {
    if (mDoneFlag == false) {
        return false;
    }
    mThreadStopFlag = stop;

    JAS_CS_START
    if (!JASDriver::registerDspSyncCallback(callback, this)) {
#line 40
        JUT_WARNING_F2("%s", "Failed JASAudioReseter start\n");
        return false;
    } else {
        mDSPLevel = JASDriver::getDSPLevel();
        _0 = param_0;
        mDoneFlag = false;
        return true;
    }
    JAS_CS_END
}

void JASAudioReseter::resume() {
#line 55
    JUT_ASSERT(mDoneFlag);
    JUT_ASSERT(! mThreadStopFlag);
    JASDriver::setDSPLevel(mDSPLevel);
}

int JASAudioReseter::checkDone() const {
    return mDoneFlag;
}

int JASAudioReseter::calc() {
    if(_0 == 0) {
        for(size_t i = 0; i < 64; i++) {
            JASDSPChannel *handle = JASDSPChannel::getHandle(i);
            if ((handle->getStatus())==0) {
                handle->drop();
            }
        }
        if(mThreadStopFlag) {
            JASAudioThread *pAudioThread = JASGlobalInstance<JASAudioThread>::getInstance();
#line 78
            JUT_ASSERT(pAudioThread);
            pAudioThread->stop();
        }
        mDoneFlag = 1;
        return -1;
    }
    _0--;
    u32 unk = _0;
    f32 dspLevel = JASDriver::getDSPLevel();
    JASDriver::setDSPLevel(((float)unk*dspLevel)/(float)(unk+1));
    return 0;
}

s32 JASAudioReseter::callback(void *p) {
    return ((JASAudioReseter*)p)->calc();
}
