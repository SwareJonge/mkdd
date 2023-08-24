#ifndef _THP_THPREAD_H
#define _THP_THPREAD_H

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CreateReadThread(OSPriority prio);
void ReadThreadStart();
void ReadThreadCancel();

#ifdef __cplusplus
}
#endif

#endif
