#include "JSystem/JAudio/System/JASResArcLoader.h"
#include "JSystem/JAudio/System/JASDvd.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/os/OSMessage.h"

size_t JASResArcLoader::getResSize(const JKRArchive *arc, u16 id) {
    JKRArchive::SDIFileEntry *entry = arc->findIdResource(id);
    if (entry == NULL) {
        return 0;
    }
    return entry->mSize;
}

size_t JASResArcLoader::getResMaxSize(const JKRArchive *arc) {
    u32 fileCount = arc->countFile();
    size_t resSize = 0;
    for (u32 i = 0; i < fileCount; i++) {
        JKRArchive::SDIFileEntry *entry = arc->findIdxResource(i);
        if (entry && resSize < entry->mSize) {
            resSize = entry->mSize;
        }
    }
    return resSize;
}

static void JASResArcLoader::loadResourceCallback(void *arg) {
    TLoadResInfo *pResInfo = (TLoadResInfo*)arg;
    u32 size = pResInfo->mArchive->readResource(pResInfo->mBuffer, pResInfo->mBufferSize, pResInfo->mID);
    if (pResInfo->mCallback) {
        pResInfo->mCallback(size, pResInfo->mCallbackArg);
    }
    if (size == 0) {
        if (pResInfo->mQueue) {
            OSSendMessage(pResInfo->mQueue, (OSMessage)0xffffffff, OS_MESSAGE_BLOCK);
        }
    }
    else if (pResInfo->mQueue) {
        OSSendMessage(pResInfo->mQueue, NULL, OS_MESSAGE_BLOCK);
    }
}

int JASResArcLoader::loadResourceAsync(JKRArchive *arc, u16 id, u8 *buf, u32 bufSize, LoadCallback lCb, u32 lCbArg) {
    TLoadResInfo resInfo(arc, id, buf, bufSize);
    resInfo.mCallback = lCb;
    resInfo.mCallbackArg = lCbArg;
    int res = JASDvd::getThreadPointer()->sendCmdMsg(loadResourceCallback, &resInfo, sizeof(resInfo));
    if (res == 0) {
#line 160
        JUT_WARNING_F2("%s", "sendCmdMsg loadResourceCallback Failed");
    }
    return res;
}
