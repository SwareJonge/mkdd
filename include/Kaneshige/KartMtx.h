#ifndef KARTMTX_H
#define KARTMTX_H

#include <dolphin/mtx.h>

#include "types.h"

extern void KartMtxPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f, bool flip);

// Unused class
class KartOrthoGraph
{
public:
    KartOrthoGraph(f32, f32, f32, f32, f32, f32, bool);
    ~KartOrthoGraph();
    void setPort();
};

#endif