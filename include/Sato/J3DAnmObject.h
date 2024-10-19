#ifndef SATO_J3DANMOBJECT_H
#define SATO_J3DANMOBJECT_H

#include "JSystem/J3D/J3DAnmBase.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
#include "JSystem/J3D/J3DMtxCalc.h"
#include "Kaneshige/ExModel.h"

class J3DAnmObjBase
{
public:
    J3DAnmObjBase() { mModel = nullptr; }

    void initFrameCtrl(J3DAnmBase *base);
    void frameProc() { mFrameCtrl.update(); }
    void update() { mFrameCtrl.update(); }
    void resetFrame() { mFrameCtrl.reset(); }
    
    J3DFrameCtrl *getFrameCtrl() { return &mFrameCtrl; }
    void setExModel(ExModel *model) { mModel = model; }
    void setRate(const float &rate) { mFrameCtrl.setRate(rate); }
    void setFrame(float frame) { mFrameCtrl.setFrame(frame); }

    virtual ~J3DAnmObjBase() {}
    virtual void anmFrameProc() = 0;

    ExModel *mModel;
    J3DFrameCtrl mFrameCtrl;
    J3DAnmBase *mAnmBase;
}; // Size: 0x20

class J3DAnmObjMaterial : public J3DAnmObjBase
{
public:
    J3DAnmObjMaterial() { mAnmBase = nullptr; }

    virtual ~J3DAnmObjMaterial() {}
    virtual void anmFrameProc() { mAnmBase->setFrame(mFrameCtrl.getFrame()); }

    static void setupTexSRTAnmData(J3DAnmTextureSRTKey **, J3DModelData *, void *);
    static void setupTevRegAnmData(J3DAnmTevRegKey **, J3DModelData *, void *);
    static void setupTexPatternAnmData(J3DAnmTexPattern **, J3DModelData *, void *);

    template <typename T>
    void attach(T *anm)
    {
        mAnmBase = anm;
        initFrameCtrl(mAnmBase);
    }

    J3DAnmBase *getAnmBase() { return mAnmBase; }

}; // Size: 0x20

class J3DAnmObjTrans : public J3DAnmObjBase
{
public:
    // TODO
    J3DAnmObjTrans() {}
    virtual ~J3DAnmObjTrans() {}
    virtual void anmFrameProc();

    static void setupTransAnmData(J3DAnmTransform **, J3DMtxCalc **, J3DModelData *, void *);

//private:
    J3DMtxCalc *mCalc;
    u32 _24;
    u8 _28;
};

#endif
