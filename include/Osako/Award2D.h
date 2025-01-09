#ifndef AWARD2D_H
#define AWARD2D_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class Award2D {
public:
    Award2D(JKRHeap *); // 0x8020f884
    void calc();        // 0x80210f48
    void draw();        // 0x80211254

    class OKButton {
        void calcAnm(); // 0x802112cc
        void zoomIn();  // 0x802113d8
        void zoomOut(); // 0x802113f4
    private:
        int _0;
        s16 _4;
        s16 _6;
        f32 _8;
        f32 _c;
        f32 _10;
    };

    void still() { _10 = 0; }
    void start() { _10 = 1; }
    bool isFinish() { return _10 == 6; } 

    u8 _00[0x10];
    u32 _10;
    u8 _14[0x58 - 0x14];
};

#endif
