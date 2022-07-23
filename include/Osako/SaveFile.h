#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "types.h"

class SaveFile
{
public:
    static bool msCRCTableComputed;
    static u32 msaCRCTable[256];
    void makeCRCTable();
    u32 getCRC(u8 *pBegin, u8 *pEndNext);
};

#endif