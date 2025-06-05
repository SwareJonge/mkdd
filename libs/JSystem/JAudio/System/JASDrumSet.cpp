#include "JSystem/JAudio/System/JASDrumSet.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"


JASDrumSet::JASDrumSet() {
    JASCalc::bzero(mPercArray, sizeof(mPercArray));
}

JASDrumSet::~JASDrumSet() {
    for (u32 i = 0; i < sPercCount; i++) {
        delete mPercArray[i];
    }
}

bool JASDrumSet::getParam(int key, int veloKey, JASInstParam *param) const {
#line 29
    JUT_ASSERT(key >= 0);

    if (key >= sPercCount) {
        JUT_WARNING_F2("JASDrumSet: key %d >= sPercCount %d\n", key, sPercCount);
        return false;
    }

    TPerc *perc = mPercArray[key];
    if (perc == NULL) {
        return false;
    }

    param->_14 = 0;
    param->_24 = 1;
    param->mVolume = perc->mVolume;
    param->mPitch = perc->mPitch;
    param->mPan = perc->mPan;
    param->_26 = perc->mRelease;
    
    static JASOscillator::Data osc;

    osc.mTarget = 0;
    osc._04 = 1.0f;
    osc.mTable = NULL;
    osc.rel_table = NULL;
    osc.mScale = 1.0f;
    osc._14 = 0.0f;

    static JASOscillator::Data *oscp = &osc;

    param->_1c = &oscp;
    param->_20 = 1;

    for (int i = 0; i < 4; i++) {
        JASInstEffect *effect = perc->mEffects[i];
        if (effect)
            effect->effect(key, veloKey, param);
    }

    for (u32 i = 0; i <  perc->mVelomapCount; i++) {
        JASVelo *velo = &perc->mVelomap[i];
        if (veloKey <= velo->_0) {
            param->mVolume *= velo->_8;
            param->mPitch *= velo->_c;
            param->_18 = velo->_4;
            return true;
        }
    }

    return false;
}

void JASDrumSet::setPerc(int index, JASDrumSet::TPerc *pperc) {
#line 87
    JUT_ASSERT(index < sPercCount);
    JUT_ASSERT(index >= 0);

    JUT_ASSERT(mPercArray[index] == 0);
    mPercArray[index] = pperc;
}

JASDrumSet::TPerc::TPerc() {
    mVolume = 1.0f;
    mPitch = 1.0f;
    mPan = 0.5f;
    mRelease = 1000;
    mVelomapCount = 0;
    mVelomap = NULL;
    JASCalc::bzero(mEffects, sizeof(mEffects));
}

JASDrumSet::TPerc::~TPerc() {
    for (int i = 0; i < EFFECT_MAX; i++) {
        delete mEffects[i];
    }
    delete[] mVelomap;
}

void JASDrumSet::TPerc::setVeloRegionCount(u32 count, JKRHeap *pHeap) {
    delete[] mVelomap;
    mVelomap = new (pHeap, 0) JASVelo[count];
#line 138
    JUT_ASSERT(mVelomap != 0);
    mVelomapCount = count;
}

JASVelo *JASDrumSet::TPerc::getVeloRegion(int index) {
#line 144
    JUT_ASSERT(index < mVelomapCount);
    JUT_ASSERT(index >= 0);
    return &mVelomap[index];
}

void JASDrumSet::TPerc::setEffect(int index, JASInstEffect *effect) {
#line 160
    JUT_ASSERT(index < EFFECT_MAX);
    JUT_ASSERT(index >= 0);
    mEffects[index] = effect;
}


void JASDrumSet::TPerc::setRelease(u32 release) {
#line 178
    JUT_ASSERT(release < 0x10000);
    mRelease = release;
}

#ifdef MATCHING
template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;
#endif
