#ifndef _DOLPHIN_DVD_H
#define _DOLPHIN_DVD_H

#include "types.h"
#include <dolphin/os/OSAlarm.h>

#define DVD_MIN_TRANSFER_SIZE 32

#define DVD_STATE_FATAL_ERROR -1
#define DVD_STATE_END 0
#define DVD_STATE_BUSY 1
#define DVD_STATE_WAITING 2
#define DVD_STATE_COVER_CLOSED 3
#define DVD_STATE_NO_DISK 4
#define DVD_STATE_COVER_OPEN 5
#define DVD_STATE_WRONG_DISK 6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_PAUSING 8
#define DVD_STATE_IGNORED 9
#define DVD_STATE_CANCELED 10
#define DVD_STATE_RETRY 11

#define DVD_FILEINFO_READY 0
#define DVD_FILEINFO_BUSY 1

#define DVD_RESULT_GOOD 0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED -2
#define DVD_RESULT_CANCELED -3

#define DVD_AIS_SUCCESS 0x0

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct DVDDiskID
{
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize; // 0 = default
    u8 padding[22];      // 0's are stored
} DVDDiskID;

typedef struct DVDCommandBlock DVDCommandBlock;

typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);

struct DVDCommandBlock
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
};

typedef struct DVDFileInfo DVDFileInfo;

typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);

struct DVDFileInfo
{
    DVDCommandBlock cb;
    u32 startAddr;
    u32 length;
    DVDCallback callback;
};

typedef struct
{
    u32 entryNum;
    u32 location;
    u32 next;
} DVDDir;

typedef struct
{
    u32 entryNum;
    BOOL isDir;
    char *name;
} DVDDirEntry;

typedef struct DVDDriveInfo
{
    u16 revisionLevel;
    u16 deviceCode;
    u32 releaseDate;
    u8 padding[24];
} DVDDriveInfo;

void DVDInit();
BOOL DVDClose(DVDFileInfo *f);
BOOL DVDSetAutoFatalMessaging(BOOL);
BOOL DVDSetAutoInvalidation(BOOL autoInval);
void DVDPause();
void DVDReset();
void DVDResume();
s32 DVDCancel(DVDCommandBlock *block);

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo *fileInfo);
s32 DVDGetCommandBlockStatus(const DVDCommandBlock *block);
BOOL DVDCancelAsync(DVDCommandBlock *block, DVDCBCallback callback);
s32 DVDCancel(DVDCommandBlock *block);
BOOL DVDCancelAllAsync(DVDCBCallback callback);
s32 DVDCancelAll();
BOOL DVDPrepareStreamAsync(DVDFileInfo *fInfo, u32 length, u32 offset, DVDCallback callback);

BOOL DVDPrepareStreamAbsAsync(DVDCommandBlock *block, u32 length, u32 offset,
                                DVDCBCallback callback);
s32 DVDPrepareStream(DVDFileInfo *fInfo, u32 length, u32 offset);

BOOL DVDCancelStreamAsync(DVDCommandBlock *block, DVDCBCallback callback);
s32 DVDCancelStream(DVDCommandBlock *block);

BOOL DVDStopStreamAtEndAsync(DVDCommandBlock *block, DVDCBCallback callback);
s32 DVDStopStreamAtEnd(DVDCommandBlock *block);

BOOL DVDGetStreamErrorStatusAsync(DVDCommandBlock *block, DVDCBCallback callback);
s32 DVDGetStreamErrorStatus(DVDCommandBlock *block);

BOOL DVDGetStreamPlayAddrAsync(DVDCommandBlock *block, DVDCBCallback callback);
s32 DVDGetStreamPlayAddr(DVDCommandBlock *block);

BOOL DVDInquiryAsync(DVDCommandBlock *block, DVDDriveInfo *info, DVDCBCallback callback);

BOOL DVDReadDiskID(DVDCommandBlock *block, DVDDiskID *diskID, DVDCBCallback callback);

s32 DVDGetDriveStatus();
DVDDiskID *DVDGetCurrentDiskID();
int DVDCheckDisk();

BOOL DVDReadAsyncPrio(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset,
                        DVDCallback callback, s32 prio);

BOOL DVDReadAbsAsyncPrio(DVDCommandBlock *block, void *addr, s32 length, s32 offset,
                            DVDCBCallback callback, s32 prio);

BOOL DVDReadAbsAsyncForBS(DVDCommandBlock *block, void *addr, s32 length, s32 offset,
                            DVDCBCallback callback);

BOOL __DVDTestAlarm(OSAlarm *alarm);
void __DVDPrepareResetAsync(DVDCBCallback callback);

#define DVDReadAsync(fileInfo, addr, length, offset, callback) \
    DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)
#define DVDSeekAsync(fileInfo, offset, callback) \
    DVDSeekAsyncPrio((fileInfo), (offset), (callback), 2)

BOOL DVDCompareDiskID(DVDDiskID *id1, DVDDiskID *id2);
void __DVDStoreErrorCode(u32 error);
void __DVDPrintFatalMessage();

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DVD