#ifndef KARTAPPENDIX_H
#define KARTAPPENDIX_H

#include "Kaneshige/KartLoader.h"

struct KartAppendix
{
    KartAppendix() {}
    void Init(int);

    void *mModelWheel;
    KartLoader *mKartLoader;
};

#endif