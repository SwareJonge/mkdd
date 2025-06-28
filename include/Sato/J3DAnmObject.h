#ifndef SATO_J3DANMOBJECT_H
#define SATO_J3DANMOBJECT_H

#include "JSystem/J3D/J3DAnmBase.h"
#include "JSystem/J3D/J3DAnmColor.h"
#include "JSystem/J3D/J3DAnmTransform.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/J3D/J3DMtxCalc.h"
#include "JSystem/JUtility/JUTAssert.h"
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

    J3DModelData *getModelData() { return mModel->getModelData(); }

    virtual ~J3DAnmObjBase() {}
    virtual void anmFrameProc() = 0;

    ExModel *mModel;         // 04
    J3DFrameCtrl mFrameCtrl; // 08
    J3DAnmBase *mAnmBase;    // 1c
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
    void attach(T *anm) {
        mAnmBase = anm;
        initFrameCtrl(mAnmBase);
    }

    J3DAnmBase *getAnmBase() { return mAnmBase; }

}; // Size: 0x20

template <class T>
class J3DAnmObjMaterialBase : public J3DAnmObjBase {
public:
    J3DAnmObjMaterialBase() {}
    virtual void anmFrameProc() {
        mAnmBase->setFrame(mFrameCtrl.getFrame());
    }
};

class J3DAnmObjMatTexPattern : public J3DAnmObjMaterialBase<J3DAnmTexPattern> {
public:
    J3DAnmObjMatTexPattern() {}
    virtual ~J3DAnmObjMatTexPattern() {}

    void changeMatAnm(J3DAnmObjMatTexPattern *nowanm) {
#line 200
        JUT_ASSERT(nowanm->getModelData() == getModelData());
        getModelData()->getMaterialTable().removeTexNoAnimator((J3DAnmTexPattern*)nowanm->mAnmBase);
        getModelData()->getMaterialTable().entryTexNoAnimator((J3DAnmTexPattern*)mAnmBase);
    }
}; // Size: 0x20

class J3DAnmObjMatColor : public J3DAnmObjMaterialBase<J3DAnmColor> {
public:
    J3DAnmObjMatColor() {}
    virtual ~J3DAnmObjMatColor() {}

    void changeMatAnm(J3DAnmObjMatColor *nowanm) {
#line 213
        JUT_ASSERT(nowanm->getModelData() == getModelData());
        getModelData()->getMaterialTable().removeMatColorAnimator((J3DAnmColor*)nowanm->mAnmBase);
        getModelData()->getMaterialTable().entryMatColorAnimator((J3DAnmColor*)mAnmBase);
    }
}; // Size: 0x20

class J3DAnmObjMatTexSRT : public J3DAnmObjMaterialBase<J3DAnmTextureSRTKey> {
public:
    J3DAnmObjMatTexSRT() {}
    virtual ~J3DAnmObjMatTexSRT() {}

    void changeMatAnm(J3DAnmObjMatTexSRT *nowanm) {
#line 226
        JUT_ASSERT(nowanm->getModelData() == getModelData());
        getModelData()->getMaterialTable().removeTexMtxAnimator((J3DAnmTextureSRTKey*)nowanm->mAnmBase);
        getModelData()->getMaterialTable().entryTexMtxAnimator((J3DAnmTextureSRTKey*)mAnmBase);
    }
}; // Size: 0x20

class J3DAnmObjMatTevReg : public J3DAnmObjMaterialBase<J3DAnmTevRegKey> {
public:
    J3DAnmObjMatTevReg() {}
    virtual ~J3DAnmObjMatTevReg() {}

    void changeMatAnm(J3DAnmObjMatTevReg *nowanm) {
#line 239
        JUT_ASSERT(nowanm->getModelData() == getModelData());
        getModelData()->getMaterialTable().removeTevRegAnimator((J3DAnmTevRegKey*)nowanm->mAnmBase);
        getModelData()->getMaterialTable().entryTevRegAnimator((J3DAnmTevRegKey*)mAnmBase);
    }
}; // Size: 0x20

class J3DAnmObjTrans : public J3DAnmObjBase
{
public:
    // TODO
    J3DAnmObjTrans() {
        _24 = 0;
        _28 = 0;
    }
    virtual ~J3DAnmObjTrans() {}
    virtual void anmFrameProc();

    void attach(J3DAnmTransform *, J3DMtxCalc *);
    void attachBlend(J3DAnmTransform *, J3DMtxCalc *);
    static void setupTransAnmData(J3DAnmTransform **, J3DMtxCalc **, J3DModelData *, void *);
    static void loadTransAnm(J3DAnmTransform **paTrans, void *pData);

    void setWeight(u8 idx, f32 w) {
        mCalc->setWeight(idx, w);
    }

    //private:
    J3DMtxCalc *mCalc; // 20
    u32 _24;
    u8 _28;
};

#endif
