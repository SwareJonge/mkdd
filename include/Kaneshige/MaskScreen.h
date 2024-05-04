#ifndef MASKSCREEN_H
#define MASKSCREEN_H

#include <JSystem/JUtility/JUTTexture.h>
#include "types.h"

#define NUM_NODES 20

class MaskScreenNode
{
public:
    MaskScreenNode();
    void reset();
    void set(f32, f32, bool);
    void capture();
    void draw();

    bool isActive() const { return mActive; }

private:
    bool _0;
    s16 mStartX;
    s16 mStartY;
    s16 mEndX;
    s16 mEndY;
    bool mFlush;
    bool mActive;
    JUTTexture *mTex;
};

class MaskScreen
{
public:
    MaskScreen();
    void reset();
    void set(f32, f32, bool);
    void resetAllMask();
    void requestMask(f32, f32, bool);
    void capture();
    void draw();

private:
    u16 mStartX;
    u16 mStartY;
    u16 mEndX;
    u16 mEndY;
    MaskScreenNode *mNodes[NUM_NODES];
};

#endif