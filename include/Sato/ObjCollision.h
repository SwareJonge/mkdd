#ifndef OBJCOLLISION_H
#define OBJCOLLISION_H

#include "JSystem/J3D/J3DAnmCluster.h"
#include "JSystem/J3D/J3DSkinDeform.h"
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

    ObjColBase(J3DModelData *, JGeometry::TVec3f, CKind);
    ObjColBase(f32, f32, CKind);
    void initRadius(J3DModelData *);
    void setScale(const JGeometry::TVec3f &);

    // Vtable
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32) = 0;
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &) = 0;

    // Inline/Unused
    void initialize();
    ObjColBase(J3DModelData *, CKind);
    ObjColBase(J3DModelData *, f32, CKind);
    void IsHitBoundPos(JGeometry::TVec3f, JGeometry::TVec3f);

    void setRadius(f32 rad) { mRadius = rad; }
    void setScale(f32 scale) { mScale = scale; }

    // VTBL _00
    f32 mRadius;            // _04
    CKind mKind;            // _08
    f32 mScale;             // _0C
    f32 mBoundDepth;        // _10
    JGeometry::TVec3f mPos; // _14
    u32 _20;                // _20, unused?
};

class ExObjColBase : public ObjColBase
{
public:
    void setWallParam(int);
    void setBottom(f32);

    // Vtable
    virtual void makeParameter(J3DModelData *, Mtx m) = 0;             // 10
    virtual void IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);     // 14
    virtual void IsHitBoundRadius(JGeometry::TVec3f, f32);             // 18
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f) = 0;     // 1C
    virtual bool IsOnDashPlane(JGeometry::TVec3f);                     // 20
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f) = 0; // 24
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

    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
    
    f32 mCylinderRadius; // _24
    f32 mCylinderHeight; // _28
};

class ObjColBoard // UNUSED
{
public:
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
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

    u8 _24[0x80 - 0x24];      // _24
    JGeometry::TVec3f _80[5]; // _80
}; // size: 0xbc

class ObjColBlock : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump1 : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump2 // UNUSED
{
public:
    void IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeParameter(J3DModelData *, Mtx m);
    void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeColParam(JGeometry::TVec3f, stPlaneParam, CrsGround::EAttr);
    void IsOnDashPlane(JGeometry::TVec3f);
    bool IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    bool IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

#endif // OBJCOLLISION_H
