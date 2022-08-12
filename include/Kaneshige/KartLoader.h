#ifndef KARTLOADER_H
#define KARTLOADER_H

#include "types.h"
#include "Kaneshige/ExModel.h"

class KartLoader
{
public:
    ExModel *getExModelBody(); /* {
        return mModelBody;
    }*/

private:
    u8 _0[0x14];
    ExModel mModelBody;
};

#endif // KARTLOADER_H