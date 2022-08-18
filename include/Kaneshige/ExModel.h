#ifndef EXMODEL_H
#define EXMODEL_H

#include "types.h"

class ExModel
{
public:
    MTX34 *getBaseTRMtx(); /* {
        return mBaseTRMtx;
    }*/

private:
    u8 _0[0x2c];
    MTX34 mBaseTRMtx;
};

#endif // EXMODEL_H

