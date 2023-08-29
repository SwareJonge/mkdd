#ifndef COURSE_H
#define COURSE_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTDbg.h"

#include "Kaneshige/Course/CrsData.h"
#include "kartEnums.h"
#include "types.h"

class CrsArea;

enum CrsAreaKind
{
    // TODO
    cCrsAreaKindShadow
};

class Course {
public:
    class Area {
    public:
        f32 getRate(const JGeometry::TVec3f &pos);
        bool check(JGeometry::TVec3f *out, const JGeometry::TVec3f &pos);

        u8 getKind() const { return mData->kind; } // enum        
        f32 getEchoScale() const { return mEchoScale; }
        s16 getLightParamID() const { return mData->lightParamId; };
        s16 getShadowID() const { return mData->shadowId; };

    public:
        CrsData::SArea *mData;
        JGeometry::TVec3f _4;
        JGeometry::TVec3f _10;
        JGeometry::TVec3f _1c;
        JGeometry::TVec3f _28;
        JGeometry::TVec3f _34;
        u8 _40[0x48 - 0x40]; // unk
        f32 mEchoScale;
    }; // size: 0x4c
    class Sector {
    public:
        int isMainSector() const;
        int getGeneration() const { return mGeneration; }
        float calcUnitDist(JGeometry::TVec3<float> const &);
        void calcGeneration(Sector * sector);

        // Inline Functions
        ECourseID getCourseID() const { return (ECourseID)mID; };
        bool isDiv() const { return bDivFlag; }
        bool isRevSearchEnable() { return mCheckPoint->mRevSearchEnable != 0; }
        uint getShortcutID() const { return mCheckPoint->mShortcutID; }
        int getNextNum() const { return mNextNum; }
        int getPrevNum() const { return mPrevNum; }
        Sector *getMainSector() { return mMainSector; }

        Sector *getPrevSector(int no) {
#line 100
            JUT_MINMAX_ASSERT(0, no, 4);
            return mPrevSectors[no];
        }

        Sector *getNextSector(int no) {
            JUT_MINMAX_ASSERT(0, no, 4);
            return mNextSectors[no];
        }

        float getTotalPriorDist() const { return mTotalPriorDist; }
        float getSectorDist(void) const { return mSectorDist; }
        bool isInvalid() const { return mCheckPoint->_0x19 != 0;  }

        void getBNormal(JGeometry::TVec3<f32> *out) {
            out->set(_0x70.x, 0.0f, _0x70.y);
            out->normalize();
        }

    private:
        s16 mID;
        s16 mGeneration;
        bool bMainSector;
        bool bDivFlag;
        CrsData::SCheckPoint *mCheckPoint;
        CrsData::SCheckPoint *mCheckPoint2;
        JGeometry::TVec3<float> _0x10[4];
        JGeometry::TVec3<float> _0x40[4];
        JGeometry::TVec2<float> _0x70;
        JGeometry::TVec2<float> _0x78;
        float mSectorDist;
        s16 mPrevNum;
        s16 mNextNum;
        Sector *mPrevSectors[4];
        Sector *mNextSectors[4];
        Sector *mMainSector;
        float mTotalPriorDist;
    };
    Course(CrsData *, int, bool);
    void createModel(JKRSolidHeap *, u32); // just a blr -_-
    void calc();
    void calcShaking();
    void reset();
    void update();
    void updateSky();

    // Inline Functions
    ECourseID getCourseID() const { return mID; }
    Sector *getStartSector() const { return &mSectors[0]; }
    float getTrackSectorDist() const { return mTrackSectorDist; }
    int getTotalSectorNumber() const { return mTotalSectors;  }
    int getTrackSectorNumber() const { return mTrackSectorNum; }
    CrsData *getCrsData() const { return mCrsData; }
    
    // These are in the cpp file itself, whoops
    void getAmbientColor(JUtility::TColor *dst); /*
     {
         getCrsData()->getAmbientColor(dst);
     }*/

    void getLightColor(JUtility::TColor *dst); /*
    {
        getCrsData()->getLightColor(dst);
    }*/

    void getLightOffsetPosition(JGeometry::TVec3f *dst); /*
    {
        getCrsData()->getLightOffsetPosition(dst);
    }*/

    int getAreaNumber() const { return mAreaNum; }
    Area *getArea(int no) const { return &mAreas[no]; }

    Sector *getSector(int no) const 
    {
#line 300
        JUT_MINMAX_ASSERT(0, no, mTotalSectors);
        return &mSectors[no];
    }

    Sector *getMainSector(int gen) const 
    {
        JUT_MINMAX_ASSERT(0, gen, mTrackSectorNum);
        return mMainSectors[gen];
    }

    Sector *searchSector(short);

private: // add all documented stuff from Ghidra
    ECourseID mID;
    u8 _4[0x4D8];
    CrsData *mCrsData; // 0x4DC
    u8 _4E0[0x50C - 0x4E0];
    s16 mTotalSectors; // 0x50C
    s16 mTrackSectorNum;
    Sector *mSectors;
    float mTrackSectorDist;
    Sector **mMainSectors;
    int _51C;
    int _520;
    int mAreaNum;
    Area *mAreas;
    u8 mSkyMode;
    u8 _052D[0x53C - 0x52d];
}; // Size: 0x53C

#endif