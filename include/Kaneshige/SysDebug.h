#ifndef SYSDEBUG_H
#define SYSDEBUG_H

#include <dolphin/mtx.h>

#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTGamePad.h"

#include "JSystem/JUtility/TColor.h"

class SysDbUsrPage : public JKRDisposer
{
public:
    SysDbUsrPage();
    virtual ~SysDbUsrPage();
    virtual void draw() = 0;

    JSULink<SysDbUsrPage> mLink;
};

class SysDebug
{
public:
    class HeapInfo : public JKRDisposer
    {
    public:
        HeapInfo(JKRHeap *heap, char *label);  // 0x801b2ad4
        virtual ~HeapInfo();                  // 0x801b2c08
        void clrGroup();                      // 0x801b2cd0
        void setGroup(const char *name);      // 0x801b2d04
        int searchSameName(const char *name); // 0x801b2d74

        void setDefaultGroup() { mHeap->changeGroupID(0xff); }

        JKRHeap *mHeap;          // 0x18
        s16 mGroupID;            // 0x1c
        const char *mGroupName[30]; // 0x20
        char mHeapLabel[8];      // 0x98
        char mTerminator;        // 0xa0
        JSULink<HeapInfo> mLink; // 0xa4
    };                           // Size: 0xb4

    static SysDebug *createManager();                     // 0x801b0d08
    SysDebug();                                           // 0x801b0d50
    void ctrlDebugMode();                                 // 0x801b1034
    void setHeapGroup(const char *name, JKRHeap *heap);   // 0x801b1648
    void setDefaultHeapGroup(JKRHeap *heap);              // 0x801b16c8
    void setUserTimeLabel(int, const char *label);        // 0x801b1768
    void clrAllUserTimeLabel();                           // 0x801b1788
    void beginUserTime(int id);                           // 0x801b17ac
    void endUserTime(int id);                             // 0x801b18b4
    void draw();                                          // 0x801b19a4
    void drawProcTime();                                  // 0x801b1ba8
    char *getHeapLabel(JKRHeap *heap);                    // 0x801b1f10
    char *getHeapTypeName(JKRHeap *heap);                 // 0x801b1fa4
    void printHeap(s32 x, s32 y, JKRHeap *heap);          // 0x801b2048
    void printHeapTree(JKRHeap *heap, s32, s32 *);        // 0x801b214c
    void drawMemMap();                                    // 0x801b225c
    void drawUsrPage();                                   // 0x801b2340
    void drawHeapSpec();                                  // 0x801b24a4
    bool beginDebugHeap();                                // 0x801b27ac
    void endDebugHeap();                                  // 0x801b27f4
    void createHeapInfo(JKRHeap *heap, char *);           // 0x801b2834
    void destroyHeapInfo(JKRHeap *heap);                  // 0x801b2900
    HeapInfo *findHeapInfo(JKRHeap *heap);                // 0x801b296c
    bool appendPage(SysDbUsrPage *);                      // 0x801b2df4
    void removePage(SysDbUsrPage *);                      // 0x801b2e40
    static void checkInvalidFloat(f32 f, char *name);     // 0x801b2e88
    static void checkInvalidFloat(f32 f, u32 id);         // UNUSED
    static void checkInvalidMatrix(Mtx m, char *name);    // 0x801b2f40
    static void checkInvalidMatrix(Mtx m, u32 id);        // 0x801b304c
    static void checkInvalidVector(Vec *vec, char *name); // 0x801b3158
    static void checkInvalidVector(Vec *vec, u32 id);     // 0x801b330c

    // Inline Functions
    void appendInfo(HeapInfo *info) { mInfoList.append(&info->mLink); }
    void removeInfo(HeapInfo *info) { mInfoList.remove(&info->mLink); }

    s32 sizeKByte(s32 size) { return (size + 1024 - 1) / 1024;  }

    JKRHeap *getDebugHeap() const { return mDebugHeap; }

    static SysDebug *getManager() { return sManager; }

    static inline SysDebug *checkNaNMatrix(Mtx mtx, unsigned long unknown) {
        checkInvalidMatrix(mtx, unknown);
    }

    static inline SysDebug *checkNaNVector(Vec *vec, unsigned long unknown) {
        checkInvalidVector(vec, unknown);
    }

private:
    static const GXColor cUserBarColorTable[8]; // 0x803780c0
    static SysDebug *sManager;

    bool mDrawPage; // 0
    JUTGamePad mGamePad; // 8
    int mPageNo; // b0
    JKRHeap *mDebugHeap;    // b4
    JKRHeap *mBackupHeap;   // b8, a backup of the previous current heap
    JKRHeap *mSelectedHeap; // bc, the heap the user is currently looking at
    int mGroupIdx; // c0
    const char *mUserTimeLabels[8]; // c4
    int mDrawPosX; // e4
    int mDrawPosY; // e8
    int mCharWidth; // ec
    int mCharHeight; // f0
    bool mDrawUserTime; // f4;
    int mProcTimeType; // f8
    u32 mProcTimeFrames; // fc
    f32 mCPUTime; // 100
    f32 mCPUTimeMax; // 104
    f32 mAllTime; // 108
    f32 mAllTimeMax; // 10c
    u32 mBufferIdx; // 110
    bool mDrawProcTime; // 114
    f32 *mCPUTimes; // 118
    f32 *mGPUTimes; // 11c
    s16 mUsrPageNo; // 120
    JSUList<SysDbUsrPage> mUsrPageList; // 124
    J2DOrthoGraph *mOrtho;              // 130
    JSUList<HeapInfo> mInfoList;        // 134
};

// Make macro for every function? or is that too ridiculous

#define GetSysDebug() \
    SysDebug::getManager()

#define SYSDBG_SetHeapGroup(name, heap) \
    GetSysDebug()->setHeapGroup(name, heap);
#define SYSDBG_SetDefaultHeapGroup(heap) \
    GetSysDebug()->setDefaultHeapGroup(heap);
#define SYSDBG_CreateHeapInfo(heap, name) \
    GetSysDebug()->createHeapInfo(heap, name);
#define SYSDBG_DestroyHeapInfo(heap) \
    GetSysDebug()->destroyHeapInfo(heap);
#define SYSDBG_ClearAllLabels() \
    GetSysDebug()->clrAllUserTimeLabel();

#ifdef DEBUG
#define SYSDBG_AppendPage(page) \
    GetSysDebug()->appendPage(page);
#define SYSDBG_CreateDebugHeapInfo(heap, name) \
    GetSysDebug()->createHeapInfo(heap, name);
#define SYSDBG_SetUserTimeLabel(id, name) \
    GetSysDebug()->setUserTimeLabel(id, name);
#define SYSDBG_BeginUserTime(id) \
    GetSysDebug()->beginUserTime(id);
#define SYSDBG_EndUserTime(id) \
    GetSysDebug()->endUserTime(id);

#else
#define SYSDBG_AppendPage(...)
#define SYSDBG_CreateDebugHeapInfo(...)
#define SYSDBG_SetUserTimeLabel(...)
#define SYSDBG_BeginUserTime(...)
#define SYSDBG_EndUserTime(...)
#endif

#endif
