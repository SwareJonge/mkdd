#ifndef GEOGRAPHYOBJMGR_H
#define GEOGRAPHYOBJMGR_H

#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Shiraiwa/Balloon.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager : JKRDisposer
{
public:
    GeographyObjManager(const CrsData & );   
    static void createMgr(const CrsData & crsData) {
        gMgr = new GeographyObjManager(crsData);
    }
    void calc();
    void createModel(JKRSolidHeap *, u32);
    void reset(const CrsData&);
    static GeographyObjManager *getGeographyObjManager() { return gMgr; }
    TJugem *getJugem(int camera_index) const;
    TBalloonManager* getBalloonMgr(int kart_index) const;
    static GeographyObjManager * gMgr;

    u8 _18[0x3FC - 0x18];
    TJugem * mJugem[4]; // 0x3FC
    u8 _40C[0x4A4 - 0x40C];

}; // size 0x4A4

inline GeographyObjManager *GetGeoObjMgr() { return GeographyObjManager::getGeographyObjManager(); };

inline void CreateGeoObjMgr(const CrsData *crsData) { GeographyObjManager::createMgr(*crsData); }

#endif