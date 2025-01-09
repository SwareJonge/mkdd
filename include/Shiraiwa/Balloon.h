#ifndef BALLOON_H
#define BALLOON_H

#include "JSystem/JGeometry.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"
#include "types.h"

class J3DAnmObjMaterial;

class TBalloonSupervisor : public GeoObjSupervisor
{
public:
    ~TBalloonSupervisor();
    void drawString(u32);
    void drawBalloon(u32);

    // void reset() // TBalloonSupervisor.h; // 0x802c0d7c
    // void calc() // TBalloonSupervisor.h; // 0x802c0d88
};

// Inherited from GeographyObj
class TBalloon : public GeographyObj
{ // Autogenerated
public:
    // Global
    TBalloon();                               // 0x802bf9b8
    virtual ~TBalloon();                      // 0x802c0d8c
    void reset();                             // 0x802bfa54
    void update();                            // 0x802bfa80
    void loadAnimation();                     // 0x802bfae8
    void calc();                              // 0x802bfb6c
    void viewCalc(u32);                       // 0x802bfef0
    void createModel(JKRSolidHeap *, u32);    // 0x802bff1c
    void setCurrentViewNo(u32);               // 0x802bff58
    static const char *scEfctBreakColor;      // 0x803a85f0
    static f32 scScaleFrame1;                 // 0x80415394
    static f32 scScaleFrame2;                 // 0x80415398
    static f32 scScale1;                      // 0x8041539c
    static const char *scEfctBreakA;          // 0x804153a0
    static const char *scEfctBreakB;          // 0x804153a4
    static J3DAnmObjMaterial *sBalloonBrkAnm; // 0x80417078
    // Inline/Unused

    // void setColor(u32);
    // void die();
    // void draw(u32);
    // void TBalloonSupervisor::TBalloonSupervisor();
    // void JSUList<TBalloonString>::~JSUList();
    // void TBalloonSupervisor::createSortBuffer(int, int);
    // void TBalloonSupervisor::deleteSortBuffer(int);
    // void TBalloonSupervisor::entry(TBalloonString *);
    // void TBalloonSupervisor::makeDefaultSortList(u32, int);
    // void TBalloonString(const CrsData::SObject &);
    // void JSULink<TBalloonString>::~JSULink();
    // void getCameraDistance(u32);
    // void load(J3DModelData *);
    // void show();
    // void hide();
    // void drawString(u32);
    // void drawNode(float, const JGeometry::TVec3f &, const JGeometry::TVec3f &, u32, u8);
    // void drawCrevicePyramid(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, u8);
    // void drawPrism(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, u8);
    // void sWidth;
    // void sRabbitScaleSize;
    // void sAlphaJudgeNode;
    // void scOrgScale_Normal;
    // void scOrgScale_Rabbit;
    // void scOrgLength_Normal;
    // void scOrgLength_Rabbit;
    // Inline
    virtual const char *getBmdFileName() {return "/Objects/MiniGame_Balloon.bmd"; } // 0x802c0e4c
};                                        // class Balloon

class TBalloonString : public TMapObjHioNode, JKRDisposer
{
public:
    TBalloonString();
    virtual ~TBalloonString() {}
    void init();
    void setColor(u32);
    void unlock(u32);
    void setMainViewNo(char);
    void reset();
    void setCharID(const EKartWeight &, const ECharID &, const ECharID &);
    void resetNodeLength();
    void setPosition(const JGeometry::TVec3f &, const JGeometry::TPos3f);
    void calc();
    void die();
    void shake();
    void drawBalloon(u32);
    void getUpDir(JGeometry::TVec3f *);
    void getPoint(JGeometry::TVec3f *);
    void getPointDir(JGeometry::TVec3f *);
    void setParameters();

    virtual TBalloonSupervisor *getSupervisor() { return sSupervisor; }
#ifdef __MWERKS__
    static TBalloonSupervisor *getSupervisor() { return sSupervisor; }
#endif
    // void sOffset;
    // void sTotalLength;
    // void sAdjustRatio;
    // void sAirFric;
    // void sPower;
    // void sDemoPower;
    // void sGravity;
    // void sNodeNum;
    // void sOrgScale;
    // void sOrgLength;
    // void sBreakOffset;
    // void sBreakUpVel;
    // void sShakePow;
    static TBalloonSupervisor *sSupervisor;
};

class TBalloonManager : public TMapObjHioNode
{
public:
    TBalloonManager();
    void init(u8, char, u8);
    void reset();
    void calc();
    void killBalloon(TBalloonString *);
    void checkCameraDistance();
    void robRivalOfBalloon(u32);
    void rob();
    void setBalloon(TBalloonString *);
    void removeBalloon();

    // static void sTotalLength;
    // static void sPullDirX;
    // static void sPullDirY;
    // static void sPullDirZ;

    // Inline/Unused
    TBalloonManager(const CrsData::SObject &);
    // void isEscape();
    // void drawBalloon(u32);
    // void getSpareBalloon();

    virtual ~TBalloonManager() { }
    virtual void createColModel(J3DModelData *) { }

    void changeWinningPos() {
        if (RCMGetKartChecker(mKartIdx)->getRank() == 1) {
            mIsWinning = true;
        }
    }

    u8 _14c[0x16e - 0x14c];
    u8 mKartIdx; // 16e
    u8 _16f;
    bool mIsWinning; // 170
};


#endif // BALLOON_H
