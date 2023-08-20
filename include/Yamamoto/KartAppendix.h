#ifndef KARTAPPENDIX_H
#define KARTAPPENDIX_H

#include "Kaneshige/KartLoader.h"

struct KartAppendix
{
    KartAppendix() {}
    void Init(int);

    ExModel *mModelWheel;
    KartLoader *mKartLoader;
};

#endif