#ifndef ANMCONTROLLER_H
#define ANMCONTROLLER_H

#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/ExModel.h"
#include "Sato/J3DAnmObject.h"
#include "types.h"

struct MatAnmInfo
{
    u8 mNumBpks; // 0
    u8 mNumBtks; // 1
    u8 mNumBtps; // 2
    u8 mNumBrks; // 3
};

class AnmControlBase
{
public:
    AnmControlBase() : mFlags(0), mMaxAnmNo(0), mNowAnmNo(0), mLink(this) {}

    virtual void initAnm(u8, ExModel *) = 0;
    virtual void resetAnm() = 0;
    virtual void resetMatAnm() = 0;
    virtual void changeAnm(u8, bool) {}
    virtual void changeMatAnm(u8, bool) {}
    virtual void setAnmProc() = 0;
    virtual void frameProc() = 0;

    u8 getNowAnmNo() const { return mNowAnmNo; }

    bool tstFlgAnmStop() const { return mFlags & 1; }
    bool tstFlgAnm2() const { return mFlags & 2; }
    void resetFlag() { mFlags &= ~1; }

    u8 mFlags;                     // 4
    u8 mMaxAnmNo;                  // 5
    u8 mNowAnmNo;                  // 6
    JSULink<AnmControlBase> mLink; // 8
};

template <typename T>
class TAnmControlBase : public AnmControlBase
{
public:
    TAnmControlBase() { mObj = nullptr; }

    virtual void initAnm(u8 anm_no, ExModel *model) // 08
    {
        mMaxAnmNo = anm_no;

        mObj = new T[mMaxAnmNo];

        for (u8 i = 0; i < mMaxAnmNo; i++)
        {
            mObj[i].setExModel(model);
            mObj[i].resetFrame();
        }
        mFlags = 0;
    }
    virtual void resetAnm() // 0C
    {
        changeAnm(0, true);
        mObj->resetFrame();
        mFlags = 0;
    }
    virtual void resetMatAnm() // 10
    {
        changeMatAnm(0, true);
        mFlags = 0;
    }
    virtual void frameProc() { doframe(mNowAnmNo); }          // 18
    virtual void setAnmProc() { mObj[mNowAnmNo].anmFrameProc(); } // 14
    
    virtual ~TAnmControlBase() {}                             // 1C

    void doframe(const u8 anmNo)
    {
#line 111
        JUT_MINMAX_ASSERT(0, anmNo, mMaxAnmNo);
        if (!tstFlgAnmStop())
            mObj[anmNo].frameProc();
    }

    T *getObj(u8 anm_no) // fabricated
    {
#line 122
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        return &mObj[anm_no];
    }

    J3DFrameCtrl *getFrameCtrl(u8 anm_no)
    {
#line 127
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        return mObj[anm_no].getFrameCtrl();
    }

protected:
    T *mObj;
};

class AnmControlTrans : public TAnmControlBase<J3DAnmObjTrans>
{
public:
    AnmControlTrans() {}
    virtual ~AnmControlTrans() {}

    virtual void changeAnm(u8 anm_no, bool reset) { 
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        mNowAnmNo = anm_no;
        if (reset) {
            mObj[mNowAnmNo].resetFrame();
        }
    }

    virtual void setAnmProc() {
        mObj[mNowAnmNo].mModel->getModelData()->getJointNodePointer(0)->setMtxCalc(mObj[mNowAnmNo].mCalc);
        mObj[mNowAnmNo].anmFrameProc();
        if (mFlags & 2) {
            mObj[_1c].anmFrameProc();
        }
    }

    virtual void frameProc();

    void registration(u8, J3DAnmTransform *, J3DMtxCalc *);
    void registrationBlend(u8, J3DAnmTransform *, J3DMtxCalc *);
    void changeBlendAnm(u8, u8, u8);

    u8 _1c;
    u8 _1d;
    u8 _1e;
    u8 _1f;
};

class AnmControlMatColor : public TAnmControlBase<J3DAnmObjMatColor> {
public:
    AnmControlMatColor() { }

    virtual void changeMatAnm(u8 anm_no, bool reset) {
#line 190
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        mObj[anm_no].changeMatAnm(&mObj[mNowAnmNo]);
        mNowAnmNo = anm_no;
        if (reset) {
            mObj[mNowAnmNo].resetFrame();
        }
    }
};

class AnmControlMatTexSRT : public TAnmControlBase<J3DAnmObjMatTexSRT> {
public:
    AnmControlMatTexSRT() { }

    virtual void changeMatAnm(u8 anm_no, bool reset) {
#line 206
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        mObj[anm_no].changeMatAnm(&mObj[mNowAnmNo]);
        mNowAnmNo = anm_no;
        if (reset) {
            mObj[mNowAnmNo].resetFrame();
        }
    }
};

class AnmControlMatTexPattern : public TAnmControlBase<J3DAnmObjMatTexPattern> {
public:
    AnmControlMatTexPattern() {}

    virtual void changeMatAnm(u8 anm_no, bool reset) {
#line 223
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        mObj[anm_no].changeMatAnm(&mObj[mNowAnmNo]);
        mNowAnmNo = anm_no;
        if (reset) {
            mObj[mNowAnmNo].resetFrame();
        }
    }
};

class AnmControlMatTevReg : public TAnmControlBase<J3DAnmObjMatTevReg> {
public:
    AnmControlMatTevReg() {

    }
    virtual void changeMatAnm(u8 anm_no, bool reset) {
#line 246
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        mObj[anm_no].changeMatAnm(&mObj[mNowAnmNo]);
        mNowAnmNo = anm_no;
        if (reset) {
            mObj[mNowAnmNo].resetFrame();
        }
    }
};

class MatAnmData {
public:
    MatAnmData() {
        mpCtrlColor = nullptr;
        mpCtrlSRT = nullptr;
        mpCtrlPattern = nullptr;
        mpCtrlTevReg = nullptr;
    }

    AnmControlMatColor *mpCtrlColor;        // 0
    AnmControlMatTexSRT *mpCtrlSRT;         // 4
    AnmControlMatTexPattern *mpCtrlPattern; // 8
    AnmControlMatTevReg *mpCtrlTevReg;      // c
};

class AnmController
{
public:
    AnmController();                                         // 0x8026f25c
    void InitRegistrationMat(const MatAnmInfo &, ExModel *); // 0x8026f27c
    void FrameProc();                                        // 0x8026f858
    void SetAnimation();                                     // 0x8026f8d4
    void Reset();                                            // 0x8026f950
    void ResetMat();                                         // 0x8026f99c
    void ResetAnm() { // fabricated but causes TAnmControlBase<J3DAnmObjTrans>::resetAnm() to be ordered correctly in GeoCannon, also inlines into a matching AnmController::reset()
        if (mTrans != nullptr) {
            mTrans->resetAnm();
        }
    }

    // Inline/Unused
    void appendMaterial(AnmControlBase *);
    void ChangeMatAnmAll(u8);
    void FrameProcTrans();
    void FrameProcMat();
    void SetAnimationTrans();
    void SetAnimationMat();

    virtual ~AnmController() {}

    void InitRegistration(u8 anm_no, ExModel *model) {
        mTrans = new AnmControlTrans();
        mTrans->initAnm(anm_no, model);
    }

    void RegisterTransBlend(u8 anm_no, J3DAnmTransform *transform, J3DMtxCalc *mtxCalc) {
        mTrans->registrationBlend(anm_no, transform, mtxCalc);
    }

    void ChangeTransAnm(u8 AnmNo) { // fabricated
        if (mTrans == nullptr) {
            return;
        }
#line 338
        JUT_MINMAX_ASSERT(0, AnmNo, mTrans->mMaxAnmNo);
        J3DAnmObjTrans *transObj = mTrans->getObj(AnmNo);
        if (transObj->_28 & 1) {
            transObj->setWeight(0, 1.0f);
            for(u8 i = 1; i < 4; i++) {
                transObj->setWeight(i, 0.0f);
            }
        }
        mTrans->changeAnm(AnmNo, true); //  TODO: additional parameter?
    }

    void ChangeBlendTransAnm(u8 AnmNo, u8 p2, u8 p3) {
        if (mTrans == nullptr)
            return;
#line 353
        JUT_MINMAX_ASSERT(0, AnmNo, mTrans->mMaxAnmNo);
        mTrans->changeBlendAnm(AnmNo, p2, p3);
    }
    u8 getNowTransNo() const { return mTrans->getNowAnmNo();  }
    J3DAnmObjTrans *getObj(u8 AnmNo) { return mTrans->getObj(AnmNo); }
    J3DFrameCtrl *getCurFrameCtrl() { return mTrans->getObj(getNowTransNo())->getFrameCtrl(); }
    J3DFrameCtrl *getFrameCtrl(u8 AnmNo) { return mTrans->getFrameCtrl(AnmNo);  }
    
    void ChangeTransAnm(u8 AnmNo, bool p2) { mTrans->changeAnm(AnmNo, p2); } // fabricated
    void RegisterTrans(u8 AnmNo, J3DAnmTransform *transform, J3DMtxCalc *calc) { mTrans->registration(AnmNo, transform, calc); } // fabricated
    bool IsAvailableTrans() const { return mTrans != nullptr; }
    void StopTrans() { mTrans->resetFlag(); } // fabricated
    bool IsTransAnm2() const { return mTrans->tstFlgAnm2(); }

    AnmControlTrans *mTrans;     // 4
    AnmControlBase **mMat;
    JSUList<AnmControlBase> *mCtrls;
    MatAnmData *mMatData;

}; // Size: 0x14

#endif // ANMCONTROLLER_H
