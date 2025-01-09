#ifndef GHOSTSCREEN_H
#define GHOSTSCREEN_H

#include <JSystem/JUtility/JUTTexture.h>
#include "types.h"

class GhostScreen
{
public:
    GhostScreen();       // 0x801d2b40
    void reset();        // 0x801d2bf0
    void captureScene(); // 0x801d2bfc
    void captureGhost(); // 0x801d2c78
    void draw();         // 0x801d2cc4
    
    void show() { mIsHide = false; }
    void hide() { mIsHide = true; }

    bool isHide() const { return mIsHide; }

private:
    u16 mStartX;
    u16 mStartY;
    u16 mEndX;
    u16 mEndY;
    bool mFlush;
    bool mIsHide;
    JUTTexture mSceneTex;
    JUTTexture mGhostTex;
};

#endif // GHOSTSCREEN_H
