#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Kawano/driver.h"

class AnimatorWind : public NullAnimator
{
    public:
        void start();
        void stop();
        void reset();
        // update() is inherited from base - calls external function
        
    protected:
        int mFrameCounter;
        int mAnimationState;
};

class LoopAnimatorWind : public AnimatorWind
{
    public:
        void update(); // Main update function called each frame - overrides base
        
    protected:
        float mMinValue;            // offset 0x1C - minimum value for animation range
        float mMaxValue;            // offset 0x20 - maximum value for animation range
        float mSpeed;               // offset 0x24 - animation speed/step size
        float mCurrentPhase;        // offset 0x28 - current phase in animation cycle
};

#endif
