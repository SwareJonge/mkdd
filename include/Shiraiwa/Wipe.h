#ifndef WIPE_H
#define WIPE_H

#include <JSystem/J2D/J2DGrafContext.h>
#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JUtility/JUTTexture.h>
#include "types.h"

// TODO: static member variables, possible errors

class TWipeBase
{
public:
    TWipeBase(JKRHeap *heap);

    JUTTexture *getTexture(u32 id);

    static void createBuffer(int, JKRHeap *heap);

    virtual ~TWipeBase();                          // 08
    virtual void init();                           // 0C
    virtual void draw(int, f32, JUTColor);         // 10
    virtual void calc(int, f32, JUTColor);         // 14
    virtual void reset();                          // 18
    virtual void calcMain(int, f32, JUTColor) = 0; // 1C
    virtual void changeState(int, int);            // 20
    virtual void drawPreIn(int, JUTColor);         // 24
    virtual void drawPreOut(int, JUTColor) {}      // 28
    virtual void drawMain(int, f32, JUTColor) = 0; // 2C

    // Unused
    static void loadTexture(JKRHeap *heap);
    void deleteBuffer();

private:
    static const char *scTextureName[3];
    static JUTTexture *sTexture[3];

    // Unused/Deadstripped
    static u8 sBuffer[0x10];
    static const u8 scBufSize;
    static const int scPortMax;

protected:
    J2DOrthoGraph **mGraph;
    u8 mConsoleNum;
    int mWipeState[4];
};

class TWipeCheck : public TWipeBase
{
public:
    TWipeCheck(JKRHeap *heap);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);

    virtual ~TWipeCheck();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawPreIn(int, JUTColor);
    virtual void drawMain(int, f32, JUTColor);

private:
    static s16 sWipeHeightOrg;
    static s16 sWipeWidthOrg;
    static f32 sAlphaDiffRate;

    u8 mAlpha[4];
    u8 mAlphaDiff[4];
    JGeometry::TBox2f mVp;
};

class TWipeCircle : public TWipeBase
{
public:
    TWipeCircle(JKRHeap *heap);

    void initGX();
    void drawBufferImage(JUTTexture *, JGeometry::TBox2f &, JGeometry::TBox2f &);

    virtual ~TWipeCircle();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);

private:
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    u32 _1c;
    u32 _20[4];
    JGeometry::TVec2f mPos[4];
    u8 _50[0x64 - 0x50];
    f32 _64[4];
};

class TWipeCurtain : public TWipeBase
{
public:
    TWipeCurtain(JKRHeap *heap);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);
    void drawImage(JUTTexture *, f32, f32, f32, f32);
    void initGXForTex(JUTColor);

    virtual ~TWipeCurtain();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);
    virtual void drawPreIn(int, JUTColor);

private:
    static f32 sLineWidth;
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    f32 mHeight[4];
};

class TWipeLineA : public TWipeBase
{
public:
    TWipeLineA(JKRHeap *);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);

    virtual ~TWipeLineA();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);
private:
    static s16 sWipeLineWidth;
    static s16 sWipeProgressLines;
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    f32 mOffset[4];
};

class TWipeLineB : public TWipeBase
{
public:
    TWipeLineB(JKRHeap *heap);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);

    virtual ~TWipeLineB();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);

private:
    static s16 sWipeLineHeight;
    static s16 sWipeProgressLines;
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    f32 mOffset[4];
};

class TWipeShut : public TWipeBase
{
public:
    TWipeShut(JKRHeap *heap);

    void initGX();
    void drawQuad(f32, f32, f32, f32, const JUTColor &);
    void drawTriangle(JGeometry::TVec2f *, const JUTColor &);

    virtual ~TWipeShut();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);

private:
    static s16 scWipeHeadHeight;
    static s16 scWipeHeadWidth;
    static f32 sChangeRate;
    static f32 sSecondWidthOrg;
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    f32 mWidth[4];
};

class TWipeSlide : public TWipeBase
{
public:
    TWipeSlide(JKRHeap *heap);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);
    void drawImage(JUTTexture *, bool, f32, f32, f32, f32);
    void initGXForTex(JUTColor);

    virtual ~TWipeSlide();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);

private:
    static u8 sWipePatternMask;

    s16 _1c[4];
    f32 _24[4];
    u8 _34[4];
    u8 mPattern;
    // u8 _0x2c[0x10];
};

class TWipeWaveA : public TWipeBase
{
public:
    TWipeWaveA(JKRHeap *heap);

    void drawQuad(f32, f32, f32, f32, const JUTColor &);
    void initGX(const JUTColor &);
    

    virtual ~TWipeWaveA();
    virtual void init();
    virtual void calcMain(int, f32, JUTColor);
    virtual void drawMain(int, f32, JUTColor);
    virtual void drawTex(f32, f32, f32, f32, f32, f32);

private:
    static f32 sWipeMoveRateMax;
    static f32 sWipeStartPos;
    static f32 sWipeHeadWidth;
    static f32 sWipeHeadHeight;
    static f32 sAlphaMinRate;
    static f32 sAlphaMaxRate;

    f32 mOffsetY[4];
    u32 _2c; // unused?
};

#endif