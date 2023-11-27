#ifndef STEMITTER_H
#define STEMITTER_H

#include "PowerPC_EABI_Support/Runtime/MWCPlusPlusLib.h"
#include <JSystem/JSupport/JSUList.h>
#include <JSystem/JGeometry.h>

struct stEmitter
{
    stEmitter(); // 0x802359b4
    u8 _0[0xd8];
}; // Size: 0xd8

struct stEmitterParamLineIP
{
    enum cIpParam
    {
        Life,
        CreateRate,
        RateStep,
        FirstVel,
        Size,
        SizeFirst,
        SizeLast,
        LifeRndRate,
        FirstVelRndAngle,
        AlphaStdRate,
        Gravity,
        AirFric
    };

    void Init(u8 funcCnt, stEmitter *, stEmitter *);
    void AddFunc(cIpParam);
    void DoIp(f32);
    void doLife();
    void doCreateRate();
    void doRateStep();
    void doFirstVel();
    void doSize();
    void doSizeFirst();
    void doSizeLast();
    void doLifeRndRate();
    void doFirstVelRndAngle();
    void doAlphaStdRate();
    void doGravity();
    void doAirFric();

    stEmitter *_00;
    stEmitter *_04;
    stEmitter *_08;
    PTMF *funcList;
    f32 scalar;
    u8 funcCnt;
}; // Size: Unknown, probably 0x18

struct stEmitterPtcl : public stEmitter
{
    stEmitterPtcl() {} // TODO
    ~stEmitterPtcl() {}
    // Inline/Unused
    void update();
    void removeAllParticles();

    // stParticleMgr particle;
    // 12c;
    // JSUPtrLink some link
    // s16 _147
}; // Size: 0x148

struct stEmitterCallBack
{
    struct stFileParamPtrList {
        stEmitter *_00;
        stEmitter *_04;
        stEmitter *_08;
    }; // Size: 0xC

    stEmitterCallBack() {
        _04 = nullptr;
        kartNum = -1;
        _22 = 0;
        _24 = 0;
        _25 = 0;
    };
    ~stEmitterCallBack() {}

    void setIpData(stFileParamPtrList *list);
    int getTirePos(JGeometry::TVec3f *tirePos);
    int getDriftPos(JGeometry::TVec3f *driftPos, const JGeometry::TVec3f &origin);
    void calc_lineIp(f32, f32, f32, f32, f32, bool, f32);
    void calc_lineIpFollow(f32, f32, f32, f32, f32, bool, f32);
    void calcParam(f32, f32, f32, f32);
    f32 getLerpLineRate(const f32 &, const f32 &, const f32 &, const f32 &);

    virtual void init() = 0;
    virtual void calc() = 0;
    virtual void strategy(f32 ratio, f32 max, f32 min, f32 dif, bool, f32);
    virtual int getGeneratePos(JGeometry::TVec3f *, Mtx);

    // Unused/Inlined
    bool chkAvailableDistance();

    // _00 Vtable
    stEmitterPtcl *_04;
    stEmitterParamLineIP ipData;
    short kartNum;
    short _22;
    u8 _24;
    u8 _25;
}; // Size: 0x28(padded)

struct stEmitterPtclMgr
{
    void calc();
    void draw(u32);
    void Create(stEmitterCallBack *, u16);
    void RemoveEmt(JSUList<stEmitterPtcl> &, JSUListIterator<stEmitterPtcl> &);
    void ChkRemoveBecomeInvalidEmt(JSUList<stEmitterPtcl> &, JSUListIterator<stEmitterPtcl> &);
    void removeAllEmt();
    void Remove(stEmitterPtcl *);
};

#endif // STEMITTER_H
