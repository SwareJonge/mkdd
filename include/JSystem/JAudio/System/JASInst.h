#ifndef JSYSTEM_JASINST_H
#define JSYSTEM_JASINST_H

#include <JSystem/JMath/JMath.h>
#include <JSystem/JAudio/System/JASSoundParams.h>


// NOTE: none of these constructors have been tested
class JASInstEffect {
public:
    JASInstEffect() { mType = 0; }
    virtual void effect(int, int, JASSoundParams *params) const = 0;

protected:
    int mType;
};

class JASInstRand : public JASInstEffect
{
public:
    JASInstRand() : JASInstEffect() {
        mX = 1.0f;
        mY = 0.0f;
     }

    virtual void effect(int, int, JASSoundParams *params) const;

    // Fabricated
    f32 getY(f32 in, f32 rnd, f32 mult) const
    {
        return rnd - (mult * in);
    }

private:
    f32 mX;
    f32 mY;
};

class JASInstSense : public JASInstEffect {
public:
    JASInstSense() : JASInstEffect() {
        _08 = 0;
        _09 = 60;
        _0C = 1.0f;
        _10 = 1.0f;
    }

private:
    int _08;
    u8 _09;
    f32 _0C;
    f32 _10;

};

#endif
