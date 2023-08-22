#ifndef CRSDATA_H
#define CRSDATA_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGeometry.h"
#include "Sato/stMath.h"
#include "kartLocale.h"
#include "types.h"

class Course; // Forward Declaration

class CrsData
{
public:
    struct SColHeader
    {
        char signature[4];
        u16 xGridNum;
        u16 zGridNum;
        int xStart; // TVec2?
        int zStart;
        int xSize;
        int zSize;
        s16 matNum;
        u32 triangleIndices;
        // Not sure of data type
        void **triangleList;
        void **vertexList;
        void **matList;
    };

    struct SColInfoSphere 
    {
        // TODO
    };

    struct SOblHeader
    {
        char mMagic[4];
        u8 mShaking;
        u8 mAmbientColor[3];
        JUtility::TColor mLightColor;
        JGeometry::TVec3f mLightOffsetPos;
        u8 mTotalLapNum;
        u8 mCourseID;
        u16 mCLPointNum;
        u16 mCheckPathNum;
        u16 ObjEntries;
        u16 mAreaNum;
        u16 mCameraNum;
        u16 mPathNum;
        u16 mJugemPointNum;
        u8 mFogType;
        u8 mFogColor[3];
        f32 mFogStartZ;
        f32 mFogEndZ;
        u8 mTexLodBiasOn;
        u8 mDummyStartLineOn;
        u8 mSnowEnabled;
        u8 mShadowDepth;
        u8 mShadowColor[3];
        u8 mStartPointNum;
        u8 mSkyFollow;
        u8 mLightParamNum;
        u8 mMGParamNum;
        u32 fileStart;
        u32 mCLPointOffset;
        u32 mCheckPathOffset;
        u32 mPathOffset;
        u32 mPointOffset;
        u32 objectsOffset;
        u32 mStartPointOffset;
        u32 mAreaOffset;
        u32 mCameraOffset;
        u32 mJugemPointOffset;
        u32 mLightParamOffset;
        u32 mMGParamOffset;
        u8 _70[0xc];
    };

    // https://mkdd.miraheze.org/wiki/BOL_(File_Format)#Section_1
    struct SCLPoint
    {
        //SCLPoint() : mPos() { }

        JGeometry::TVec3f mPos;
        u16 mDriftDir; 
        s16 mGroupID; 
        f32 mScale;
        char mSwerve;
        u8 mGroupSetting;
        u8 mUniqueGroupID;
        u8 mDriftAcuteness;
        u8 mDriftDuration;
        u8 mDriftSupplement;
        u8 mIsNoMushroomZone;
        u8 pad[5];
    }; // Size: 0x20

    struct SObject 
    {
        // TODO
    };

    struct SJugemPoint
    { // mostly copied from https://mkdd.miraheze.org/wiki/BOL_(File_Format) (Section 9: Respawn Points)
        JGeometry::TVec3f mPosition;
        s16 _c; // called by "getFrDirection" however according to the wiki this is an array of 3 integers?
        s16 _e;
        s16 _10;
        s16 _12[3]; // seems to be padding
        u16 mID;
        u16 mCPointID;
        s16 mCameraID;
        s16 mSectorID;
    };

    class Ground
    {
    public:
        bool checkPosition(stPlaneParam *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f *) const;                                                                     // 0x8019fb34
        void checkWallBySphere(JGeometry::TVec3f *, const SColInfoSphere &, const JGeometry::TVec3f *, float *, JGeometry::TVec3f *, JGeometry::TVec3f *) const;                                 // 0x801a0078
        void getNearPoint(const SColInfoSphere &, float, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, JGeometry::TVec3f *, JGeometry::TVec3f *) const; // 0x801a02fc
        void checkPolygonCoverWall(const JGeometry::TVec3f &, const SColInfoSphere &, const Ground *);                                                                                                             // 0x801a0590
        void checkFront(const CrsData *, const JGeometry::TVec3f &) const;                                                                                                                                         // 0x801a0770
        void checkFaceSide(const CrsData *, const JGeometry::TVec3f &) const;                                                                                                                                      // 0x801a083c
        void getPlaneY(const JGeometry::TVec3f &, Course *) const;                                                                                                                                                 // 0x801a097c
        void checkPolygonTouchBySphere(const JGeometry::TVec3f *, const JGeometry::TVec3f &, float, Course *) const;                                                                                         // 0x801a0ae4
        void getTouchState(const SColInfoSphere &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, JGeometry::TVec3f *, float *, float) const;                                                   // 0x801a0c5c
        void getCenter(JGeometry::TVec3f *, const JGeometry::TVec3f *, Course *) const;                                                                                                                      // 0x801a0f64
        // Inline
        u8 getZmaxIndex() const;    // 0x801a0034
        u8 getXmaxIndex() const;    // 0x801a0040
        u8 getZminIndex() const;    // 0x801a004c
        u8 getXminIndex() const;    // 0x801a0058
        u8 getAddThickness() const; // 0x801a0064
    private:
        // TODO
    };

    class StartPoint {
    public:
        void getLinePosition(JGeometry::TVec3f *) const; // 0x801a1260

        bool isRight() const { return mOrientation != 0; };
        f32 getJugemOffsetY() const { return mJugemOffsetY; };

        void getPosition(JGeometry::TVec3f *dst) const
        {
            dst->set(mPos);
        }
        void getFrDirection(JGeometry::TVec3f *vec) const
        {
            vec->x = mRot.x * 1.0E-4f;
            vec->y = 0.0f;
            vec->z = mRot.z * 1.0E-4f;
            vec->normalize();
        }

    private:
        JGeometry::TVec3f mPos;
        JGeometry::TVec3f mScale;
        JGeometry::TVec3<s16> mRot;
        u8 _1E[0x24 - 0x1E];
        u8 mOrientation;
        s16 mJugemOffsetY; // could be TVec2/3
    };

    CrsData(SColHeader *, SOblHeader *);
    void getAmbientColor(JUtility::TColor *) const;
    void getLightColor(JUtility::TColor *) const;
    void getShadowColor(JUtility::TColor *) const;
    void getLightOffsetPosition(JGeometry::TVec3f *) const;
    void patchCamDataForStaffRoll(Language, VideoMode);
    StartPoint* getStartPoint(u8) const;
    int getCourseID() const;
    u8 getTotalLapNumber() const { return BOL->mTotalLapNum; };
    bool isTexLODBiasOn() const { return BOL->mTexLodBiasOn != 0; };
    u8 getShadowDepth() const { return BOL->mShadowDepth; }

private:
    SColHeader *BCO;
    SOblHeader *BOL;
    void *objList;
};

#endif
