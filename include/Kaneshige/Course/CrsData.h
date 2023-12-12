#ifndef CRSDATA_H
#define CRSDATA_H

#include "JSystem/JUtility/TColor.h"
#include "Sato/stMath.h"
//#include "JSystem/JGeometry.h"

#include "kartEnums.h"
#include "kartLocale.h"
#include "types.h"

class Course; // Forward Declaration

// TODO: refactor some of these structs to not use camelcase naming

class CrsData
{
public:
    struct SArea
    {
        JGeometry::TVec3f pos;
        JGeometry::TVec3f scale;
        JGeometry::TVec3<s16> zNorm;
        JGeometry::TVec3<s16> yNorm;
        u8 shape; // 1 is cylinder?
        u8 kind;
        s16 cameraId;
        int feather1;
        int feather2;
        s16 echoScale;
        s16 _32;
        s16 shadowId;
        s16 lightParamId;
    };

    struct SCheckPoint
    {
        JGeometry::TVec3f cpStart;
        JGeometry::TVec3f cpEnd;
        u8 mShortcutID;
        u8 _0x19;
        bool mRevSearchEnable;
        u8 _0x1b;
    }; // Size: 0x1c

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
        JGeometry::TVec3f _0;
        JGeometry::TVec3f _c;
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
        u8 mSkyMode;
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

    // https://mkdd.miraheze.org/wiki/mObjectData_(File_Format)#Section_1
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
    { // mostly copied from https://mkdd.miraheze.org/wiki/mObjectData_(File_Format) (Section 9: Respawn Points)
        JGeometry::TVec3f mPosition;
        s16 dirX; // TVec3?
        s16 dirY;
        s16 dirZ;
        s16 _12[3]; // seems to be padding
        u16 mID;
        u16 mCPointID;
        s16 mCameraID;
        s16 mSectorID;
    };


    struct CheckPath 
    {
        int getELinkNum() const;

        bool isMainPath() const { return !(link & 0x8000) ; }

        u16 pointCount;
        u16 link;
        s16 prev[4]; // The indices of up to 4 group links that follow to this one. Unneeded slots are set to value -1.
        s16 next[4]; // The indices of up to 4 group links to follow after this one. Unneeded slots are set to value -1.
    };

    class Ground
    {
    public:
        bool checkPosition(stPlaneParam *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f *) const;                                                         // 0x8019fb34
        void checkWallBySphere(JGeometry::TVec3f *, const SColInfoSphere &, const JGeometry::TVec3f *, float *, JGeometry::TVec3f *, JGeometry::TVec3f *) const;                           // 0x801a0078
        void getNearPoint(const SColInfoSphere &, float, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, JGeometry::TVec3f *, JGeometry::TVec3f *) const; // 0x801a02fc
        void checkPolygonCoverWall(const JGeometry::TVec3f &, const SColInfoSphere &, const Ground *);                                                                                     // 0x801a0590
        void checkFront(const CrsData *, const JGeometry::TVec3f &) const;                                                                                                                 // 0x801a0770
        void checkFaceSide(const CrsData *, const JGeometry::TVec3f &) const;                                                                                                              // 0x801a083c
        void getPlaneY(const JGeometry::TVec3f &, Course *) const;                                                                                                                         // 0x801a097c
        void checkPolygonTouchBySphere(const JGeometry::TVec3f *, const JGeometry::TVec3f &, float, Course *) const;                                                                       // 0x801a0ae4
        void getTouchState(const SColInfoSphere &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, JGeometry::TVec3f *, float *, float) const;                                       // 0x801a0c5c
        void getCenter(JGeometry::TVec3f *, const JGeometry::TVec3f *, Course *) const;                                                                                                    // 0x801a0f64
        // Inline
        u8 getZmaxIndex() const;    // 0x801a0034
        u8 getXmaxIndex() const;    // 0x801a0040
        u8 getZminIndex() const;    // 0x801a004c
        u8 getXminIndex() const;    // 0x801a0058
        u8 getAddThickness() const; // 0x801a0064
    private:
        // TODO
    };

    struct PathData
    {
        int getPointNumber() const { return pointCount; }
        bool isClosed() const { return flags & 1;}
        u16 pointCount;
        u16 pointStart;
        u16 unk;
        u16 flags;
        u8 pad[0x8];
    };

    struct PointData
    {
        JGeometry::TVec3f pos;
        u32 linkPoint;
        u8 pad[0x10];
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
        u8 mPlayerID;
        s16 mJugemOffsetY;
    };

    CrsData(SColHeader *, SOblHeader *);
    void getAmbientColor(JUtility::TColor *) const;
    void getLightColor(JUtility::TColor *) const;
    void getShadowColor(JUtility::TColor *) const;
    void getLightOffsetPosition(JGeometry::TVec3f *) const;
    PathData *getPathData(u16 pathIdx) const;
    PointData *getPointData(u16, u16) const;
    void patchCamDataForStaffRoll(Language, VideoMode);
    int getFogType() const;
    f32 getFogStartZ() const;
    f32 getFogEndZ() const;
    void getFogColor(JUtility::TColor *color) const;
    StartPoint* getStartPoint(u8) const;
    ECourseID getCourseID() const { return (ECourseID)mObjectData->mCourseID; }
    u8 getTotalLapNumber() const { return mObjectData->mTotalLapNum; }
    bool isTexLODBiasOn() const { return mObjectData->mTexLodBiasOn != 0; }
    u8 getShadowDepth() const { return mObjectData->mShadowDepth; }
    u8 getShaking() const { return mObjectData->mShaking; }
    int getAreaNumber() const { return mObjectData->mAreaNum; }
    int getCheckPathNumber() const { return mObjectData->mCheckPathNum; }
    int getCLPointNumber() const { return mObjectData->mCLPointNum; }
    int getJugemPointNumber() const { return mObjectData->mJugemPointNum; }
    int getPathNumber() const { return mObjectData->mPathNum; }
    bool isSkyFollow() const { return mObjectData->mSkyMode != 0; }
    bool isShaking() const { return mObjectData->mShaking != 0; }

    int getCheckPointOffset(int n) const;

    // These are also not weak you silly
    SCheckPoint *getCheckPointTable() const; /*
     {
         CheckPath *checkPathTbl = getCheckPathTable();
         return (SCheckPoint *)&checkPathTbl[getCheckPathNumber()]; // CheckPoint Data is after the CheckPath section
     }*/

    // Read from file offset

    /*Camera *getCameraData(int camIdx) const
    {
        Camera *camData = nullptr;

        if (camIdx >= 0 && camIdx < getCameraNumber())
        {
            camData = reinterpret_cast<Camera *>(reinterpret_cast<char *>(mObjectData) + mObjectData->mCameraOffset) + camIdx;
        }
        return camData;
    }*/

    SCLPoint *getCLPointTable() const { return (SCLPoint *)(((u8 *)mObjectData) + mObjectData->mCLPointOffset); }
    SArea *getAreaTable() const { return (SArea *)(((u8 *)mObjectData) + mObjectData->mAreaOffset); }
    CheckPath *getCheckPathTable() const { return (CheckPath *)(((u8 *)mObjectData) + mObjectData->mCheckPathOffset); }
    SJugemPoint *getJugemPointTable() const { return (SJugemPoint *)(((u8 *)mObjectData) + mObjectData->mJugemPointOffset); }

private:
    SColHeader *mCollionData;
    SOblHeader *mObjectData;
    void *mObjList;
};

#endif
