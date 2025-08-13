#ifndef CRSGROUND_H
#define CRSGROUND_H

#include "Kaneshige/Course/CrsData.h"
#include "types.h"

#include "JSystem/JGeometry.h"
#include "Kaneshige/Course/Course.h"

class GeoCannon;
class GeographyObj; // forward declaration

class CrsGround
{
public:
    enum EAttr
    {
        Attr_0,
        Attr_1,
        Attr_2,
        Attr_3,
        Attr_4,
        Attr_5,
        Attr_6,
        Attr_7,
        Attr_8,
        Attr_Cannon,
        Attr_10,
        Attr_11,
        Attr_12,
        Attr_Pipe,
        Attr_14,
        Attr_15,
        Attr_Puller,
        Attr_Water,
        Attr_18,
        Attr_19,
        Attr_55 = 0x37,
        Attr_71 = 0x47,
        Attr_255 = 0xff,
    };

    enum EMat
    {
        // TODO
    };

    CrsGround(Course *course = nullptr);
    void reset();                                                                                                                                 // 0x801a1814
    void search(const JGeometry::TVec3f &);                                                                                                       // 0x801a18f0
    void search(const JGeometry::TVec3f &, const JGeometry::TVec3f &);                                                                            // 0x801a1920
    void search(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &);                                                            // 0x801a1950
    void searchBase(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, u32);                                                   // 0x801a1974
    void search(const JGeometry::TVec3f &, const JGeometry::TVec3f &, f32, bool);                                                                 // 0x801a21e4
    void roundDepth(const JGeometry::TVec3f &);                                                                                                   // 0x801a262c
    void checkGridBySphere(f32 *, const CrsData::SColInfoSphere &, u32, f32, f32, f32);                                                           // 0x801a2698
    bool checkPolygonCover(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, f32, const CrsData::Ground *, f32 *); // 0x801a290c
    void checkPolygonCoverMain(const JGeometry::TVec3f &, const CrsData::SColInfoSphere &, u32, f32, f32, f32, const CrsData::Ground *);          // 0x801a2d20
    void isInsideGrid(const JGeometry::TVec3f &, f32, f32, f32, f32);                                                                             // 0x801a2ec0
    void searchGridIndex(int *, int *, Course *, const JGeometry::TVec3f &, f32, f32, f32, f32, int, int);                                        // 0x801a3004
    f32 getHeight() const;                                                                                                                        // 0x801a3168
    void getNormal(JGeometry::TVec3f *) const;                                                                                                    // 0x801a3170
    u8 getCameraCode() const;                                                                                                                     // 0x801a319c
    bool isItemInvalGround() const;                                                                                                               // 0x801a3204
    u8 getAddThickness() const;                                                                                                                   // 0x801a3268
    u8 getStaggerCode() const;                                                                                                                    // 0x801a32c0
    u8 getSpiralCode() const;                                                                                                                     // 0x801a3338
    u8 getSplashCode() const;                                                                                                                     // 0x801a338c
    f32 getSplashHeight();                                                                                                                        // 0x801a33e4
    int getSplashID();                                                                                                                           // 0x801a3520
    JugemPoint *getJugemPoint();                                                                                                                         // 0x801a35ec
    JugemPoint *getCannonPoint();                                                                                                                        // 0x801a36d4
    JugemPoint *getPipePoint();                                                                                                                   // 0x801a3728
    GeoCannon *getGeoCannon();                                                                                                                          // 0x801a377c
    f32 getWaterHeight();                                                                                                                         // 0x801a37e8
    bool isShaking() const;                                                                                                                       // 0x801a3848
    bool isAttributeCollision();                                                                                                                  // 0x801a38a8
    void getWallNormal(JGeometry::TVec3f *, JGeometry::TVec3f *);                                                                                 // 0x801a38f4
    void addPullerVelocity();                                                                                                                     // 0x801a3e9c

    // Inlines
    void getVelocity(JGeometry::TVec3f *dest) const { dest->set(mVelocity); }
    int getMaterial() const { return mMaterial; }
    u32 getAttrIndex() const { return mAttrIdx; }
    EAttr getAttribute() const { return mAttribute; }

    bool isObject() const { return mGeoObj != nullptr; }
    bool exceptValley(bool except) {
        mExceptVally = except;
    }

    static f32 getOverLevel() { return 50.0f; }

private:
    Course *mCourse;
    JGeometry::TVec3f mWorldPos;
    JGeometry::TVec3f _10;
    bool mExceptVally;
    u8 _1d;
    CrsData::Ground *mGround;
    void *mGeoObj;
    JGeometry::TVec3f mNormal;
    JGeometry::TVec3f mVelocity;
    JGeometry::TVec3f mWallNormal;
    EAttr mAttribute;
    u32 mAttrIdx;
    int mMaterial;
    f32 mHeight;
    f32 mDepth;
};

#endif
