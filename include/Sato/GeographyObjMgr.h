#ifndef GEOGRAPHYOBJMGR_H
#define GEOGRAPHYOBJMGR_H

#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Shiraiwa/Balloon.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager : JKRDisposer
{
public:
    GeographyObjManager(const CrsData &);

    void calc();
    void createModel(JKRSolidHeap *, u32);
    void reset(const CrsData &);
    
    TJugem *getJugem(int camera_index) const;
    TBalloonManager *getBalloonMgr(int kart_index) const;

    f32 getKartHitDepthNormalObj(int kart_index) const { return mHitDepth[kart_index]; }
    static void createMgr(const CrsData &crsData) { gMgr = new GeographyObjManager(crsData); }
    static GeographyObjManager *getGeographyObjManager() { return gMgr; }

    static GeographyObjManager *gMgr;

    u8 _18[0x2A8 - 0x18];   //
    f32 mHitDepth[8];       // 2A8
    u8 _328[0x3FC - 0x2C8]; //
    TJugem *mJugem[4];      // 3FC
    u8 _40C[0x4A4 - 0x40C]; //

}; // size 0x4A4

inline GeographyObjManager *GetGeoObjMgr() { return GeographyObjManager::getGeographyObjManager(); };

inline void CreateGeoObjMgr(const CrsData *crsData) { GeographyObjManager::createMgr(*crsData); }

#endif