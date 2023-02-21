#ifndef EFFECTSCREEN_H
#define EFFECTSCREEN_H

class EffectScreenMgr
{
public:
    EffectScreenMgr();
    void reset();
    static void createMgr() {
        ThisMgr = new EffectScreenMgr();
    }

    static EffectScreenMgr * getThis() {
        return ThisMgr;
    }

    static EffectScreenMgr *ThisMgr;
    u8 _00[0xd0];
};

inline void CreateEfctScreenMgr() {
    EffectScreenMgr::createMgr();
}

inline EffectScreenMgr * GetEfctScreenMgr() {
    return EffectScreenMgr::getThis();
}

#endif