#ifndef KARTCHECK_H
#define KARTCHECK_H

#include "types.h"

class KartCheck
{
public:
    KartCheck() {}

    void Init(int);
    void CheckCrash();
    void CheckCheange(int);
    void CheckSpecialDyn();
    void CheckSpecialDyn2();
    void CheckBodyHit(int, int);
    void CheckAllClearKey();
    void CheckAllClearKeyT();
    void CheckPartsClearKey(int);
    void CheckOnlyTandemPartsClearKey(int);
    void CheckPlayer();
    void CheckDraw(u32, int);
    void CheckIndication();

private:
    // TODO
};

#endif