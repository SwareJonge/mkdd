#ifndef CRSDATA_H
#define CRSDATA_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JGeometry.h"
#include "kartLocale.h"
#include "types.h"

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

    struct SOblHeader
    {
        char mMagic[4];
        u8 mShaking;
        u8 mAmbientColor[3];
        JUtility::TColor mLightColor;
        JGeometry::TVec3<f32> mLightOffsetPos;
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

    class StartPoint {
    public:
        bool isRight() const { return mRight != 0; };
        f32 getJugemOffsetY() const { return mJugemOffsetY; };

        // inline but too lazy to add it
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

        JGeometry::TVec3f mPos;
        JGeometry::TVec3f mScale;
        JGeometry::TVec3<s16> mRot;
        u8 _1E[0x24 - 0x1E];
        u8 mRight; // probably call this something else
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
