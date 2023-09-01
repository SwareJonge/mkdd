#include <std/math.h>
#include "Kaneshige/KartMtx.h"

void KartMtxPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f, bool flip)
{
    f32 angle = std::tanf(MTXDegToRad(0.5f * fovY));
    f32 top = n * angle;
    f32 bottom = -top;
    f32 left = -top * aspect;

    if(flip) 
        left = -left;

    C_MTXFrustum(m, top, bottom, left, -left, n, f);
}

// Unused Functions
KartOrthoGraph::KartOrthoGraph(f32, f32, f32, f32, f32, f32, bool) {

}
void KartOrthoGraph::setPort() {

}
KartOrthoGraph::~KartOrthoGraph() {

}