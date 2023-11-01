#ifndef SYSDEBUG_H
#define SYSDEBUG_H

#include <dolphin/mtx.h>

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "JSystem/JUTility/TColor.h"

class SysDbUsrPage : public JKRDisposer
{
public:
    SysDbUsrPage();
    virtual ~SysDbUsrPage();

private:
    JSULink<SysDbUsrPage> link;
};

class SysDebug
{
public:
    class HeapInfo : public JKRDisposer
    {
    public:
        HeapInfo(JKRHeap *heap, char *);   // 0x801b2ad4
        virtual ~HeapInfo();               // 0x801b2c08
        void clrGroup();                   // 0x801b2cd0
        void setGroup(const char *);       // 0x801b2d04
        void searchSameName(const char *); // 0x801b2d74
    private:
        JKRHeap *mHeap;          // 0x18
        s16 mGroupID;            // 0x1c
        char *mGroups[30];       // 0x20
        char mGroupName;         // 0x98
        JSULink<HeapInfo> mLink; // 0xa4
    }; // Size: 0xb4

    static void createManager();                          // 0x801b0d08
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
    void beginDebugHeap();                                // 0x801b27ac
    void endDebugHeap();                                  // 0x801b27f4
    void createHeapInfo(JKRHeap *heap, char *);           // 0x801b2834
    void destroyHeapInfo(JKRHeap *heap);                  // 0x801b2900
    HeapInfo *findHeapInfo(JKRHeap *heap);                // 0x801b296c
    bool appendPage(SysDbUsrPage *);                      // 0x801b2df4
    void removePage(SysDbUsrPage *);                      // 0x801b2e40
    static void checkInvalidFloat(float f, char *name);   // 0x801b2e88
    static void checkInvalidMatrix(Mtx m, char *name);    // 0x801b2f40
    static void checkInvalidMatrix(Mtx m, u32 id);        // 0x801b304c
    static void checkInvalidVector(Vec *vec, char *name); // 0x801b3158
    static void checkInvalidVector(Vec *vec, u32 id);     // 0x801b330c

    JKRHeap *getDebugHeap() const { return mDebugHeap; }

    static SysDebug *getManager() { return sManager; }

private:
    static JUtility::TColor cUserBarColorTable[4]; // 0x803780c0
    static SysDebug *sManager;

    u8 _0[0xb8];
    JKRHeap *mDebugHeap;
};

#define SYSDBG_SetHeapGroup(name, heap) \
    SysDebug::getManager()->setHeapGroup(name, heap);

#define SYSDBG_SetDefaultHeapGroup(heap) \
    SysDebug::getManager()->setDefaultHeapGroup(heap);

// TODO
#define SYSDBG_CreateHeapInfo2(heap, name) \
    SysDebug::getManager()->createHeapInfo(heap, name);

#ifdef DEBUG
#define SYSDBG_CreateHeapInfo(heap, name) \
    SysDebug::getManager()->createHeapInfo(heap, name);
#define SYSDBG_SetUserTimeLabel(id, name) \
    SysDebug::getManager()->setUserTimeLabel(id, name);
#define SYSDBG_BeginUserTime(id) \
    SysDebug::getManager()->beginUserTime(id);
#define SYSDBG_EndUserTime(id) \
    SysDebug::getManager()->endUserTime(id);

#else
#define SYSDBG_CreateHeapInfo(...)
#define SYSDBG_SetUserTimeLabel(...)
#define SYSDBG_BeginUserTime(...)
#define SYSDBG_EndUserTime(...)
#endif

#endif