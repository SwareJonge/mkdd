#ifndef KARTSUS_H
#define KARTSUS_H

#include <JSystem/JGeometry.h>

class KartBody;

class KartSus
{
public:
    KartSus() {}

    void InitSettingParam(int);
    void InitParam(int);
    void Init(int);
    void NormalInit(int);
    void ResetInit(int);
    void CircleFriction(JGeometry::TVec3f *, f32);
    f32 GetCornerForce();
    f32 GetCircleFric();
    void DoSusAction(int); // amogus
    // Inlines
    KartBody *getKartBody() /* { return mBody; } */;

private:
    // TODO
    // KartBody *mBody; // 0xc
};

#endif