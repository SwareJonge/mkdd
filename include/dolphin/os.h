#ifndef _DOLPHIN_OS_H
#define _DOLPHIN_OS_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

// Defines for cached and uncached memory.
#define OS_BASE_CACHED (0x80000000)
#define OS_BASE_UNCACHED (0xC0000000)

// Address conversions.
#define OSPhysicalToCached(paddr) ((void *)((u32)(paddr) + OS_BASE_CACHED))
#define OSPhysicalToUncached(paddr) ((void *)((u32)(paddr) + OS_BASE_UNCACHED))
#define OSCachedToPhysical(caddr) ((u32)((u8 *)(caddr)-OS_BASE_CACHED))
#define OSUncachedToPhysical(ucaddr) ((u32)((u8 *)(ucaddr)-OS_BASE_UNCACHED))
#define OSCachedToUncached(caddr) ((void *)((u8 *)(caddr) + (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#define OSUncachedToCached(ucaddr) ((void *)((u8 *)(ucaddr) - (OS_BASE_UNCACHED - OS_BASE_CACHED)))

#define OS_CACHED_REGION_PREFIX 0x8000
#define OS_UNCACHED_REGION_PREFIX 0xC000
#define OS_PHYSICAL_MASK 0x3FFF

u32 __OSBusClock : (0x800000F8);

#define OS_BUS_CLOCK __OSBusClock
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)

#define OSTicksToSeconds(ticks) ((ticks) / OS_TIMER_CLOCK)
#define OSTicksToMilliseconds(ticks) ((ticks) / (OS_TIMER_CLOCK / 1000))
#define OSTicksToMicroseconds(ticks) (((ticks) * 8) / (OS_TIMER_CLOCK / 125000))
#define OSTicksToNanoseconds(ticks) (((ticks) * 8000) / (OS_TIMER_CLOCK / 125000))

#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSMicrosecondsToTicks(usec) (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)
#define OSNanosecondsToTicks(nsec) (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)
#define OSDiffTick(tick1, tick0) ((s32)(tick1) - (s32)(tick0))

  // __ppc_eabi_init
  extern void __OSPSInit();
  extern void __OSFPRInit();
  extern void __OSCacheInit();

  void OSInit();

  // OS logging
  void OSReport(const char *message, ...);
  void OSPanic(const char *file, int line, const char *message, ...);

  typedef s16 __OSInterrupt;
  typedef u64 OSTime;
  typedef u32 OSTick;

  volatile u16 OS_AI_DMA_ADDR_HI : 0xCC005030;
  volatile u16 OS_AI_DMA_ADDR_LO : 0xCC005032;

  volatile u16 OS_ARAM_DMA_BASE : 0xCC005000;
  volatile u16 OS_ARAM_DMA_ADDR_HI : 0xCC005020;
  volatile u16 OS_ARAM_DMA_ADDR_LO : 0xCC005022;
  volatile u16 OS_DI_DMA_ADDR : 0xCC006014;

#define OSError(...) OSPanic(__FILE__, __LINE__, __VA_ARGS__)
#ifndef MATCHING
#define OSErrorLine(...) OSError(__VA_ARGS__)
#define OSHalt(msg) OSPanic(__FILE__, __LINE__, msg)
#else
#define OSErrorLine(line, ...) OSPanic(__FILE__, line, __VA_ARGS__)
#define OSHalt(line, msg) OSPanic(__FILE__, line, msg)
#endif


  void OSRegisterVersion(const char *);

  typedef struct DVDDiskID DVDDiskID;

  struct DVDDiskID
  {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize; // 0 = default
    u8 padding[22];      // 0's are stored
  };

  typedef struct OSBootInfo_s
  {
    DVDDiskID DVDDiskID; // 0x0
    // u32 DVDmagic;     // 0x18 0xc2339f3d for Nintendo Game Disc
    u32 magic;         // 0x20
    u32 version;       // 0x24
    u32 memorySize;    // 0x28
    u32 consoleType;   // 0x2C
    void *arenaLo;     // 0x30
    void *arenaHi;     // 0x34
    void *FSTLocation; // 0x38
    u32 FSTMaxLength;  // 0x3C
  } OSBootInfo;
  typedef struct BI2Debug
  {
    s32 debugMonSize;  // 0x0
    s32 simMemSize;    // 0x4
    u32 argOffset;     // 0x8
    u32 debugFlag;     // 0xC
    int trackLocation; // 0x10
    int trackSize;     // 0x14
    u32 countryCode;   // 0x18
    u8 unk[8];         // 0x1C
    u32 padSpec;       // 0x24
  } BI2Debug;

  typedef struct OSDummyCommandBlock OSDummyCommandBlock;
  typedef void OSCommandBlockCallback(int, struct OSDummyCommandBlock *);

  // TODO: This might be first 0x30 bytes of DVDPlayer?
  struct OSDummyCommandBlock
  {
    u8 _00[8];                   // _00
    u32 _08;                     // _08
    int _0C;                     // _0C
    unknown _10;                 // _10
    unknown _14;                 // _14
    DVDDiskID *_18;              // _18
    unknown _1C;                 // _1C
    unknown _20;                 // _20
    DVDDiskID *diskID;           // _24
    OSCommandBlockCallback *_28; // _28
    u8 _2C[4];                   // _2C
  };

  // OSArena
  extern void *__OSArenaHi;

  void *OSGetArenaHi(void);
  void *OSGetArenaLo(void);

  void OSSetArenaHi(void *addr);
  void OSSetArenaLo(void *addr);

  // OSMemory
  typedef int OSHeapHandle;

  extern volatile OSHeapHandle __OSCurrHeap;

  void *OSInitAlloc(void *, void *, int);
  OSHeapHandle OSCreateHeap(void *, void *);
  OSHeapHandle OSSetCurrentHeap(OSHeapHandle);
  void *OSAllocFromHeap(OSHeapHandle, u32);
  long OSCheckHeap(OSHeapHandle);
  void OSFreeToHeap(OSHeapHandle heap, void *ptr);

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr) OSFreeToHeap(__OSCurrHeap, (ptr))
  // OSLink
  void __OSModuleInit(void);

  // OSFont
  u16 OSGetFontEncode();
  u8 OSInitFont();
  char *OSGetFontTexture(const char *string, void **image, s32 *x, s32 *y, s32 *width);
  char *OSGetFontWidth(const char *string, s32 *width);

  // targsupp
  extern void TRKAccessFile(void);
  extern void TRKCloseFile(void);

  typedef struct OSFstEntry
  {
    int mEntryNum;
    int mNextEntryNum;
    char *mFileNameMaybe;
  } OSFstEntry;

  void DCInvalidateRange(void *addr, u32 nBytes);
  void DCFlushRange(void *addr, u32 nBytes);
  void DCStoreRange(void *addr, u32 nBytes);
  void DCFlushRangeNoSync(void *addr, u32 nBytes);
  void DCStoreRangeNoSync(void *addr, u32 nBytes);
  void DCZeroRange(void *addr, u32 nBytes);
  void DCTouchRange(void *addr, u32 nBytes);

  void ICInvalidateRange(void *addr, u32 nBytes);

#define LC_BASE_PREFIX 0xE000
#define LC_BASE (LC_BASE_PREFIX << 16)

  void L2Init(void);
  void L2Disable(void);
  void L2GlobalInvalidate(void);
  void LCEnable(void);
  void LCDisable(void);
  void LCLoadBlocks(void *destTag, void *srcAddr, u32 numBlocks);
  void LCStoreBlocks(void *destAddr, void *srcTag, u32 numBlocks);
  u32 LCLoadData(void *destAddr, void *srcAddr, u32 nBytes);
  u32 LCStoreData(void *destAddr, void *srcAddr, u32 nBytes);
  u32 LCQueueLength(void);
  void LCQueueWait(u32 len);
  void LCFlushQueue(void);

#define LCGetBase() ((void *)LC_BASE)

  u64 OSGetTime();
  u32 OSGetTick();

#define OS_SYS_CALL_HANDLER ((void *)0x80000C00)
#define OS_HANDLER_SLOT_SIZE (0x100)

  void __OSSystemCallVectorStart();
  void __OSSystemCallVectorEnd();

  void __OSInitSystemCall();

  void __OSUnlockSramEx(int);
  u8 *__OSLockSramEx(void);

  void __OSUnmaskInterrupts(int);
  BOOL OSDisableInterrupts(void);
  BOOL OSRestoreInterrupts(BOOL);
  BOOL OSEnableInterrupts();

  uint OSGetSoundMode();
  void OSSetSoundMode(uint);
  void OSSetSaveRegion(int, int);
  int OSGetProgressiveMode();

  typedef struct OSFunctionInfo
  {
    void *m_function;
    uint _04;
    u8 _08[8];
  } OSFunctionInfo;

  void OSRegisterResetFunction(OSFunctionInfo *);
  BOOL OSGetResetSwitchState();

  void OSProtectRange(u32, u32, u32, u32); /** TODO: Are these params correct? */
  extern u32 __OSFpscrEnableBits; /** TODO: find a wrapper for this. Symbol is defined in OSError.c. */

#define HW_REG(reg, type) *(volatile type *)(u32)(reg)
#define OSGetTicksPerSecond() (*(u32 *)0x800000F8 / 4)

// u32 GameCode : 0x80000000;
// u32 FSTLocationInRam : 0x80000038;

// Rounds to nearest multiple of 20 upwards and downwards
#define OSRoundUp32B(x) (((u32)(x) + 0x1F) & ~(0x1F))
#define OSRoundDown32B(x) (((u32)(x)) & ~(0x1F))

  inline void OSInitFastCast()
  {
    asm
    {
		li r3, 4
		oris r3, r3, 4
		mtspr 0x392, r3
		li r3, 5
		oris r3, r3, 5
		mtspr 0x393, r3
		li r3, 6
		oris r3, r3, 6
		mtspr 0x394, r3
		li r3, 7
		oris r3, r3, 7
		mtspr 0x395, r3
    }
  }

#include <dolphin/os/OSAlarm.h>
#include <dolphin/os/OSContext.h>
#include <dolphin/os/OSException.h>
#include <dolphin/os/OSError.h>
#include <dolphin/os/OSMessage.h>
#include <dolphin/os/OSMutex.h>
#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
