#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JGeometry/Vec.h"

f32 JPAConvertFixToFloat(s16) {
    return 1.0f;
    // UNUSED FUNCTION
}

void JPAGetDirMtx(const JGeometry::TVec3f &vec, Mtx m) {
    Vec *v = (Vec *)&vec; // fakematch?
    JGeometry::TVec3f newVec;
    newVec.set(vec.y, -vec.x, 0.0f);
    f32 mag = newVec.length();
    if (mag <= JGeometry::TUtilf::epsilon()) {
        newVec.zero();
    } else {
        newVec.scale(1.0f / mag);
    }

    const f32 xSq  = newVec.x * newVec.x; // f30
    const f32 ySq  = newVec.y * newVec.y; // f29
    const f32 xLen = newVec.x * mag; // f28
    const f32 yLen = newVec.y * mag; // f27
    const f32 xyz  = (1.0f - vec.z) * (newVec.x * newVec.y); // f26

    m[0][0] = xSq + vec.z * (1.0f - xSq);
    m[0][1] = xyz;
    m[0][2] = -yLen;
    m[0][3] = 0.0f;

    m[1][0] = xyz;
    m[1][1] = ySq + vec.z * (1.0f - ySq);
    m[1][2] = xLen;
    m[1][3] = 0.0f;

    m[2][0] = yLen;
    m[2][1] = -xLen;
    m[2][2] = v->z;
    m[2][3] = 0.0f;
}

void JPAGetYZRotateMtx(s16 angleY, s16 angleZ, Mtx m) {
    f32 cosy  = JMASCos(angleY);
    f32 cosz  = JMASCos(angleZ);
    f32 siny  = JMASSin(angleY);
    f32 sinz  = JMASSin(angleZ);
    m[0][0] = (cosy * cosz);
    m[0][1] = -sinz;
    m[0][2] = (siny * cosz);
    m[1][0] = (cosy * sinz);
    m[1][1] = cosz;
    m[1][2] = (siny * sinz);
    m[2][0] = -siny;
    m[2][2] = cosy;
    m[2][3] = 0.0f;
    m[2][1] = 0.0f;
    m[1][3] = 0.0f;
    m[0][3] = 0.0f;
}

void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, Mtx m) {
    f32 cx = JMASCos(x);
    f32 cy = JMASCos(y);
    f32 cz = JMASCos(z);

    f32 sx = JMASSin(x);
    f32 sy = JMASSin(y);
    f32 sz = JMASSin(z);

    m[0][0] = cy * cz;
    m[1][0] = cy * sz;

    m[2][0] = -sy;
    m[2][1] = sx * cy;
    m[2][2] = cx * cy;

    f32 cxsz = cx * sz;
    f32 sxcz = sx * cz;

    m[0][1] = sxcz * sy - cxsz;
    m[1][2] = cxsz * sy - sxcz;

    f32 sxsz = sx * sz;
    f32 cxcz = cx * cz;
    m[0][2] = sxsz + cxcz * sy;
    m[1][1] = cxcz + sxsz * sy;

    m[2][3] = 0.0f;
    m[1][3] = 0.0f;
    m[0][3] = 0.0f;
}

void JPASetRMtxfromMtx(const Mtx src, Mtx dst) {
    JGeometry::TVec3f v1, v2;
    JPASetRMtxSTVecfromMtx(src, dst, &v1, &v2);
}

void JPASetRMtxTVecfromMtx(const Mtx src, Mtx dst, JGeometry::TVec3f *v2) {
    JGeometry::TVec3f v;
    JPASetRMtxSTVecfromMtx(src, dst, &v, v2);
}

void JPASetRMtxSTVecfromMtx(const Mtx m, Mtx RMtx, JGeometry::TVec3f* lengths, JGeometry::TVec3f* translation) {
    JGeometry::TVec3f tempVec;
    tempVec.set(m[0][0], m[1][0], m[2][0]);
    lengths->x = tempVec.length();
    tempVec.set(m[0][1], m[1][1], m[2][1]);
    lengths->y = tempVec.length();
    tempVec.set(m[0][2], m[1][2], m[2][2]);
    lengths->z = tempVec.length();
    PSMTXIdentity(RMtx);
    if (lengths->x != 0.0f) {
        f32 fVar5  = 1.0f / lengths->x;
        RMtx[0][0] = m[0][0] * fVar5;
        RMtx[1][0] = m[1][0] * fVar5;
        RMtx[2][0] = m[2][0] * fVar5;
    }
    if (lengths->y != 0.0f) {
        f32 fVar5  = 1.0f / lengths->y;
        RMtx[0][1] = m[0][1] * fVar5;
        RMtx[1][1] = m[1][1] * fVar5;
        RMtx[2][1] = m[2][1] * fVar5;
    }
    if (lengths->z != 0.0f) {
        f32 fVar5  = 1.0f / lengths->z;
        RMtx[0][2] = m[0][2] * fVar5;
        RMtx[1][2] = m[1][2] * fVar5;
        RMtx[2][2] = m[2][2] * fVar5;
    }

    translation->set(m[0][3], m[1][3], m[2][3]);
}

f32 JPACalcKeyAnmValue(f32 currentFrame, u16 keyFrameCount, const f32* keyFrameData) {
    if (currentFrame < keyFrameData[0]) {
        return keyFrameData[1];
    }

    int lastKeyFrameIndex = keyFrameCount - 1;
    if (keyFrameData[lastKeyFrameIndex * 4] <= currentFrame) {
        return keyFrameData[lastKeyFrameIndex * 4 + 1];
    }

    int frame = keyFrameCount;
    while (frame > 1) {
        u32 halfIndex = frame / 2;
        if (currentFrame >= keyFrameData[halfIndex * 4]) {
            keyFrameData += halfIndex * 4;
            frame -= halfIndex;
        } else {
            frame = halfIndex;
        }
    }

    return JMAHermiteInterpolation(currentFrame, keyFrameData[0], keyFrameData[1], keyFrameData[3], keyFrameData[4], keyFrameData[5],
                                   keyFrameData[6]);
}
