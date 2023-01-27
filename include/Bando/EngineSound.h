#ifndef ENGINESOUND_H
#define ENGINESOUND_H

#include "Kaneshige/KartInfo.h"

// create Header with just enums
enum EngineType {
    ENGINE0,
    ENGINE1,
    ENGINE2,
    ENGINE3,
    ENGINE4,
    ENGINE5,
    ENGINE6,
    ENGINE7,
    ENGINE8
};

namespace EngineSound {
    EngineType getEngineType(EKartID kartID);
}

#endif // !ENGINESOUND_H
