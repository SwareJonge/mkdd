#ifndef GEOGRAPHYOBJMGR
#define GEOGRAPHYOBJMGR

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager
{
public:
    void createMgr();
    void createModel(JKRSolidHeap *, u32);
    GeographyObjManager *getGeographyObjManager();
    TJugem *getJugem(int) const;
};

GeographyObjManager *GetGeoObjMgr();

void CreateGeoObjMgr(const CrsData *);

#endif