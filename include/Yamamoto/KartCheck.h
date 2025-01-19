#ifndef KARTCHECK_H
#define KARTCHECK_H

#include "types.h"

class KartCheck
{
public:
    KartCheck() {}

    void Init(int);
    bool CheckCrash();
    bool CheckCheange(int);
    bool CheckSpecialDyn();
    bool CheckSpecialDyn2();
    bool CheckBodyHit(int, int);
    bool CheckAllClearKey();
    bool CheckAllClearKeyT();
    bool CheckPartsClearKey(int);
    bool CheckOnlyTandemPartsClearKey(int);
    bool CheckPlayer();
    bool CheckDraw(u32, int);
    bool CheckIndication();

private:
    u8 _0[0x8];

}; // 0x8

#endif
