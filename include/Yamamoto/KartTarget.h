#ifndef KARTTARGET_H
#define KARTTARGET_H

#include "Kaneshige/CenterLine.h"

#include "types.h"

class KartTarget
{
public:
    KartTarget() {}

    void Init();
    void GetBrosTarget(int, f32 *);
    void GetNextTarget(int, f32 *);
    void GetBeforeTarget(int, f32 *);
    void DoSearchTarget(int);
    void DoSearchRescueTarget(CLPoint *);

private:
    CLPoint *mTargetPoint;
};

#endif