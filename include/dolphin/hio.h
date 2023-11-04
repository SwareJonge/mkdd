#ifndef _DOLPHIN_HOSTIO_H
#define _DOLPHIN_HOSTIO_H

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

#include <dolphin/os.h>

#include <dolphin/types.h>

typedef enum
{
    HIO_DEVICE_INVALID = -1,
    HIO_DEVICE_EXI2USB_0 = 0,
    HIO_DEVICE_EXI2USB_1 = 1,
    HIO_DEVICE_MrEXI = 2
} HostIOType;

typedef enum
{
    GRAB_NOT_TRANSFERRING = 0,
    GRAB_TRANSFERRING = 1
} HostIOGrabStatus;

typedef BOOL (*HIOEnumCallback)(s32 chan);
typedef void (*HIOCallback)(void);

BOOL HIOInit(s32 channel, HIOCallback cb);
BOOL HIOEnumDevices(HIOEnumCallback);
BOOL HIOWrite(u32 chunkSize, void * buf, u32 bufSize);
BOOL HIOReadMailbox(u32 *);
BOOL HIOWriteMailbox(u32);

#ifdef __cplusplus
}
#endif

#endif
