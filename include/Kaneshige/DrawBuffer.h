#ifndef DRAWBUFFER_H
#define DRAWBUFFER_H

#include "types.h"
#include <JSystem/J3D/J3DDrawBuffer.h>

class DrawBuffer {
public:
    DrawBuffer();          // 0x80198810
    DrawBuffer(u32);       // 0x80198830    
    void reset();          // 0x80198888
    ~DrawBuffer();         // 0x80198904
    void create(u32, u32); // 0x8019897c
    void draw(u32);        // 0x80198a54
    void drawOpa(u32);     // 0x80198afc
    void drawXlu(u32);     // 0x80198b64

    // Inline
    void frameInit();       // 0x801988b8
    void getXluBuf() const; // 0x801988f4
    void getOpaBuf() const; // 0x801988fc
    void isDraw() const;    // 0x80198aec
    void isLock() const;    // 0x80198c58
    
    // Vtable 0x0
    virtual void drawIn(u32);   // 0x80198a50
    virtual void update();      // 0x80198bb0
    virtual void viewCalc(u32); // 0x80198c64

private:
    u16 mLockFlag;          // 0x4
    u16 mDrawFlag;          // 0x6
    J3DDrawBuffer *mXluBuf; // 0x8
    J3DDrawBuffer *mOpaBuf; // 0xc
}; // Size 0x10

#endif