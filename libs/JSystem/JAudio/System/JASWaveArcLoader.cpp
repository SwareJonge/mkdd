#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JAudio/System/JASDvd.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASKernel.h"
#include "JSystem/JAudio/System/JASMutex.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dvd/dvd.h"
#include "dolphin/dvd/dvdfs.h"
#include "dolphin/os/OSMutex.h"
#include <cstring>

namespace JASWaveArcLoader {
    JASHeap *sAramHeap;
static char sCurrentDir[DIR_MAX] = "/AudioRes/Waves/";

JASHeap *getRootHeap() {
    return sAramHeap ? sAramHeap : JASKernel::getAramHeap();
}

void setRootHeap(JASHeap *heap) {
    sAramHeap = heap;
}

void setCurrentDir(const char *dir) {
#line 40
    JUT_ASSERT(std::strlen(dir) < DIR_MAX-1);
    std::strcpy(sCurrentDir, dir);
    size_t len = std::strlen(sCurrentDir);
    if (sCurrentDir[len-1] != '/') {
#line 45
        JUT_ASSERT(len+1 < DIR_MAX);
        sCurrentDir[len] = '/';
        sCurrentDir[len + 1] = 0;
    }
}

char *getCurrentDir() {
    return sCurrentDir;
}

}

JASWaveArc::JASWaveArc() : mHeap(this) {
    _48 = 0;
    _4c = 0;
    mEntryNum = -1;
    mFileLength = 0;
    _58 = 0;
    _5a = 0;
    OSInitMutex(&mMutex);
}

JASWaveArc::~JASWaveArc() {}

void JASWaveArc::activate() {
    // UNUSED
}

bool JASWaveArc::loadSetup(u32 p1) {
    JASMutexLock lock(&mMutex);
    if (_58 != p1) 
        return false;

    if (_4c != 1)
        return false;

    _48 = 1;
    _4c = 2;
    return true;
}

bool JASWaveArc::eraseSetup() {
    JASMutexLock lock(&mMutex);
    if (_4c == 0) 
        return false;

    if (_4c == 1) {
        _4c = 0;
        return false;
    }

    _48 = 0;
    _4c = 0;
    return true;
}

void JASWaveArc::loadToAramCallback(void *arg) {
    loadToAramCallbackParams *ripperArgs = (loadToAramCallbackParams*)arg;
    JASWaveArc *arc = ripperArgs->mWavArc;
    if (!JKRDvdAramRipper::loadToAram(ripperArgs->mEntryNum, ripperArgs->mBase, EXPAND_SWITCH_DEFAULT, 0, 0, NULL)) {
#line 129
        JUT_WARNING_F2("%s", "loadToAram Failed");
        return;
    }

    arc->_5a--;
    if (arc->loadSetup(ripperArgs->_c)) {
        arc->onLoadDone();
    }
}

void JASWaveArc::execLoad() {
    // UNUSED
}

bool JASWaveArc::sendLoadCmd() {
    JASMutexLock lock(&mMutex);
    _48 = 0;
    _4c = 1;
    loadToAramCallbackParams params;
    params.mWavArc = this;
    params.mEntryNum = mEntryNum;
    params.mBase =(u32) mHeap.getBase();
    params._c = ++_58;
    _5a++;

    if (!JASDvd::getThreadPointer()->sendCmdMsg(loadToAramCallback, &params, sizeof(params))) {
#line 193
        JUT_WARNING_F2("%s", "sendCmdMsg loadToAramCallback Failed");
        mHeap.free();
        return false;
    }
    return true;
}

bool JASWaveArc::load(JASHeap *heap) {
    if (mEntryNum < 0)
        return false;

    JASMutexLock lock(&mMutex);
    if (_4c != 0)
        return false;

    if (!heap)
        heap = JASWaveArcLoader::getRootHeap();

    if (!mHeap.alloc(heap, mFileLength))
        return false;
    
    return sendLoadCmd();

}

void JASWaveArc::loadBlock(JASHeap *heap) {
    // UNUSED
}

void JASWaveArc::erase() {
    mHeap.free();
}

void JASWaveArc::onDispose() {
    if (eraseSetup()) {
        onEraseDone();
    }
}

void JASWaveArc::setEntryNum(s32 entryNum) {
    DVDFileInfo info;
    if (entryNum >= 0 && DVDFastOpen(entryNum, &info)) {
        mFileLength = info.length;
        DVDClose(&info);
        mEntryNum = entryNum;
    }
}

void JASWaveArc::setFileName(const char *fileName) {
    const char *dir = JASWaveArcLoader::getCurrentDir();
    size_t pathLen = std::strlen(dir);
    pathLen += std::strlen(fileName);

    char *path = new (JASKernel::getSystemHeap(), -4) char[pathLen +1];
#line 322
    JUT_ASSERT(path);
    std::strcpy(path, dir);
    std::strcat(path, fileName);
    path[pathLen] = 0;
    s32 entryNum = DVDConvertPathToEntrynum(path);
    delete[] path;
    if (entryNum >= 0) {
        setEntryNum(entryNum);
    }

}

bool JASWaveArc::isLoading() const {
    return _5a != 0;
}
