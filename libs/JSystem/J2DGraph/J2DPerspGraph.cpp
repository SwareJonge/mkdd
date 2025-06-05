#include "types.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "fdlibm.h"
#include "math.h"
#include "dolphin/gx.h"


J2DPerspGraph::J2DPerspGraph(f32 x, f32 y, f32 w, f32 h, f32 fovY, f32 near, f32 far) : J2DGrafContext(x, y, w, h) {
    set(fovY, near, far);
}

J2DPerspGraph::J2DPerspGraph() : J2DGrafContext(0.0f, 0.0f, 0.0f, 0.0f)
{
}

void J2DPerspGraph::set(f32 fovY, f32 near, f32 far)
{
    setFovy(fovY);
    mNear = near;
    mFar  = far;
    setLookat();
}

void J2DPerspGraph::setFovy(f32 fovY)
{
    mFovY = fovY;
    if (fovY < 1.0f) {
        mFovY = 1.0f;
    } else if (fovY > 179.0f) {
        mFovY = 179.0f;
    }
}

void J2DPerspGraph::setPort()
{
    J2DGrafContext::setPort();
    C_MTXPerspective(mMtx44, mFovY, mBounds.getWidth() / mBounds.getHeight(), mNear, mFar);
    GXSetProjection(mMtx44, GX_PERSPECTIVE);
}

void J2DPerspGraph::setLookat()
{
    f32 tanTheta = tan(mFovY * F_PI / 360.0f);
    mZPos        = (mBounds.getHeight() / 2) / tanTheta;
    makeLookat();
}

void J2DPerspGraph::makeLookat()
{
    f32 width  = (mBounds.f.x + mBounds.i.x) / 2;
    f32 height = (mBounds.f.y + mBounds.i.y) / 2;

    Vec pos;
    pos.x = width;
    pos.y = height;
    pos.z = -(mZPos);

    Vec dest;
    dest.x = width;
    dest.y = height;
    dest.z = 0.0f;

    Vec up;
    up.x = 0.0f;
    up.y = -1.0f;
    up.z = 0.0f;

    C_MTXLookAt(mPosMtx, &pos, &up, &dest);
    GXLoadPosMtxImm(mPosMtx, 0);
}
