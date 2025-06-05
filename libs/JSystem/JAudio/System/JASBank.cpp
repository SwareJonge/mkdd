#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JAudio/System/JASDriver.h"

#ifdef MATCHING
template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;
#endif

JASChannel *JASBank::noteOn(const JASBank *pBnk, u32 instId, u8 param_2, u8 vel, u16 prio,
                         JASChannel::Callback cb,
                         void *param_6) {
    if (instId > 0xef) {
        return noteOnOsc(instId - 0xf0, param_2, vel, prio, cb, param_6);
    }
    if (!pBnk) {
        return NULL;
    }
    JASInst *inst = pBnk->getInst(instId);
    if (!inst) {
        return NULL;
    }
    JASInstParam instParam;
    if (!inst->getParam(param_2, vel, &instParam)) {
        return NULL;
    }
    JASWaveBank *waveBank = pBnk->getWaveBank();
    if (!waveBank) {
        return NULL;
    }
    JASWaveHandle *waveHandle = waveBank->getWaveHandle(instParam._18);
    if (!waveHandle) {
        return NULL;
    }
    const JASWaveInfo *waveInfo = waveHandle->getWaveInfo();
    if (!waveInfo) {
        return NULL;
    }
    int wavePtr = waveHandle->getWavePtr();
    if (!wavePtr) {
        return NULL;
    }
    
    JASChannel *channel = new JASChannel(cb, param_6);
    if (!channel) {
        return NULL;
    }

    channel->setPriority(prio);
    channel->mWaveData.mInfo = *waveInfo;
    channel->mWaveData.mPtr = wavePtr;
    channel->mWaveData._0 = instParam._14;
    channel->setBankDisposeID(pBnk);
    channel->setInitPitch(instParam.mPitch * (waveInfo->_04 / JASDriver::getDacRate()));
    
    if (instParam._24 == 0) {
        channel->setKey(param_2 - waveInfo->_01);
    }

    channel->setInitVolume(instParam.mVolume);
    channel->setVelocity(vel);
    channel->setInitPan(instParam.mPan);
    channel->setInitFxmix(instParam.mFxMix);
    channel->setInitDolby(instParam.mDolby);

    for (u32 i = 0; i < instParam._20; i++) {
        channel->setOscInit(i, instParam._1c[i]);
    }

    channel->setDirectRelease(instParam._26);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}
JASChannel *JASBank::noteOnOsc(u32 wavePtr, u8 p2, u8 p3, u16 p4, JASChannel::Callback cb, void *ptr) {
    static JASOscillator::Point const OSC_RELEASE_TABLE[2] = {
        {0x0001, 0x000A, 0x0000},
        {0x000F, 0x0000, 0x0000},
    };
    static const JASOscillator::Data OSC_ENV = {0, 1.0f, NULL, OSC_RELEASE_TABLE, 1.0f, 0.0f};
    JASChannel *channel = new JASChannel(cb, ptr);
    if (!channel) {
        return NULL;
    }
    channel->setPriority(p4);
    channel->mWaveData.mPtr = wavePtr;
    channel->mWaveData._0 = 2;
    channel->setInitPitch(16736.016f / JASDriver::getDacRate());
    channel->setKey(p2 - channel->mWaveData.mInfo._01);
    channel->setVelocity(p3);
    channel->setOscInit(0, &OSC_ENV);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}
