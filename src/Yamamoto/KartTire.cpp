#include "Yamamoto/kartCtrl.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void KartTire::Init(int idx) {
    mBody = GetKartCtrl()->getKartBody(idx);
    _4 = 0;
    _5 = 0;
    _8 = 0;
    _6 = 0;
}