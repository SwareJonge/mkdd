#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JUtility/JUTAssert.h"

JASSimpleWaveBank::JASSimpleWaveBank() {
    mWaveTable = NULL;
    mWaveCount = 0;
}

JASSimpleWaveBank::~JASSimpleWaveBank() {
    delete[] mWaveTable;
}

void JASSimpleWaveBank::setWaveTableSize(u32 count, JKRHeap *heap) {
    delete[] mWaveTable;
    mWaveTable = new (heap, 0) TWaveHandle[count];
#line 29
    JUT_ASSERT(mWaveTable != 0);
    mWaveCount = count;
}

JASWaveHandle *JASSimpleWaveBank::getWaveHandle(u32 waveID) const {
    if (waveID >= mWaveCount)
        return NULL;
    return &mWaveTable[waveID];
}

void JASSimpleWaveBank::setWaveInfo(u32 waveID, const JASWaveInfo &waveInfo) {
    mWaveTable[waveID].mInfo = waveInfo;
    mWaveTable[waveID].mInfo._24 = &_48;
    mWaveTable[waveID].mHeap = &mHeap;
    
}

JASWaveArc *JASSimpleWaveBank::getWaveArc(u32 p1) {
    if (p1 != 0)
        return NULL;

    return this;
}

int JASSimpleWaveBank::TWaveHandle::getWavePtr() const {
    void *base = mHeap->getBase();
    if (base == 0) {
        return 0;
    }
    return (int)base + mInfo._08;
}
