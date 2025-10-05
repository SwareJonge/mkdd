#ifndef KARTCHECK_H
#define KARTCHECK_H

#include "types.h"

class KartBody;

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
    KartBody* mBody;    // 0x0 (Inferred from assembly: stw this, 0x0(r30))
    u8 mField4;         // 0x4 (Inferred from assembly: stb r0, 0x4(r30), where r0=0)

}; // 0x8

#endif
