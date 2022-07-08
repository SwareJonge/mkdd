#ifndef JMATRIGONOMETRIC_H
#define JMATRIGONOMETRIC_H

#include "types.h"
#include "std/pair.h"

// from TP decomp and pikmin decomp
struct TSinCosTable {
    std::pair<f32, f32> table[0x2000];

    f32 sinShort(s16 v) const { return table[static_cast<u16>(v) >> 5].first; }
    f32 cosShort(s16 v) const { return table[static_cast<u16>(v) >> 5].second; }
};

#endif // !JMATRIGONOMETRIC_H 