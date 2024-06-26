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

    };

    ObjColBase(J3DModelData *, JGeometry::TVec3f, CKind);
    ObjColBase(f32, f32, CKind);
    void initRadius(J3DModelData *);
    void setScale(const JGeometry::TVec3f &);

    // Vtable
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32) = 0;
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &) = 0;

    // Inline/Unused
    void initialize();
    ObjColBase(J3DModelData *, CKind);
    ObjColBase(J3DModelData *, f32, CKind);
    void IsHitBoundPos(JGeometry::TVec3f, JGeometry::TVec3f);

    void setRadius(f32 rad) { mRadius = rad; }
    void setScale(f32 scale) { mScale = scale; }

    f32 mRadius;
    CKind mKind;
    f32 mScale;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1c;
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

class ObjColCylinder : public ObjColBase
{
public:
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
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
    void makeParameter(J3DModelData *, Mtx m);
    void updateParameter(Mtx m);
    void chkIsHitQuad(const JGeometry::TVec3f &, const f32 &);
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColSphere : public ObjColBase
{
public:
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColBlock : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump1 : public ExObjColBase
{
public:
    virtual void makeParameter(J3DModelData *, Mtx m);
    virtual void SearchWall(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    virtual void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    virtual void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

class ObjColJump2 // UNUSED
{
public:
    void IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeParameter(J3DModelData *, Mtx m);
    void Search(JGeometry::TVec3f, JGeometry::TVec3f);
    void makeColParam(JGeometry::TVec3f, stPlaneParam, CrsGround::EAttr);
    void IsOnDashPlane(JGeometry::TVec3f);
    void IsHitSphere(JGeometry::TVec3f, JGeometry::TVec3f, f32);
    void IsHitCylinder(JGeometry::TVec3f, JGeometry::TVec3f, const ObjColCylinder &);
};

#endif // OBJCOLLISION_H