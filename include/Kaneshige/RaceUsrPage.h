#ifndef RACEUSRPAGE_H
#define RACEUSRPAGE_H

#include "Kaneshige/SysDebug.h"
#include "Kaneshige/RaceInfo.h"

class RaceUsrPage : public SysDbUsrPage {
public:
    RaceUsrPage(RaceInfo *raceInfo) {
        mRaceInfo = raceInfo;
    }
    virtual ~RaceUsrPage() {};
    virtual void draw();

    RaceInfo *mRaceInfo;
};


#endif