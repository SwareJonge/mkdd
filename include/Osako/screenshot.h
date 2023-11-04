#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <dolphin/types.h>
#include <dolphin/hio.h>
#include <dolphin/gx.h>

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

typedef void *(*SCREENSHOTAllocFunc)(u32 size);
typedef void (*SCREENSHOTFreeFunc)(void *p);

void SCREENSHOTSetRenderModeObj(GXRenderModeObj *obj);
void SCREENSHOTService(void *bufferXFB, SCREENSHOTAllocFunc allocFunc, SCREENSHOTFreeFunc freeFunc);

extern BOOL g_connected;
extern BOOL g_ping_received;
extern u32 g_received_signal;
extern BOOL g_mail_waiting;
extern BOOL g_minimize_buffer;
extern u8 *g_data;
extern s32 g_usb_channel;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus


#endif