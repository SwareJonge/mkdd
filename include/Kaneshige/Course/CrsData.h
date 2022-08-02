#ifndef CRSDATA_H
#define CRSDATA_H

#include "types.h"

#include "JSystem/JGeometry.h"

// add this to JSystem
struct TColor
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

class CrsData
{
public:
    struct SOblHeader
    {
        char mMagic[4];
        u8 mShaking;
        u8 mAmbientColor[3];
        TColor mLightColor;
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
        JGeometry::TVec3<f32> mPosition;
        s16 _c; // called by "getFrDirection" however according to the wiki this is an array of 3 integers?
        s16 _e;
        s16 _10;
        s16 _12[3]; // seems to be padding
        u16 mID;
        u16 mCPointID;
        s16 mCameraID;
        s16 mSectorID;
    };

private:
    void *BCO;
    SOblHeader *BOL;
    void *objList;
};

#endif