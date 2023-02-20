#ifndef GEOGRAPHYOBJMGR
#define GEOGRAPHYOBJMGR

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager : JKRDisposer
{
public:
    GeographyObjManager(const CrsData & );   
    static void createMgr(const CrsData & crsData) {
        gMgr = new GeographyObjManager(crsData);
    }
    void createModel(JKRSolidHeap *, u32);
    void reset(CrsData*);
    GeographyObjManager *getGeographyObjManager();
    TJugem *getJugem(int) const;
    static GeographyObjManager * gMgr;
};

GeographyObjManager *GetGeoObjMgr();

void CreateGeoObjMgr(const CrsData *crsData) { GeographyObjManager::createMgr(*crsData); }

#endif