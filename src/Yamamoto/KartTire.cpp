#include "Yamamoto/kartCtrl.h"

void KartTire::Init(int idx) {
    mBody = GetKartCtrl()->getKartBody(idx);
    _4 = 0;
    _5 = 0;
    _8 = 0;
    _6 = 0;
}