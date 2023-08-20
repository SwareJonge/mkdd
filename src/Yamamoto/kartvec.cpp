#include "Yamamoto/KartMat.h"
#include "Yamamoto/KartMath.h"
#include "Yamamoto/KartVec.h"

#include "std/math.h"

#include <ppcdis.h>

#include "orderfloatsm/8041d7f8_8041d818.inc"
#include "orderdoublesm/8041d818_8041d830.inc"

// TODO: hire a mathematician to properly name variables

void KartVec::SetVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void KartVec::AddVector(VecPtr vec, f32 x, f32 y, f32 z){
    vec->x += x;
    vec->y += y;
    vec->z += z;
}

void KartVec::MulVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x *= x;
    vec->y *= y;
    vec->z *= z;
}

f32 KartVec::VectorLength(VecPtr vec){
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

f32 KartVec::VectorLengthSqrtf(VecPtr vec){
    return SpeedySqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

void KartVec::OuterCalc(VecPtr out, VecPtr vec1, VecPtr vec2)
{
    f32 x1 = vec1->x;
    f32 y1 = vec1->y;
    f32 z1 = vec1->z;
    f32 x2 = vec2->x;
    f32 y2 = vec2->y;
    f32 z2 = vec2->z;

    out->x = y1 * z2 - z1 * y2;
    out->y = z1 * x2 - x1 * z2;
    out->z = x1 * y2 - y1 * x2;
}

f32 KartVec::InnerCalc(VecPtr vec1, VecPtr vec2)
{
    return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

void KartVec::VectorElement(JGeometry::TVec3f *out, JGeometry::TVec3f *vec1, JGeometry::TVec3f *vec2)
{
    f32 len = VectorLength((VecPtr)vec1);
    if(vec1->isZero() == false) {
        f32 inner = InnerCalc((VecPtr)vec2, (VecPtr)vec1);
        out->x = vec2->x * (inner / len);
        out->y = vec2->y * (inner / len);
        out->z = vec2->z * (inner / len);
    }
    else {
        out->zero();
    }
}

f32 KartMath::SpeedySqrtf(register f32 x) {
    register f64 recip;
    volatile f32 xf;
    if (x > 0.0f)
    {
        __asm { frsqrte recip, x }
        xf = x * recip;
        return xf;
    }
    return x;
}

void KartMath::CrossToPolar(JGeometry::TVec3f *vec1, JGeometry::TVec3f *vec2, f32 *pLength, f32 *pAng1, f32 *pAng2)
{
    f32 divX = (vec2->x - vec1->x);
    f32 divY = (vec2->y - vec1->y);
    f32 divZ = (vec2->z - vec1->z);

    f32 len = SpeedySqrtf(divX * divX + divY * divY + divZ * divZ);

    *pLength = len;
    *pAng1 = std::atan2f(SpeedySqrtf(divX * divX + divZ * divZ), divY);
    *pAng2 = std::atan2f(divZ, divX);
}

void KartMath::PolarToCross(VecPtr base, VecPtr out, f32 x, f32 y, f32 z) {
    // angle in ??? convert radian to euler?    
    s16 angleY = 16384.0f * ((180.0f * y) / 3.141f) / 90.0f;
    s16 angleZ = 16384.0f * ((180.0f * z) / 3.141f) / 90.0f;

    f32 cosY = JMASCos(angleY);
    f32 sinZ = JMASSin(angleZ);

    f32 cosZ = JMASCos(angleZ);
    f32 sinY = JMASSin(angleY);

    out->x = base->x + x * cosY * sinZ;
    out->y = base->y + x * sinY;
    out->z = base->z + x * cosY * cosZ;
}


// These 2 are by far the hardest in this file
MANGLED_ASM(f32 KartMath::lu(Mtx33, int *)){
#include "asm/802e5600.s"
}

f32 fake(f32 x) // remove when ::lu is decompiled
{
    return std::fabs(x);
}

/*f32 KartMath::lu(Mtx33 m, int *pValidMatrices) {

    f32 tmp[3];

    for(int i = 0; i < 3; i++) {
        pValidMatrices[i] = i;
        f32 x = 0.0f;

        for(int j = 0; j < 3; j++) {
            f32 absval = std::fabs(m[j][i]);
            if (x < absval) {
                x = absval;
            }
        }
        if(x == 0.0f) return x;

        tmp[i] = 1.0f / x;
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {

        }
    }

}*/

MANGLED_ASM(void KartMath::solve(Mtx33, f32 *, int *, f32 *, int n)){
#include "asm/802e5824.s"
}

f32 KartMath::Gauss(Mtx33 mtx, f32 *vec1, f32 *vec2)
{
    int stack[4];
    f32 ret = lu(mtx, stack);

    if (ret != 0.0f)
    {
        solve(mtx, vec1, stack, vec2, 3);
    }
    return ret;
}

f32 KartMath::LimmtNumber(f32 f1, f32 f2) {
    if(f1 < -f2) {
        return -f2;
    }
    if(f1 > f2) {
        return f2;
    }
    return f1;
}

f32 KartMath::fcnvge(f32 x, f32 y, f32 z, f32 w)
{
    if (x < y)
    {
        if (x + z > y)
        {
            return y;
        }
        return x + z;
    }
    else
    {
        if (x - w < y)
        {
            return y;
        }
        return x - w;
    }
}

void KartMath::ChaseFnumber(f32 *out, f32 x, f32 y) {
    *out += y * (x - *out);

    if (*out < 0.001 && *out > -0.001)
    {
        *out = 0.0f;
    }
}

void KartMath::ChaseFcnvge(f32 *outf, u16 *outi, f32 x, u16 y) {
    *outf += (f32)*outi / (f32)y * (x - *outf);
    *outi = *outi + 1;

    if (*outi >= y)
    {
        *outi = y;
    }
}

void KartMath::ChaseFcnvge4(f32 *outf, u16 *outi, f32 x, u16 y)
{
    f32 tmp = (f32)*outi / (f32)y;
    tmp = tmp * tmp;
    *outf += tmp * tmp * (x - *outf);
    *outi = *outi + 1;

    if (*outi >= y)
    {
        *outi = y;
    }
}

void KartMat::ClearRotMatrix(Mtx mtx) {
    mtx[0][0] = 1.0f;
    mtx[1][0] = 0.0f;
    mtx[2][0] = 0.0f;

    mtx[0][1] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[2][1] = 0.0f;

    mtx[0][2] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[2][2] = 1.0f;

    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
}

void KartMat::SetPosePosMatrix(Mtx out, Mtx in, VecPtr vec){
    out[0][0] = in[0][0];
    out[1][0] = in[1][0];
    out[2][0] = in[2][0];

    out[0][1] = in[0][1];
    out[1][1] = in[1][1];
    out[2][1] = in[2][1];

    out[0][2] = in[0][2];
    out[1][2] = in[1][2];
    out[2][2] = in[2][2];

    out[0][3] = vec->x;
    out[1][3] = vec->y;
    out[2][3] = vec->z;
}

void KartMat::SetPosMatrix(Mtx out, VecPtr vec) {
    out[0][3] = vec->x;
    out[1][3] = vec->y;
    out[2][3] = vec->z;
}

void KartMat::MulMatrixByVector(VecPtr out, VecPtr base, Mtx m)
{
    f32 x = m[0][0] * base->x + m[0][1] * base->y + m[0][2] * base->z;
    f32 y = m[1][0] * base->x + m[1][1] * base->y + m[1][2] * base->z;
    f32 z = m[2][0] * base->x + m[2][1] * base->y + m[2][2] * base->z;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::DevMatrixByVector(VecPtr out, VecPtr base, Mtx m) {
    f32 x = m[0][0] * base->x + m[1][0] * base->y + m[2][0] * base->z;
    f32 y = m[0][1] * base->x + m[1][1] * base->y + m[2][1] * base->z;
    f32 z = m[0][2] * base->x + m[1][2] * base->y + m[2][2] * base->z;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::DevMatrixByVector2(JGeometry::TVec3f *out, JGeometry::TVec3f *base, Mtx m) {

    f32 x = m[0][0] * base->x + m[1][0] * base->y + m[2][0] * base->z;
    f32 y = m[0][1] * base->x + m[1][1] * base->y + m[2][1] * base->z;
    f32 z = m[0][2] * base->x + m[1][2] * base->y + m[2][2] * base->z;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::DevCrdMatrixByVector(VecPtr out, VecPtr base, Mtx m) {
    f32 divX = (base->x - m[0][3]); // i hate this codec!
    f32 divY = (base->y - m[1][3]);
    f32 divZ = (base->z - m[2][3]);

    f32 x = m[0][0] * divX + m[1][0] * divY + m[2][0] * divZ;
    f32 y = m[0][1] * divX + m[1][1] * divY + m[2][1] * divZ;
    f32 z = m[0][2] * divX + m[1][2] * divY + m[2][2] * divZ;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::MulMatrix(Mtx m, Mtx m2, Mtx m3) {
    m[0][0] = m2[0][0] * m3[0][0] + m2[1][0] * m3[0][1] + m2[2][0] * m3[0][2];
    m[1][0] = m2[0][0] * m3[1][0] + m2[1][0] * m3[1][1] + m2[2][0] * m3[1][2];
    m[2][0] = m2[0][0] * m3[2][0] + m2[1][0] * m3[2][1] + m2[2][0] * m3[2][2];

    m[0][1] = m2[0][1] * m3[0][0] + m2[1][1] * m3[0][1] + m2[2][1] * m3[0][2];
    m[1][1] = m2[0][1] * m3[1][0] + m2[1][1] * m3[1][1] + m2[2][1] * m3[1][2];
    m[2][1] = m2[0][1] * m3[2][0] + m2[1][1] * m3[2][1] + m2[2][1] * m3[2][2];

    m[0][2] = m2[0][2] * m3[0][0] + m2[1][2] * m3[0][1] + m2[2][2] * m3[0][2];
    m[1][2] = m2[0][2] * m3[1][0] + m2[1][2] * m3[1][1] + m2[2][2] * m3[1][2];
    m[2][2] = m2[0][2] * m3[2][0] + m2[1][2] * m3[2][1] + m2[2][2] * m3[2][2];
}

void KartMat::RotYMatrix(Mtx m, f32 ang) {
    // Use PI Macro later
    s16 v = 16384.0f * ((180.0f * ang) / 3.141f) / 90.0f;

    f32 cos = JMASCos(v);
    f32 sin = JMASSin(v);

    m[0][0] = cos;
    m[1][0] = 0.0f;
    m[2][0] = -sin;

    m[0][1] = 0.0f;
    m[1][1] = 1.0f;
    m[2][1] = 0.0f;

    m[0][2] = sin;
    m[1][2] = 0.0f;
    m[2][2] = cos;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

void KartMat::RotYMatrix33(Mtx m, f32 ang){
    // Use PI Macro later
    s16 v = 16384.0f * ((180.0f * ang) / 3.141f) / 90.0f;

    f32 cos = JMASCos(v);
    f32 sin = JMASSin(v);

    m[0][0] = cos;
    m[1][0] = 0.0f;
    m[2][0] = -sin;

    m[0][1] = 0.0f;
    m[1][1] = 1.0f;
    m[2][1] = 0.0f;

    m[0][2] = sin;
    m[1][2] = 0.0f;
    m[2][2] = cos;
}

void KartMat::RotXMatrix(Mtx m, f32 ang) {
    // Use PI Macro later
    s16 v = 16384.0f * ((180.0f * ang) / 3.141f) / 90.0f;

    f32 cos = JMASCos(v);
    f32 sin = JMASSin(v);

    m[0][0] = 1.0f;
    m[1][0] = 0.0f;
    m[2][0] = 0.0f;

    m[0][1] = 0.0f;
    m[1][1] = cos;
    m[2][1] = sin;

    m[0][2] = 0.0f;
    m[1][2] = -sin;
    m[2][2] = cos;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

void KartMat::RotZMatrix(Mtx m, f32 ang) {
    // Use PI Macro later
    s16 v = 16384.0f * ((180.0f * ang) / 3.141f) / 90.0f;

    f32 cos = JMASCos(v);
    f32 sin = JMASSin(v);

    m[0][0] = cos;
    m[1][0] = sin;
    m[2][0] = 0.0f;

    m[0][1] = -sin;
    m[1][1] = cos;
    m[2][1] = 0.0f;

    m[0][2] = 0.0f;
    m[1][2] = 0.0f;
    m[2][2] = 1.0f;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

void KartMat::AddMatrix(Mtx m, Mtx other) {
    m[0][0] += other[0][0];
    m[1][0] += other[1][0];
    m[2][0] += other[2][0];

    m[0][1] += other[0][1];
    m[1][1] += other[1][1];
    m[2][1] += other[2][1];

    m[0][2] += other[0][2];
    m[1][2] += other[1][2];
    m[2][2] += other[2][2];
}

void KartMat::NormalizeMatrix(Mtx m) {

    f32 x = m[0][2];
    f32 y = m[1][2];
    f32 z = m[2][2];
    f32 len = x * x + y * y + z * z;

    if (len == 0.0f) {
        m[0][2] = 0.0f;
        m[1][2] = 0.0f;
        m[2][2] = 0.0f;
    }
    else {
        len = 1.0f/KartMath::SpeedySqrtf(len);
        m[0][2] *= len;
        m[1][2] *= len;
        m[2][2] *= len;
    }

    Vec ab, a, b;

    SetVector(&a, m[0][1], m[1][1], m[2][1]);
    SetVector(&b, m[0][2], m[1][2], m[2][2]);
    PSVECCrossProduct(&a, &b, &ab);

    m[0][0] = ab.x;
    m[1][0] = ab.y;
    m[2][0] = ab.z;

    x = m[0][0];
    y = m[1][0];
    z = m[2][0];
    len = x * x + y * y + z * z;
    if (len == 0.0f)
    {
        m[0][0] = 0.0f;
        m[1][0] = 0.0f;
        m[2][0] = 0.0f;
    }
    else
    {
        len = 1.0f / KartMath::SpeedySqrtf(len);
        m[0][0] *= len;
        m[1][0] *= len;
        m[2][0] *= len;
    }

    SetVector(&a, m[0][2], m[1][2], m[2][2]);
    SetVector(&b, m[0][0], m[1][0], m[2][0]);
    PSVECCrossProduct(&a, &b, &ab);

    m[0][1] = ab.x;
    m[1][1] = ab.y;
    m[2][1] = ab.z;
}

void KartMat::OmegaMatrix(Mtx m, VecPtr vec, f32 w)
{
    m[0][0] = 0.0f;
    m[1][0] = vec->z;
    m[2][0] = -vec->y;

    m[0][1] = -vec->z;
    m[1][1] = 0.0f;
    m[2][1] = vec->x;

    m[0][2] = vec->y;
    m[1][2] = -vec->x;
    m[2][2] =  0.0f;

    m[0][3] = 0.0;
    m[1][3] = w;
    m[2][3] = 0.0f;
}
