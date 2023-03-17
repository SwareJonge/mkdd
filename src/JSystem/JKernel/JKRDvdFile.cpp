#include <JSystem/JKernel/JKRDvdFile.h>
#include <JSystem/JUtility/JUTDbg.h>

JSUList<JKRDvdFile> JKRDvdFile::sDvdList;

JKRDvdFile::JKRDvdFile() : JKRFile(), mLink(this) {
    initiate();
}

JKRDvdFile::JKRDvdFile(s32 entrynum) : JKRFile(), mLink(this) {
    initiate();

    mFileOpen = open(entrynum);
    if(mFileOpen) // very useful
        return;
}

JKRDvdFile::~JKRDvdFile() {
    close();
}

void JKRDvdFile::initiate() {
    mDvdFileInfo.file = this;
    OSInitMutex(&mDvdMutex);
    OSInitMutex(&mAramMutex);
    OSInitMessageQueue(&mMessageQueue_C0, &mMessage_E0, OS_MESSAGE_BLOCK);
    OSInitMessageQueue(&mMessageQueue, &mMessage, OS_MESSAGE_BLOCK);
    mThread = nullptr;
    mCommandThread = nullptr;
    _58 = 0;
}

bool JKRDvdFile::open(const char * fileName) {
    if(!mFileOpen) {
        mFileOpen = DVDOpen(fileName, &mDvdFileInfo);
        if(mFileOpen) {
            sDvdList.append(&mLink);
            DVDGetCommandBlockStatus(&mDvdFileInfo);
        }
    }
    return mFileOpen;
}

bool JKRDvdFile::open(s32 entryNum) {
    if(!mFileOpen) {
        mFileOpen = DVDFastOpen(entryNum, &mDvdFileInfo);
        if(mFileOpen) {
            sDvdList.append(&mLink);
            DVDGetCommandBlockStatus(&mDvdFileInfo);
        }
    }
    return mFileOpen;
}

void JKRDvdFile::close() {
    if(mFileOpen) {
        if(DVDClose(&mDvdFileInfo)) {
            mFileOpen = false;
            sDvdList.remove(&mLink);
        }
        else {
            JUT_PANIC(213, "cannot close DVD file\n");
        }
    }
}

s32 JKRDvdFile::readData(void * addr,s32 length, s32 offset) {
    JUT_ASSERT(238, ( length & 0x1f ) == 0);
    OSLockMutex(&mDvdMutex);
    s32 retAddr;
    if(mThread != nullptr) {
        OSUnlockMutex(&mDvdMutex);
        return -1;
    }
    else {
        mThread = OSGetCurrentThread();
        retAddr = -1;
        if (DVDReadAsyncPrio(&mDvdFileInfo, addr, length, offset, doneProcess, 2))
        {
            retAddr = (s32)sync();
        }
        mThread = nullptr;
        OSUnlockMutex(&mDvdMutex);
    }
    return retAddr;
}

s32 JKRDvdFile::writeData(const void * addr,s32 length, s32 offset) {
    JUT_ASSERT(344, ( length & 0x1f ) == 0);
    return -1;
}

OSMessage JKRDvdFile::sync() {
    OSMessage msg;
    OSLockMutex(&mDvdMutex);
    OSReceiveMessage(&mMessageQueue_C0, &msg, OS_MESSAGE_BLOCK);
    mThread = nullptr;
    OSUnlockMutex(&mDvdMutex);
    return msg;
}

BOOL JKRDvdFile::doneProcess(s32 msg, DVDFileInfo * fInfo) {
    JKRDvdFile * dvdFile = reinterpret_cast<JKRDvdFile *>(fInfo->file); // TODO: is this part of DVDFileInfo or JKRDvdFile?
    return OSSendMessage(&dvdFile->mMessageQueue_C0, (void*)msg, OS_MESSAGE_NOBLOCK);
}

