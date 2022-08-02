#ifndef KARTCTRL_H
#define KARTCTRL_H

#include "types.h"

class KartCtrl
{
public:
    static KartCtrl *getKartCtrl(); /*{
       return mCtrl;

   }*/
    void GetBodyPos(int idx, JGeometry::TVec3<f32> *playerPos);

private:
    // static KartCtrl * mCtrl;
};

#endif