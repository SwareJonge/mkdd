#ifndef KARTDISP_H
#define KARTDISP_H

#include <JSystem/JGeometry.h>
#include "types.h"

class KartBody;

class KartDisp
{
public:
    KartDisp() {}

    void Init(int i);             // 0x803101d8
    void DoShake();               // 0x80310214
    void MakeDispBodyPose();      // 0x803104d4
    void MakeDispBodyPos();       // 0x8031069c
    void MakeSusMatrix(int i);    // 0x803109d4
    void MatrixScale(Mtx, f32); // 0x80311138
    void MakeTireDispMatrix();    // 0x803111a8
    void MakeBodyDispMatrix();    // 0x803116d0
    void MakeScaleMatrix();       // 0x80311820
    void MakeKartDisp();          // 0x80311980
    // Inline/Unused
    void DomFalgDisp();
    void DoDispDebug();
private:
    KartBody *mBody;
};
#endif // KARTDISP_H