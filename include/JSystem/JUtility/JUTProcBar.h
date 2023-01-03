#ifndef _JUTPROCBAR_H
#define _JUTPROCBAR_H

#include "Dolphin/OS.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "types.h"

class JUTProcBar
{
public:
    struct CTime
    {
        u32 tick;
        u32 _04;
        u32 _08;
        u32 _0C;
        u8 _10;
        u8 _11;
        u8 _12;

        void start(u8 p1, u8 p2, u8 p3) {
            _10 = p1;
            _11 = p2;
            _12 = p3;
            tick = OSGetTick();
        }

        void end() {
            _04 = OSTicksToMicroseconds(OSDiffTick(OSGetTick(), tick));
            if (_04 == 0)
                _04 = 1;
        }

        CTime() { // weak
            _04 = 0;
            _08 = 0;
            _0C = 0;
        };
    };

    JUTProcBar(); // unused / inlined
    ~JUTProcBar(); // unused / inlined

    static void create();
    static void destroy();
    static void clear();

    void draw();
    void drawProcessBar();
    void drawHeapBar();

    // Unused Functions / Inlines
    void bar_subroutine(int, int, int, int, int, int, int, JUtility::TColor, JUtility::TColor);
    void adjustMeterLength(u32, f32 *, f32, f32, int *);
    void getUnuseUserBar();
    void addrToXPos(void *, int);
    void byteToXLen(int, int);
    void heapBar(JKRHeap *, int, int, int, int, int);

    u32 getGpCost() const {
        return mGpCost;
    }

    u32 getCpuCost() const {
        return mCpuCost;
    }

    u32 getUserCost(int idx) {
        return sManager->mUsers[idx]._04;
    }

    static JUTProcBar *getManager() {
        return sManager;
    }

    void setVisible(bool visible) {
        mVisible = visible;
    }

    void userStart(int idx, u8 p2, u8 p3, u8 p4) {
        sManager->mUsers[idx].start(p2, p3, p4);
        sManager->_108 |= 1 << idx;
    }

    static JUTProcBar *sManager; // might be private too
private:
    u8 _00[4];    // _00
    u32 _04;      // _04
    u32 _08;      // _08
    u32 _0C;      // _0C
    u8 _10[8];    // _10
    u32 mGpCost;  // _18
    u32 _1C;      // _1C
    u32 _20;      // _20
    u8 _24[8];    // _24
    u32 mCpuCost; // _2C
    u32 _30;      // _30
    u32 _34;      // _34
    u8 _38[8];    // _38
    u32 _40;      // _40
    u32 _44;      // _44
    u32 _48;      // _48
    u8 _4C[8];    // _4C
    u32 _54;      // _54
    u32 _58;      // _58
    u32 _5C;      // _5C
    u32 _60;      // _60
    CTime mUsers[8]; // _64
    u32 _104;     // _104
    u32 _108;     // _108
    bool mVisible;// _10C
    u32 _110;     // _110
    u32 _114;     // _114
    u32 _118;     // _118
    u32 _11C;     // _11C
    u32 _120;     // _120
    u32 _124;     // _124
    u32 _128;     // _128
    u32 _12C;     // _12C
    bool mHeapBarVisible; // _130

private:
}; // 0x134 size

#endif