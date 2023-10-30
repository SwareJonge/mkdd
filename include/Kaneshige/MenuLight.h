#ifndef MENULIGHT_H
#define MENULIGHT_H

#include <JSystem/JUtility/TColor.h>
#include "Kaneshige/LightMgr.h"

class MenuAmbient : public LtObjAmbient
{
    MenuAmbient(JUtility::TColor color); /* : LtObjAmbient(color) { sAmbient = this; }*/
    virtual ~MenuAmbient();

    void createAndAppend(JKRHeap *heap, JUtility::TColor color);

private:
    static MenuAmbient *sAmbient;
}; // Size: 0x3c

class MenuSceneLight : public LtObjDiffuse // ("前")
{

private:
    

    JGeometry::TVec3f _90;
    JGeometry::TVec3f _9c;
    JGeometry::TVec3f _a8;
    JGeometry::TVec3f _b4;
    JGeometry::TVec3f _c0;
    JGeometry::TVec3f _cc;
    Mtx _d8;
}; // Size: 0x108

#endif