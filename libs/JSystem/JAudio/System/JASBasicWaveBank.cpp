#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASMutex.h"
#include "JSystem/JAudio/System/JASWaveBank.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/os/OSMutex.h"

JASBasicWaveBank::JASBasicWaveBank() {
    mWaveTable = NULL;
    mHandleCount = 0;
    mWaveGroupArray = NULL;
    mGroupCount = 0;
    OSInitMutex(&mMutex);
}

JASBasicWaveBank::~JASBasicWaveBank() {
    delete[] mWaveTable;
    for (u32 i = 0; i < mGroupCount; i++) {
        delete mWaveGroupArray[i];
    }
    delete[] mWaveGroupArray;
}

JASBasicWaveBank::TWaveGroup *JASBasicWaveBank::getWaveGroup(u32 groupIdx) {
    if (groupIdx >= mGroupCount)
        return NULL;
    return mWaveGroupArray[groupIdx];
    
}

void JASBasicWaveBank::setGroupCount(u32 count, JKRHeap *pHeap) {
    for (int i = 0; i < mGroupCount; i++) {
        delete mWaveGroupArray[i];
    }
    delete[] mWaveGroupArray;
    mGroupCount         = count;
    mWaveGroupArray     = new (pHeap, 0) TWaveGroup*[count];
#line 52
    JUT_ASSERT(mWaveGroupArray != 0);
    for (int i = 0; i < mGroupCount; i++) {
        mWaveGroupArray[i] = new (pHeap, 0) TWaveGroup(this);
    }
}

void JASBasicWaveBank::setWaveTableSize(u32 count, JKRHeap *pHeap) {
    delete[] mWaveTable;
    mWaveTable = new(pHeap, 0) TWaveInfo*[count];
#line 64
    JUT_ASSERT(mWaveTable != 0);
    JASCalc::bzero(mWaveTable, count * sizeof(TWaveHandle*));
    mHandleCount = count;
}

void JASBasicWaveBank::incWaveTable(const JASBasicWaveBank::TWaveGroup *wave) {
    JASMutexLock lock(&mMutex);
    for (int i = 0; i < wave->mWaveCount; i++) {
        TWaveInfo **table   = &mWaveTable[wave->getWaveID(i)];
        TWaveInfo *currInfo = &wave->mCtrlWaveArray[i];
        currInfo->mPrev     = nullptr;
        currInfo->mNext     = (*table);
        if ((*table) != nullptr) {
            (*table)->mPrev = currInfo;
        }
        (*table) = currInfo;
    }
}

void JASBasicWaveBank::decWaveTable(const JASBasicWaveBank::TWaveGroup *wave) {
    JASMutexLock lock(&mMutex);
    for (int i = 0; i < wave->mWaveCount; i++) {
        u32 id              = wave->getWaveID(i);
        TWaveInfo *info     = mWaveTable[id];
        TWaveInfo *currInfo = &wave->mCtrlWaveArray[i];
        for (; info; info = info->mNext) {
            if (info != currInfo) {
                continue;
            }

            if (!info->mPrev) {
                mWaveTable[id] = info->mNext;
            } else {
                info->mPrev->mNext = info->mNext;
            }

            if (info->mNext) {
                info->mNext->mPrev = info->mPrev;
            }
            break;
        }
    }
}

JASWaveHandle *JASBasicWaveBank::getWaveHandle(u32 handleIdx) const {
    if (handleIdx >= mHandleCount) {
        return NULL;
    }
    if (!mWaveTable[handleIdx]) {
        return NULL;
    }
    return &mWaveTable[handleIdx]->mHandle;
}

JASBasicWaveBank::TWaveGroup::TWaveGroup(JASBasicWaveBank *bank) {
    mBank = bank;
    mCtrlWaveArray = NULL;
    mWaveCount = 0;
}

JASBasicWaveBank::TWaveGroup::~TWaveGroup() {
    delete[] mCtrlWaveArray;
}


void JASBasicWaveBank::TWaveGroup::setWaveCount(u32 count, JKRHeap *pHeap) {
    delete[] mCtrlWaveArray;
    mWaveCount = count;
    mCtrlWaveArray = new(pHeap, 0) TWaveInfo[count];
#line 172
    JUT_ASSERT(mCtrlWaveArray != 0);

    for (int i = 0; i < count; i++) {
        mCtrlWaveArray[i].mHandle.mHeap     = &mHeap;
        mCtrlWaveArray[i].mHandle.mInfo._24 = &_48;
    }
}


void JASBasicWaveBank::TWaveGroup::setWaveInfo(int index, u32 waveID, const JASWaveInfo &info) {
#line 182
    JUT_ASSERT(index < mWaveCount);
    JUT_ASSERT(index >= 0);
    mCtrlWaveArray[index].mHandle.mWaveID   = waveID;
    mCtrlWaveArray[index].mHandle.mInfo     = info;
    mCtrlWaveArray[index].mHandle.mInfo._24 = &_48;
}

void JASBasicWaveBank::TWaveGroup::onLoadDone() {
#line 193
    JUT_ASSERT(mBank);
    mBank->incWaveTable(this);
}

void JASBasicWaveBank::TWaveGroup::onEraseDone() {
#line 199
    JUT_ASSERT(mBank);
    mBank->decWaveTable(this);
}

u32 JASBasicWaveBank::TWaveGroup::getWaveID(int index) const {
#line 205
    JUT_ASSERT(index < mWaveCount);
    JUT_ASSERT(index >= 0);
    return mCtrlWaveArray[index].mHandle.mWaveID;
}

int JASBasicWaveBank::TWaveHandle::getWavePtr() const {
#line 221
    JUT_ASSERT(mHeap);
    void *base = mHeap->getBase();
    if (base == 0) {
        return 0;
    }
    return (int)base + mInfo._08;
}
