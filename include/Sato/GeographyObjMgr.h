#ifndef GEOGRAPHYOBJMGR
#define GEOGRAPHYOBJMGR

#include "Kaneshige/Course/CrsData.h"
#include "Shiraiwa/JugemMain.h"

class GeographyObjManager
{
public:
    void createMgr();
    GeographyObjManager *getGeographyObjManager();
    TJugem *getJugem(int) const;
};

GeographyObjManager *GetGeoObjMgr();

void CreateGeoObjMgr(const CrsData *);

#endif