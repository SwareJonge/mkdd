#ifndef _DOLPHIN_DVDFS_H
#define _DOLPHIN_DVDFS_H

#include <dolphin/os.h>

#ifdef __cplusplus
extern "C"
{
// clang-format off
#endif

// TODO


s32 DVDConvertPathToEntrynum(const char *pathPtr);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo *fileInfo);
BOOL DVDOpen(const char *fileName, DVDFileInfo *fileInfo);
BOOL DVDClose(DVDFileInfo *fileInfo);
BOOL DVDGetCurrentDir(char *path, u32 maxlen);
BOOL DVDChangeDir(char *dirName);

BOOL DVDReadAsyncPrio(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
s32 DVDReadPrio(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, s32 prio);

BOOL DVDSeekAsyncPrio(DVDFileInfo *fileInfo, s32 offset, DVDCallback callback, s32 prio);
s32 DVDSeekPrio(DVDFileInfo *fileInfo, s32 offset, s32 prio);

s32 DVDGetFileInfoStatus(DVDFileInfo *fileInfo);
BOOL DVDFastOpenDir(s32 entrynum, DVDDir *dir);
BOOL DVDOpenDir(char *dirName, DVDDir *dir);
BOOL DVDReadDir(DVDDir *dir, DVDDirEntry *dirent);
BOOL DVDCloseDir(DVDDir *dir);
void DVDRewindDir(DVDDir *dir);
void *DVDGetFSTLocation();

BOOL DVDPrepareStreamAsync(DVDFileInfo *fileInfo, u32 length, u32 offset, DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo *fileInfo, u32 length, u32 offset);

s32 DVDGetTransferredSize(DVDFileInfo *fileinfo);

extern OSThreadQueue __DVDThreadQueue;

#ifdef __cplusplus
}
// clang-format on
#endif

#endif