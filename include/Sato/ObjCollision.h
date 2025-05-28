#ifndef OBJCOLLISION_H
#define OBJCOLLISION_H

#include "Kaneshige/Course/CrsGround.h"
#include "Sato/stMath.h"

class ObjColCylinder;

class ObjColBase
{ 
public:
    enum CKind
    {
        SPHERE,
        CYLINDER,
        CUBE,
    };

    void initialize();
    ObjColBase(J3DModelData *, CKind);
    ObjColBase(J3DModelData *, f32, CKind);
    ObjColBase(J3DModelData *, JGeometry::TVec3f, CKind);
    ObjColBase(f32, f32, CKind);
    void initRadius(J3DModelData *);
    void setScale(const JGeometry::TVec3f &);

    // Vtable
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32) = 0;
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &) = 0;

    u8 IsHitBoundPos(JGeometry::TVec3f, JGeometry::TVec3f);

    void setRadius(f32 rad) { mRadius = rad; }
    void setScale(f32 scale) { mScale = scale; }

    f32 getRadius() const { return mRadius; }
    f32 getScale() const { return mScale; }

// private:
    // VTBL _00
    f32 mRadius;            // _04
    CKind mKind;            // _08
    f32 mScale;             // _0C
    f32 mBoundDepth;        // _10
    JGeometry::TVec3f mPos; // _14
    u32 _20;                // _20, unused?
};

class ObjColSphere : public ObjColBase
{
public:
    ObjColSphere(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, SPHERE) {}
    ObjColSphere(f32 radius, f32 scale) : ObjColBase(radius, scale, SPHERE) {}
    
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColCylinder : public ObjColBase
{
public:
    ObjColCylinder(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, CYLINDER), mCylinderRadius(0.0f), mCylinderHeight(0.0f) {}
    ObjColCylinder(f32 radius, f32 scale) : ObjColBase(radius, scale, CYLINDER), mCylinderRadius(0.0f), mCylinderHeight(0.0f) {}
    
    void setCylinderRadius(f32 radius) { mCylinderRadius = radius; }
    void setCylinderHeight(f32 height) { mCylinderHeight = height; }

    f32 getScaledHeight() const { return mScale * mCylinderHeight; }

    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
    
    f32 mCylinderRadius; // _24
    f32 mCylinderHeight; // _28
};

class ObjColCube : public ObjColBase
{
public:
    ObjColCube(J3DModelData *modelData, JGeometry::TVec3f scale) : ObjColBase(modelData, scale, CUBE) {}

    void makeParameter(J3DModelData *, Mtx m);
    void updateParameter(Mtx m);
    bool chkIsHitQuad(const JGeometry::TVec3f &, const f32 &);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);

    f32 mDimensions[3];             // _24 - edge lengths
    stPlaneParam mSidePlanes[4];    // _30
    stPlaneParam mTopPlane;         // _70
    JGeometry::TVec3f mVertices[5]; // _80
}; // size: 0xbc

class ExObjColBase : public ObjColBase
{
public:
    void setBottom(f32);
    void setWallParam(int);

    // Vtable
    virtual void makeParameter(J3DModelData *, Mtx m) = 0;             // 10
    virtual bool IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);     // 14
    virtual u8 IsHitBoundRadius(JGeometry::TVec3f, f32);               // 18
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f) = 0;     // 1C
    virtual u8 IsOnDashPlane(JGeometry::TVec3f)                        // 20
    {
        return mIsDashPlane;
    }

    JGeometry::TVec3f getNormal() const {
        return mNormal;
    }
    JGeometry::TVec3f getWallNormal() const {
        return mWallNormal;
    }
    JGeometry::TVec3f getVelocity() {
        return mVelocity;
    }
    f32 getHeight() const {
        return mHeight;
    }
    u32 getAttr() const {
        return mAttr;
    }
    u32 getMaterial() const {
        return mMaterial;
    }
    f32 getColDepth() const {
        return mColDepth;
    }
    u32 getAttrIndex() const {
        return mAttrIndex;
    }
    
    stPlaneParam mSidePlanes[4];   // _24
    stPlaneParam mTopPlane;        // _64
    stPlaneParam mBottomPlane;     // _74
    JGeometry::TVec3f mNormal;     // _84
    JGeometry::TVec3f mWallNormal; // _90
    JGeometry::TVec3f mDirection;  // _9c
    JGeometry::TVec3f mVelocity;   // _a8
    f32 mHeight;                   // _b4
    f32 mBottom;                   // _b8
    u32 mAttr;                     // _bc
    u32 mMaterial;                 // _c0
    s32 mHitNum;                   // _c4
    f32 mColDepth;                 // _c8
    u8 mIsDashPlane;               // _cc, name is just a guess
    u32 mAttrIndex;                // _d0
};

class ObjColJump1 : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    f32 SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump2 : public ExObjColBase // UNUSED
{
public:
    virtual bool IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void makeColParam(JGeometry::TVec3f, stPlaneParam, CrsGround::EAttr);
    virtual u8 IsOnDashPlane(JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColBlock : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual f32 SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColBoard : public ExObjColBase // UNUSED
{
public:
    f32 SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
};

#endif // OBJCOLLISION_H
