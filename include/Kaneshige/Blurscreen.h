#ifndef BLURSCREEN_H
#define BLURSCREEN_H

#include <JSystem/JUtility/JUTTexture.h>
#include "types.h"

class BlurScreen
{
public:
    BlurScreen();                                                                    // 0x801ccf74
    void draw(const JUtility::TColor &color, u8, u8, f32, f32); // 0x801cd030
    void captureScene();                                                             // 0x801cd46c
    void captureBlur();                                                              // 0x801cd4b8
private:
    u16 mStartX;
    u16 mStartY;
    u16 mEndX;
    u16 mEndY;
    bool mFlush;
    bool mFlushBlur;
    JUTTexture mBlurTex;
    JUTTexture mSceneTex;
};

#endif // BLURSCREEN_H