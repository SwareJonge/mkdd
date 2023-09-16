#ifndef _DOLPHIN_DVDLOW_H
#define _DOLPHIN_DVDLOW_H

#include <dolphin/os.h>

#include <dolphin/DVDPriv.h>

#ifdef __cplusplus
extern "C" {
// clang-format off
#endif

typedef void (*DVDLowCallback)(u32 intType);

void __DVDInitWA();
void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext *context);

BOOL DVDLowRead(void *addr, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowSeek(u32 offset, DVDLowCallback callback);
BOOL DVDLowWaitCoverClose(DVDLowCallback callback);
BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback);
BOOL DVDLowStopMotor(DVDLowCallback callback);
BOOL DVDLowRequestError(DVDLowCallback callback);
BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback);
BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback);
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);

void DVDLowReset();

DVDLowCallback DVDLowClearCallback();

void __DVDLowSetWAType(u32 type, u32 location);
BOOL __DVDLowTestAlarm(OSAlarm *alarm);

#ifdef __cplusplus
}
// clang-format on
#endif

#endif