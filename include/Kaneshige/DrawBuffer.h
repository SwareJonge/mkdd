#ifndef DRAWBUFFER_H
#define DRAWBUFFER_H

#include "types.h"
#include <JSystem/J3D/J3DDrawBuffer.h>

class DrawBuffer
{
public:
    DrawBuffer();                                // 0x80198810
    DrawBuffer(u32 bufSize);                     // 0x80198830
    void reset();                                // 0x80198888
    ~DrawBuffer();                               // 0x80198904
    void create(u32 opaBufSize, u32 xluBufSize); // 0x8019897c
    void draw(u32 viewNo);                       // 0x80198a54
    void drawOpa(u32 viewNo);                    // 0x80198afc
    void drawXlu(u32 viewNo);                    // 0x80198b64

    // Inline
    void create(u32 bufSize) // 0x801b8c20
    {
        create(bufSize, bufSize);
    }

    void frameInit() // 0x801988b8
    {
        getOpaBuf()->frameInit();
        getXluBuf()->frameInit();
    }

    void lock() { mLockFlag |= 1; }

    J3DDrawBuffer *getXluBuf() const { return mXluBuf; }    // 0x801988f4
    J3DDrawBuffer *getOpaBuf() const { return mOpaBuf; }    // 0x801988fc
    bool isDraw() const { return mDrawFlag != 0; }          // 0x80198aec
    bool isLock() const { return mLockFlag & ~0xfffffffe; } // 0x80198c58
    void clrDraw() { mDrawFlag = 0; }

    static bool isValidDrawBuffer(J3DDrawBuffer *drawBuf) { return drawBuf != &sDummyBuf; }

    // Vtable 0x0
    virtual void drawIn(u32 viewNo);   // 0x80198a50
    virtual void update();             // 0x80198bb0
    virtual void viewCalc(u32 viewNo); // 0x80198c64

private:
    static J3DDrawBuffer sDummyBuf;

    u16 mLockFlag;          // 0x4
    u16 mDrawFlag;          // 0x6
    J3DDrawBuffer *mOpaBuf; // 0x8
    J3DDrawBuffer *mXluBuf; // 0xc

}; // Size 0x10

class EffectDrawBuffer : public DrawBuffer {
public:
    EffectDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {}
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
};

class FeelDrawBuffer : public DrawBuffer {
public:
    FeelDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {}
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
};

class GeoDrawBuffer : public DrawBuffer {
public:
    GeoDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {}
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
};

class ShadowGeoDrawBuffer : public DrawBuffer {
public:
    ShadowGeoDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {}
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
};

class ItemDrawBuffer : public DrawBuffer {
public:
    ItemDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {
        mTargetKart = -1;
    }
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);

    void setTargetKart(s16 kartNo) { mTargetKart = kartNo; }

private:
    s16 mTargetKart;
};

class JugemDrawBuffer : public DrawBuffer {
public:
    JugemDrawBuffer(u32 bufSize=4) : DrawBuffer(bufSize) {}
    virtual void drawIn(u32);
    virtual void update();
    virtual void viewCalc(u32);
};

class OsageDrawBuffer : public DrawBuffer {
public:
    OsageDrawBuffer(u32 bufSize) : DrawBuffer(bufSize) {}
        // Vtable 0x0
    virtual void drawIn(u32 idx);   // 0x80198a50
    virtual void update();          // 0x80198bb0
    virtual void viewCalc(u32 idx); // 0x80198c64

    void setTargetKart(s16 targetKart) { mTargetKart = targetKart; }
private:
    s16 mTargetKart;
}; // Size 0x14

#endif
