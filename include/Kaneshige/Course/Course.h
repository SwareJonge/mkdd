#ifndef COURSE_H
#define COURSE_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTDbg.h"

#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/CrsData.h"
#include "types.h"

enum ECourseID {
    TEST_1 = 0x1,
    TEST_2 = 0x2,
    TEST_3 = 0x3,
    TEST_4 = 0x4,
    TEST_5 = 0x5,
    TEST_6 = 0x6,
    TEST_7 = 0x7,
    TEST_8 = 0x8,
    TEST_9 = 0x9,
    TEST_10 = 0xa,
    TEST_11 = 0xb,
    TEST_12 = 0xc,
    TEST_13 = 0xd,
    TEST_14 = 0xe,
    TEST_15 = 0xf,
    TEST_16 = 0x10,
    TEST_17 = 0x11,
    TEST_18 = 0x12,
    TEST_19 = 0x13,
    TEST_20 = 0x14,
    TEST_21 = 0x15,
    TEST_22 = 0x16,
    TEST_23 = 0x17,
    TEST_24 = 0x18,
    TEST_25 = 0x19,
    TEST_26 = 0x1a,
    TEST_27 = 0x1b,
    DEBOKOKO = 0x1d,
    A3 = 0x1e,
    E3 = 0x1f,
    // UNK1 = 0x20,
    BABY_PARK = 0x21,
    PEACH_BEACH = 0x22,
    DAISY_CRUISER = 0x23,
    LUIGI_CIRCUIT = 0x24,
    MARIO_CIRCUIT = 0x25,
    YOSHI_CIRCUIT = 0x26,
    // UNK2 = 0x27,
    MUSHROOM_BRIDGE = 0x28,
    MUSHROOM_CITY = 0x29,
    WALUIGI_STADIUM = 0x2a,
    WARIO_COLOSSEUM = 0x2b,
    DINO_DINO_JUNGLE = 0x2c,
    DK_MOUNTAIN = 0x2d,
    // UNK3 = 0x2e,
    BOWSER_CASTLE = 0x2f,
    // UNK4 = 0x30,
    RAINBOW_ROAD = 0x31,
    DRY_DRY_DESERT = 0x32,
    SHERBET_LAND = 0x33,
    MINI_1 = 0x34,
    MINI_2 = 0x35,
    MINI_3 = 0x36,
    MINI_4 = 0x37,
    MINI_5 = 0x38,
    MINI_6 = 0x39,
    MINI_7 = 0x3a,
    MINI_8 = 0x3b,
    REVERSE_1 = 0x3c,
    REVERSE_2 = 0x3d,
    REVERSE_3 = 0x3e,
    REVERSE_4 = 0x3f,
    REVERSE_5 = 0x40,
    REVERSE_6 = 0x41,
    REVERSE_7 = 0x42,
    REVERSE_8 = 0x43,
    AWARD = 0x44,
    ENDING = 0x45
};

class Course {
public:
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
        CrsArea::SCheckPoint *mCheckPoint;
        CrsArea::SCheckPoint *mCheckPoint2;
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

    Sector *getSector(int no) const 
    {
#line 300
        JUT_MINMAX_ASSERT(0, no, mTotalSectors);
        return &mSectors[no];
    }

    Sector *getMainSector(int gen) const 
    {
        JUT_MINMAX_ASSERT(0, gen, mTrackSectorNum);
        return mMainSector[gen];
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
    Sector **mMainSector;
    u8 _051C[0x53C - 0x51C];
}; // Size: 0x53C

#endif