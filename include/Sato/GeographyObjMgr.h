#ifndef GEOGRAPHYOBJMGR_H
#define GEOGRAPHYOBJMGR_H

#include "JSystem/JKernel/JKRSolidHeap.h"
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
    void reset(CrsData*);
    static GeographyObjManager *getGeographyObjManager();
    TJugem *getJugem(int) const;
    TBalloonManager* getBalloonMgr(int kart_index) const;
    static GeographyObjManager * gMgr;

    u8 _18[0x4a4 - 0x18];
};

inline GeographyObjManager *GetGeoObjMgr() { return GeographyObjManager::getGeographyObjManager(); };

inline void CreateGeoObjMgr(const CrsData *crsData) { GeographyObjManager::createMgr(*crsData); }

#endif