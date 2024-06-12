#ifndef ANMCONTROLLER_H
#define ANMCONTROLLER_H

#include "Kaneshige/ExModel.h"
#include "Sato/J3DAnmObject.h"

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

    bool tstFlgAnmStop() const { mFlags & 1; }

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
        u8 max = mMaxAnmNo;

        mObj = new T[anm_no];

        for (u8 i = 0; i < max; i++)
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
        changeAnm(0, true);
        mFlags = 0;
    }
    virtual void setAnmProc() { mObj[mNowAnmNo].resetAnm(); } // 14
    virtual void frameProc() { doframe(mNowAnmNo); }          // 18
    virtual ~TAnmControlBase() {}                             // 1C

    void doframe(u8 anmNo)
    {
#line 111
        JUT_MINMAX_ASSERT(0, anmNo, mMaxAnmNo);
        if (!tstFlgAnmStop())
            mObj[anmNo].frameProc();
    }

    J3DFrameCtrl *getFrameCtrl(u8 anm_no)
    {
#line 127
        JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
        return mObj[anm_no].getFrameCtrl();
    }

private:
    T *mObj;
};

class AnmControlTrans : public TAnmControlBase<J3DAnmObjTrans>
{
public:
    AnmControlTrans() {}

    void registration(u8, J3DAnmTransform *, J3DMtxCalc *);
    void registrationBlend(u8, J3DAnmTransform *, J3DMtxCalc *);
    void changeBlendAnm(u8, u8, u8);
    void frameProc();
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
    // Inline/Unused
    void appendMaterial(AnmControlBase *);
    void ChangeMatAnmAll(u8);
    void FrameProcTrans();
    void FrameProcMat();
    void SetAnimationTrans();
    void SetAnimationMat();

    virtual ~AnmController() {}

    void ChangeBlendTransAnm(u8 AnmNo, u8 p2, u8 p3)
    {
        if (mTrans == nullptr)
            return;
#line 353
        JUT_MINMAX_ASSERT(0, AnmNo, mTrans->mMaxAnmNo);
        mTrans->changeBlendAnm(AnmNo, p2, p3);
    }

    bool IsAvailableTrans() const { return mTrans != nullptr; }

    AnmControlTrans *mTrans; // 4
    void *_08;                   // AnmControlMatColor
    void *_0C;                   // some JSUPtrList
    void *_10;                   // AnmControlMatColor, copy of _08? or other way around

}; // Size: 0x14

#endif // ANMCONTROLLER_H