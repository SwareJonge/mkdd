#ifndef KARTTARGET_H
#define KARTTARGET_H

#include "Kaneshige/CenterLine.h"

#include "types.h"

class KartTarget
{
public:
    KartTarget() {}

    void Init();
    CLPoint *GetBrosTarget(int, f32 *);
    CLPoint *GetNextTarget(int, f32 *);
    CLPoint *GetBeforeTarget(int, f32 *);
    void DoSearchTarget(int);
    void DoSearchRescueTarget(CLPoint *);

private:
    CLPoint *mCenter;
};

#endif