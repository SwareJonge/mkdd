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
    bool CheckOnlyTandemPartsClearKey(int);
    bool CheckPlayer();
    bool CheckDraw(u32, int);
    bool CheckIndication();

private:
    u8 _0[0x8];

}; // 0x8

#endif