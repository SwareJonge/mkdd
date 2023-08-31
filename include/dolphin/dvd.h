#ifndef _DOLPHIN_DVD_H
#define _DOLPHIN_DVD_H

#include <dolphin/types.h>
#include <dolphin/os.h>

struct DVDQueue
{
    struct DVDQueue *m_head; // _00
    struct DVDQueue *m_tail; // _04
};

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

    typedef struct DVDCommandBlock DVDCommandBlock;

    typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);

    typedef struct DVDCommandBlock
    {
        DVDCommandBlock *next;
        DVDCommandBlock *prev;
        u32 command;
        s32 state;
        u32 offset;
        u32 length;
        void *addr;
        u32 currTransferSize;
        u32 transferredSize;
        DVDDiskID *id;
        DVDCBCallback callback;
        void *userData;
    } DVDCommandBlock;

    typedef struct DVDFileInfo DVDFileInfo;

    typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);

    typedef struct DVDFileInfo
    {
        DVDCommandBlock cb;
        u32 startAddr;
        u32 length; 
        DVDCallback callback;
    } DVDFileInfo;

    typedef BOOL DVDDoneReadCallback(long, DVDFileInfo *);
    typedef void DVDState(OSDummyCommandBlock *);
    typedef void DVDLowCallback(u32);

    void DVDInit();

    // TODO: refactor
    BOOL DVDOpen(const char *, DVDFileInfo *);
    BOOL DVDFastOpen(long, DVDFileInfo *);
    BOOL DVDClose(DVDFileInfo *);
    int DVDCancel(DVDFileInfo *); // Definitely int; returns -1 on failure.
    void DVDResume();
    void DVDReset();
    int DVDReadPrio(DVDFileInfo *, void *readBuffer, s32 byteCount, u32 startOffset, s32 queueIndex);
    BOOL DVDReadAsyncPrio(DVDFileInfo *, void *, long, long, DVDDoneReadCallback *, int);
    BOOL DVDReadAbsAsyncPrio(DVDFileInfo *, void *readBuffer, long byteCount, u8 *startAddress,
                             DVDDoneReadCallback *doneReadCallback, int queueIndex);

#define DVDReadAsync(fileInfo, addr, length, offset, callback) DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)
// Minimum transfer size.
#define DVD_MIN_TRANSFER_SIZE 32
    BOOL DVDConvertEntrynumToPath(int, char *);
    int DVDConvertPathToEntrynum(char *);

    BOOL DVDOpenDir(char *, OSFstEntry *);
    BOOL DVDReadDir(OSFstEntry *, OSFstEntry *);
    BOOL DVDCloseDir(OSFstEntry *);
    int DVDChangeDir(char *); // this might be a BOOL, but the problem there
                              // is it's treated as 4 bytes...

    BOOL DVDCompareDiskID(DVDDiskID *, DVDDiskID *);
    int DVDCheckDisk();

    int DVDGetDriveStatus();
    int DVDGetCommandBlockStatus(DVDFileInfo *);

    BOOL __DVDPushWaitingQueue(int, struct DVDQueue *);
    struct DVDQueue *__DVDPopWaitingQueue();
    BOOL __DVDCheckWaitingQueue();
    void __DVDClearWaitingQueue();

    BOOL __DVDLowTestAlarm(OSAlarm *);
    void __DVDFSInit();
    void __fstLoad();

    u8 ErrorCode2Num(u32);
    void __DVDStoreErrorCode(u32);
    void __DVDPrintFatalMessage();

    extern OSThreadQueue __DVDThreadQueue;
    extern DVDState *LastState;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif