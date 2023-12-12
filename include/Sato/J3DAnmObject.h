#ifndef SATO_J3DANMOBJECT_H
#define SATO_J3DANMOBJECT_H

#include "JSystem/J3D/J3DAnmBase.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
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
    // TODO
};

#endif