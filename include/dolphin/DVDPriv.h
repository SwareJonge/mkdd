#ifndef _DOLPHIN_DVDPRIV
#define _DOLPHIN_DVDPRIV

#include <dolphin/dvd.h>
#include <dolphin/hw_regs.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DVDBB1 {
  u32 appLoaderLength;
  void* appLoaderFunc1;
  void* appLoaderFunc2;
  void* appLoaderFunc3;
} DVDBB1;

typedef struct DVDBB2 {
  u32 bootFilePosition;
  u32 FSTPosition;
  u32 FSTLength;
  u32 FSTMaxLength;
  void* FSTAddress;
  u32 userPosition;
  u32 userLength;

  u32 padding0;
} DVDBB2;

#ifdef __cplusplus
}
#endif // _DOLPHIN_DVDPRIV

#endif __DVDPRIV_H__
