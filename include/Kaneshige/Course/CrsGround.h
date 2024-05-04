#ifndef CRSGROUND_H
#define CRSGROUND_H

#include "types.h"

#include "JSystem/JGeometry.h"
#include "Kaneshige/Course/Course.h"

class GeographyObj; // forward declaration

class CrsGround
{
public:
    CrsGround(Course *course = nullptr);
    void reset();                                                                                                                                        // 0x801a1814
    void search(const JGeometry::TVec3f &);                                                                                                              // 0x801a18f0
    void search(const JGeometry::TVec3f &, const JGeometry::TVec3f &);                                                                                   // 0x801a1920
    void search(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &);                                                                   // 0x801a1950
    void searchBase(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, u32);                                                // 0x801a1974
    void search(const JGeometry::TVec3f &, const JGeometry::TVec3f &, f32, bool);                                                                      // 0x801a21e4
    void roundDepth(const JGeometry::TVec3f &);                                                                                                          // 0x801a262c
    void checkGridBySphere(f32 *, const CrsData::SColInfoSphere &, u32, f32, f32, f32);                                                // 0x801a2698
    void checkPolygonCover(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, f32, const CrsData::Ground *, f32 *);    // 0x801a290c
    void checkPolygonCoverMain(const JGeometry::TVec3f &, const CrsData::SColInfoSphere &, u32, f32, f32, f32, const CrsData::Ground *); // 0x801a2d20
    void isInsideGrid(const JGeometry::TVec3f &, f32, f32, f32, f32);                                                                            // 0x801a2ec0
    void searchGridIndex(int *, int *, Course *, const JGeometry::TVec3f &, f32, f32, f32, f32, int, int);                                       // 0x801a3004
    f32 getHeight() const;                                                                                                                              // 0x801a3168
    void getNormal(JGeometry::TVec3f *) const;                                                                                                           // 0x801a3170
    u8 getCameraCode() const;                                                                                                                            // 0x801a319c
    void isItemInvalGround() const;                                                                                                                      // 0x801a3204
    u8 getAddThickness() const;                                                                                                                          // 0x801a3268
    u8 getStaggerCode() const;                                                                                                                           // 0x801a32c0
    u8 getSpiralCode() const;                                                                                                                            // 0x801a3338
    u8 getSplashCode() const;                                                                                                                            // 0x801a338c
    f32 getSplashHeight();                                                                                                                               // 0x801a33e4
    void getSplashID();                                                                                                                                  // 0x801a3520
    void getJugemPoint();                                                                                                                                // 0x801a35ec
    void getCannonPoint();                                                                                                                               // 0x801a36d4
    JugemPoint *getPipePoint();                                                                                                                                 // 0x801a3728
    void getGeoCannon();                                                                                                                                 // 0x801a377c
    f32 getWaterHeight();                                                                                                                               // 0x801a37e8
    void isShaking() const;                                                                                                                              // 0x801a3848
    void isAttributeCollision();                                                                                                                         // 0x801a38a8
    void getWallNormal(JGeometry::TVec3f *, JGeometry::TVec3f *);                                                                                        // 0x801a38f4
    void addPullerVelocity();                                                                                                                            // 0x801a3e9c

    // Inlines
    void getVelocity(JGeometry::TVec3f *dest) const { dest->set(mVelocity); }
    int getMaterial() const { return mMaterial; }
    int getAttrIndex() const { return mAttrIdx; }
    int getAttribute() const { return mAttribute; }

    enum EAttr
    {
        // TODO
    };

    enum EMat
    {
        // TODO
    };

private:
    Course *mCourse;
    JGeometry::TVec3f _0x4;
    JGeometry::TVec3f _0x10;
    bool _0x1c;
    u8 _0x1d;
    void *mGround;
    void *mGeoObj;
    JGeometry::TVec3f mNormal;
    JGeometry::TVec3f mVelocity;
    JGeometry::TVec3f _0x40;
    int mAttribute;
    int mAttrIdx;
    int mMaterial;
    f32 mHeight;
    f32 _0x5C;
};

#endif