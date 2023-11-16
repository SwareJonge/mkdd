#ifndef SHADOWSCR_H
#define SHADOWSCR_H

#include <dolphin/mtx.h>
#include <JSystem/JUtility/JUTTexture.h>

class ShadowScreen
{
public:
    
    ShadowScreen();
    void draw();
private:
    u16 mStartX, mStartY, mEndX, mEndY; // screen bounds
    Mtx mPosMtx;
    Mtx44 mProjMtx;
    JUTTexture mTexture;
}; // Size: 0xb8

#endif // SHADOWSCR_H