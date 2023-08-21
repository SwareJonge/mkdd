#include "Yamamoto/KartMat.h"
#include "Yamamoto/KartMath.h"
#include "Yamamoto/KartVec.h"

#include "std/math.h"

// TODO: hire a mathematician to properly name variables

// Unused
void KartVec::SetZeroVector(VecPtr vec)
{
    vec->x = 0.0f;
    vec->y = 0.0f;
    vec->z = 0.0f;
}

void KartVec::SetVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void KartVec::AddVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x += x;
    vec->y += y;
    vec->z += z;
}

void KartVec::SubVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x -= x;
    vec->y -= y;
    vec->z -= z;
}

void KartVec::MulVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x *= x;
    vec->y *= y;
    vec->z *= z;
}

void KartVec::DevVector(VecPtr vec, f32 x, f32 y, f32 z)
{
    vec->x /= x;
    vec->y /= y;
    vec->z /= z;
}

f32 KartVec::VectorLength(VecPtr vec)
{
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

f32 KartVec::VectorLengthSqrtf(VecPtr vec)
{
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

// Unused, size match, not confirmed matching, needed for float ordering
void KartVec::NormalizeVector(VecPtr vec)
{
    f32 len = 1.0f / KartMath::SpeedySqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
    vec->x *= len;
    vec->y *= len;
    vec->z *= len;
}

void KartVec::VectorElement(JGeometry::TVec3f *out, JGeometry::TVec3f *vec1, JGeometry::TVec3f *vec2)
{
    f32 len = VectorLength((VecPtr)vec1);
    if (!vec1->isZero())
    {
        f32 inner = InnerCalc((VecPtr)vec2, (VecPtr)vec1) / len;
        out->x = vec2->x * inner;
        out->y = vec2->y * inner;
        out->z = vec2->z * inner;
    }
    else
    {
        out->zero();
    }
}

f32 KartMath::SpeedySqrtf(f32 x)
{
    volatile f32 xf;
    if (x > 0.0f)
    {
        xf = __frsqrte(x) * x;
        return xf;
    }
    return x;
}

void KartMath::CrossToPolar(JGeometry::TVec3f *vec1, JGeometry::TVec3f *vec2, f32 *pLength, f32 *pAng1, f32 *pAng2)
{
    f32 divX = (vec2->x - vec1->x); // i hate this codec!
    f32 divY = (vec2->y - vec1->y);
    f32 divZ = (vec2->z - vec1->z);

    f32 len = SpeedySqrtf(divX * divX + divY * divY + divZ * divZ);

    *pLength = len;
    *pAng1 = std::atan2f(SpeedySqrtf(divX * divX + divZ * divZ), divY);
    *pAng2 = std::atan2f(divZ, divX);
}

void KartMath::PolarToCross(VecPtr base, VecPtr out, f32 x, f32 y, f32 z)
{
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

f32 KartMath::lu(Mtx33 m, int *pivot)
{
    // TODO: rename everything, get rid of hacky matches, figure out if this is matrix43
    // release https://decomp.me/scratch/YnEQ9
    // debug: https://decomp.me/scratch/XSADp
    // f1: ret
    // f2: tmp
    // f3: max_val
    // f4: 0.0f
    // f5: row_val
    f32 tmp[3];
    f32 rowMaxValue;
    f32 scaleFactor;
    f32 rowValue;
    f32 ret = 0.0f;

    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        pivot[i] = i;
        rowMaxValue = 0.0f;

        for (k = 0; k < 3; k++)
        {
            f32 absval = std::fabs(m[i][k]);
            if (absval > rowMaxValue)
            {
                rowMaxValue = absval;
            }
        }
        if (ret == rowMaxValue)
        {
            goto hell;
        }

        tmp[i] = 1.0f / rowMaxValue;
    }

    ret = 1.0f;

    for (i = 0; i < 3; i++)
    {
        rowMaxValue = -1.0f;
        for (j = i; j < 3; j++)
        {
            int pk = pivot[j];
            f32 absval = tmp[pk] * std::fabs(m[pk][i]);
            if (absval > rowMaxValue)
            {
                rowMaxValue = absval;
                k = j;
            }
        }

        int row = pivot[k];

        if (k != i)
        { // swap pivot rows
            ret = -ret;
            pivot[k] = pivot[i];
            pivot[i] = row;
        }

        rowValue = m[row][i];
        ret *= rowValue;

        if (0.0f == rowValue)
            break;

        for (j = i; ++j < 3;)
        {
            int prow = pivot[j];
            scaleFactor = m[prow][i] / rowValue;
            m[prow][i] = scaleFactor;

            for (k = i; ++k < 3;)
            {
                m[prow][k] -= scaleFactor * m[row][k];
            }
        }
    }
hell:
    return ret;
}

void KartMath::solve(Mtx33 m, f32 *a, int *pivot, f32 *b, int n)
{
    int i, j;
    // Solve Ly
    for (i = 0; i < n; i++)
    {
        int pivot_row = pivot[i];
        f32 val = a[pivot_row];

        for (j = 0; j < i; j++)
        {
            val -= b[j] * m[pivot_row][j];
        }
        b[i] = val;
    }

    // Solve Ux
    for (i = n; --i >= 0;)
    {
        int pivot_row = pivot[i];
        f32 val = b[i];

        for (j = i + 1; j < n; j++)
        {
            val -= b[j] * m[pivot_row][j];
        }
        b[i] = val / m[pivot_row][i];
    }
}

f32 KartMath::Gauss(Mtx33 a, f32 *b, f32 *x)
{
    int pivot[4]; // 3 or 4?
    f32 max_val = lu(a, pivot);

    if (max_val != 0.0f)
    {
        solve(a, b, pivot, x, 3);
    }
    return max_val;
}

f32 KartMath::LimmtNumber(f32 f1, f32 f2)
{
    if (f1 < -f2)
    {
        return -f2;
    }
    if (f1 > f2)
    {
        return f2;
    }
    return f1;
}

int KartMath::cnvge(int x, int y, int z, int w)
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

void KartMath::ChaseFnumber(f32 *out, f32 x, f32 y)
{
    *out += y * (x - *out);

    if (*out < 0.001 && *out > -0.001)
    {
        *out = 0.0f;
    }
}

void KartMath::ChaseFcnvge(f32 *outf, u16 *outi, f32 x, u16 y)
{
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

void KartMat::ClearRotMatrix(Mtx mtx)
{
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

void KartMat::SetPosePosMatrix(Mtx out, Mtx in, VecPtr vec)
{
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

void KartMat::SetPosMatrix(Mtx out, VecPtr vec)
{
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

void KartMat::DevMatrixByVector(VecPtr out, VecPtr base, Mtx m)
{
    f32 x = m[0][0] * base->x + m[1][0] * base->y + m[2][0] * base->z;
    f32 y = m[0][1] * base->x + m[1][1] * base->y + m[2][1] * base->z;
    f32 z = m[0][2] * base->x + m[1][2] * base->y + m[2][2] * base->z;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::DevMatrixByVector2(JGeometry::TVec3f *out, JGeometry::TVec3f *base, Mtx m)
{
    f32 x = m[0][0] * base->x + m[1][0] * base->y + m[2][0] * base->z;
    f32 y = m[0][1] * base->x + m[1][1] * base->y + m[2][1] * base->z;
    f32 z = m[0][2] * base->x + m[1][2] * base->y + m[2][2] * base->z;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::DevCrdMatrixByVector(VecPtr out, VecPtr base, Mtx m)
{
    f32 divX = (base->x - m[0][3]);
    f32 divY = (base->y - m[1][3]);
    f32 divZ = (base->z - m[2][3]);

    f32 x = m[0][0] * divX + m[1][0] * divY + m[2][0] * divZ;
    f32 y = m[0][1] * divX + m[1][1] * divY + m[2][1] * divZ;
    f32 z = m[0][2] * divX + m[1][2] * divY + m[2][2] * divZ;

    out->x = x;
    out->y = y;
    out->z = z;
}

void KartMat::MulMatrix(Mtx m, Mtx m2, Mtx m3)
{
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

void KartMat::RotYMatrix(Mtx m, f32 ang)
{
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

void KartMat::RotYMatrix33(Mtx m, f32 ang)
{
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

void KartMat::RotXMatrix(Mtx m, f32 ang)
{
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

void KartMat::RotZMatrix(Mtx m, f32 ang)
{
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

void KartMat::AddMatrix(Mtx m, Mtx other)
{
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

void KartMat::NormalizeMatrix(Mtx m)
{
    f32 x = m[0][2];
    f32 y = m[1][2];
    f32 z = m[2][2];
    f32 len = x * x + y * y + z * z;

    if (len == 0.0f)
    {
        m[0][2] = 0.0f;
        m[1][2] = 0.0f;
        m[2][2] = 0.0f;
    }
    else
    {
        len = 1.0f / KartMath::SpeedySqrtf(len); // new variable?
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
    m[2][2] = 0.0f;

    m[0][3] = 0.0;
    m[1][3] = w;
    m[2][3] = 0.0f;
}
