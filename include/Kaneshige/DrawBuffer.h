#ifndef DRAWBUFFER_H
#define DRAWBUFFER_H

#include "types.h"

class DrawBuffer {
public:
    DrawBuffer(u32);
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);

private:
    // 00 Vtable
    u8 _04[0x10 - 0x4];
}; // Size 0x10

#endif