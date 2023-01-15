#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "types.h"

void HostIOEnumCallback(s32);
void HostIOCallBack();
// Function pointers
// void GrabChunk(unsigned long, void *, void *(*)(unsigned long), void (*)(void *));
// void CheckMail(void *, void * (*) (unsigned long), void (*) (void *));
// Inline/Unused
// void CopyoutEFB(unsigned char *, unsigned long, unsigned long);
// void CopyoutPortionEFB(unsigned char *, unsigned long, unsigned long, unsigned long);
// void TakeScreenshotEFB(void * (*) (unsigned long));
// void TakeScreenshotXFB(void *, void * (*) (unsigned long));
// void WriteScreenshotPortionEFBtoUSB(unsigned long, void * (*) (unsigned long));
// void ConnectToUSB();

#ifdef __cplusplus
    extern "C"
{
#endif // ifdef __cplusplus
void SCREENSHOTService(void *, void *, void *);
#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif