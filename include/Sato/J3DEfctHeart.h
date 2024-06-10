#ifndef J3DEFCTHEART_H
#define J3DEFCTHEART_H

#include <JSystem/JParticle/JPAEmitter.h>
#include "Sato/J3DEffectMgr.h"

class ItemObj;
class ItemHeart;

class JPTraceHParticleCallBack
{
public:
    void execute(JPABaseEmitter *, JPABaseParticle *);
    void draw(JPABaseEmitter *, JPABaseParticle *);
    virtual ~JPTraceHParticleCallBack() {} // 0x80288ba4
};

class J3DEfctHeart : public J3DEffectObj
{
public:
    J3DEfctHeart();                                                             // 0x80287b74
    void getBmdFileName();                                                      // 0x80287be8
    void onlyinit();                                                            // 0x80287bf4
    void init();                                                                // 0x80287ef8
    void calc();                                                                // 0x80287f94
    void initFunc_Rolling();                                                    // 0x802880a8
    void doFunc_Rolling();                                                      // 0x80288144
    void initFunc_MoveToHand();                                                 // 0x802883e4
    void doFunc_MoveToHand();                                                   // 0x802883e8
    void initFunc_ColItemDisappear();                                           // 0x80288524
    void doFunc_ColItemDisappear();                                             // 0x80288528
    void initFunc_Disappear();                                                  // 0x802886c0
    void doFunc_Disappear();                                                    // 0x8028872c
    void update();                                                              // 0x80288730
    void setHitStartPos(const ItemObj &);                                       // 0x80288760
    void initItemHit(const ItemObj &);                                          // 0x80288864
    void setMoveEfct(ItemObj *, ItemHeart *);                                   // 0x80288998
    void doPosLerp(JGeometry::TVec3f *, const JGeometry::TVec3f &); // 0x80288a3c
    // Inline/Unused
    // void StateObserverT<J3DEfctHeart>::~StateObserverT();
    // Inline
    virtual ~J3DEfctHeart(); // 0x80288b24

    void set_dc(const u8 &val)
    {
        _dc = val;
    }

    u8 _d8[0xdc - 0xd8];    //
    u8 _dc;                 //
    u8 _dd;                 //
    u8 _de;                 //
    u8 _df[0xec - 0xdf];    //
    u32 _ec;                //
    u8 _f0;                 //
    u8 _f1[0xf4 - 0xf1];    //
    u8 _f4;                 //
    u8 _f5[0x158 - 0xf5];   //
    ItemHeart *mHeartLink;  // 158
    u8 _15c[0x164 - 0x15c]; //
}; // 0x164

#endif // J3DEFCTHEART_H