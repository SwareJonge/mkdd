#ifndef SHADOWMODEL_H
#define SHADOWMODEL_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J3D/J3DAnmCluster.h"
#include "JSystem/J3D/J3DSkinDeform.h"
#include "Kaneshige/ViewCtrlModel.h"

class ShadowModel : public ViewCtrlModel
{
public:
    enum ShadowKind
    {
        cShadowKind_Kart,
        cShadowKind_Geo, /// based of GeographyObj::getShadowKind() const
        cShadowKind_2,
        cShadowKind_SkyShip // based of TMapObjSkyShip::getShadowKind() const
    };

    ShadowModel(u8 flags, ShadowKind kind, u8 p3) : ViewCtrlModel(flags)
    {
        mKind = kind;
        _94 = p3;
        _95 = 0;
    }

    ShadowKind getShadowKind() const { return mKind; }
    u8 getAlpha() const { return _94; }
    u8 getFlag95() const { return _95; }
    void setFlag95(bool flag) { _95 = flag; }

    virtual bool createModel(JKRSolidHeap *, u32, u32); // 0x801f7154, override
private:
    ShadowKind mKind;
    u8 _94;
    u8 _95;
};

class KartShadowModel : public ShadowModel
{
public:
    KartShadowModel() : ShadowModel(0xf, cShadowKind_Kart, 0xff) // 0x801a6e18
    {
        mScaleX = 1.0f;
        mScaleY = 1.0f;
    }

    virtual void calc();                                // 0x801f71d0
    virtual bool createModel(JKRSolidHeap *, u32, u32); // 0x801f732c, override

    void setScale(f32 x, f32 y)
    {
        mScaleX = x;
        mScaleY = y;
    }
    void setRoll(f32 roll) { mRoll = roll; }
    void setPosY(f32 y) { mPosY = y; }

    // Inline Functions
    void setAnmCluster(J3DAnmCluster *anmCluster) { mAnmCluster = anmCluster; }; // 0x801a738c
    void setDeformData(J3DDeformData *deformData) { mDeformData = deformData; }; // 0x801a7394

protected:
    J3DDeformData *mDeformData; // 0x98
    J3DAnmCluster *mAnmCluster; // 0x9c
    f32 mRoll;                  // 0xa0
    f32 mPosY;                  // 0xa4
    f32 mScaleX;                // 0xa8
    f32 mScaleY;                // 0xac
};

#endif
