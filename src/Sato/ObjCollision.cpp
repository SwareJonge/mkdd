#include "Sato/ObjCollision.h"
#include "JSystem/JGeometry/Vec.h"
#include "Sato/stMath.h"
#include "mathHelper.h"

// UNUSED, size match, inlines into other constructors
void ObjColBase::initialize()
{
    mPos.zero();
    mBoundDepth = 0.0f;
}

// UNUSED, size match, needed for sdata ordering
ObjColBase::ObjColBase(J3DModelData *modelData, ObjColBase::CKind kind) : mKind(kind), mScale(1.0f)
{
    if (!modelData) { return; }

    initRadius(modelData);
    initialize();
}

ObjColBase::ObjColBase(J3DModelData *modelData, JGeometry::TVec3f scale, CKind kind) : mKind(kind)
{
    if (!modelData) { return; }

    initRadius(modelData);
    setScale(scale);
    initialize();
}

// UNUSED, size match
ObjColBase::ObjColBase(J3DModelData *modelData, f32 scale, ObjColBase::CKind kind) : mKind(kind), mScale(scale)
{
    if (!modelData) { return; }

    initRadius(modelData);
    initialize();
}

void ObjColBase::initRadius(J3DModelData *model)
{
    f32 maxRadius = 0.0f;

    int nJoints = 3;
    if (nJoints >= model->mJointTree.mJointCnt) { nJoints = model->mJointTree.mJointCnt; }

    for (u8 i = 0; i < nJoints; i++) {
        if (maxRadius < model->mJointTree.mJoints[i]->mRadius) { maxRadius = model->mJointTree.mJoints[i]->mRadius; }
    }
    mRadius = maxRadius;
    mPos.zero();
}

ObjColBase::ObjColBase(f32 radius, f32 scale, CKind kind)
{
    mRadius = radius;
    mKind = kind;
    mScale = scale;
    mPos.zero();
    mBoundDepth = 0.0f;
}

void ObjColBase::setScale(const JGeometry::TVec3f &scale)
{
    f32 s = scale.x;
    if (scale.y > s) { s = scale.y; }
    if (scale.z > s) { s = scale.z; }
    mScale = s;
}

u8 ObjColBase::IsHitBoundPos(JGeometry::TVec3f objPos, JGeometry::TVec3f boundPos)
{
    JGeometry::TVec3f deltaPos;
    deltaPos.sub(objPos, boundPos);

    u8 isHit = 0;
    f32 minSeparation = mRadius * mScale;
    if (deltaPos.squared() < minSeparation * minSeparation) { isHit = 1; }
    return isHit;
}

bool ObjColSphere::IsHitSphere(JGeometry::TVec3f posOther, JGeometry::TVec3f posThis, f32 otherRadius)
{
    bool isHit = false;
    mPos.sub(posThis, posOther);
    f32 minSeparationSq, distanceSq;
    if (checkRadialCollisionXYZ(otherRadius, mPos, minSeparationSq, distanceSq)) {
        f32 minSeparation = JMAFastSqrt(minSeparationSq);
        f32 distance = JMAFastSqrt(distanceSq);
        mBoundDepth = minSeparation - distance;
        stVecNormalize(mPos);
        isHit = true;
    }
    return isHit;
}

bool ObjColSphere::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther,
        const ObjColCylinder &cylinderCol)
{
    bool isHit = false;
    f32 sphereScaledRadius = mRadius * getScale();
    f32 deltaX, deltaZ, minSeparationSq, distanceSq;
    if (cylinderCol.checkRadialCollisionXZ(sphereScaledRadius, posThis, posOther,
                minSeparationSq, distanceSq, deltaX, deltaZ)) {
        if ((posThis.y - sphereScaledRadius < posOther.y + cylinderCol.getScaledHeight()) &&
                (posThis.y + sphereScaledRadius > posOther.y)) {
            f32 minSeparation = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparation - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

bool ObjColCylinder::IsHitSphere(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, f32 otherRadius)
{
    bool isHit = false;
    f32 deltaX, deltaZ, minSeparationSq, distanceSq;
    if (checkRadialCollisionXZ(otherRadius, posThis, posOther, minSeparationSq, distanceSq,
                deltaX, deltaZ) &&
            posOther.y + otherRadius > posThis.y) {
        if (posOther.y - otherRadius < (mCylinderHeight * mScale) + posThis.y) {
            f32 minSeparation = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparation - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

bool ObjColCylinder::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther,
        const ObjColCylinder &colOther)
{
    bool isHit = false;
    f32 deltaX, deltaZ, minSeparationSq, distanceSq;
    if (checkRadialCollisionXZ(colOther.mCylinderRadius * colOther.mScale, posThis, posOther,
                minSeparationSq, distanceSq, deltaX, deltaZ)) {
        if ((posThis.y < posOther.y + colOther.getScaledHeight()) &&
                (posThis.y + (mCylinderHeight * mScale) > posOther.y)) {
            f32 minSeparation = JMAFastSqrt(minSeparationSq);
            f32 distance = JMAFastSqrt(distanceSq);
            mBoundDepth = minSeparation - distance;
            mPos.set(deltaX, 0.0f, deltaZ);
            stVecNormalize(mPos);
            isHit = true;
        }
    }
    return isHit;
}

void ObjColCube::makeParameter(J3DModelData *modelData, Mtx matrix)
{
    // bounding-box verts
    J3DJoint *j = modelData->mJointTree.mJoints[0];
    JGeometry::TVec3f min(j->mMin);
    JGeometry::TVec3f max(j->mMax);

    mVertices[0].set(min.x, min.y, max.z);
    mVertices[1].set(min.x, min.y, min.z);
    mVertices[2].set(max.x, min.y, min.z);
    mVertices[3].set(max.x, min.y, max.z);
    mVertices[4].set(min.x, max.y, max.z);

    // transform verts
    JGeometry::TVec3f t[5];
    JGeometry::TVec3f *src = mVertices, *dst = t;
    for (u8 i = 0; i < 5; i++, src++, dst++) {
        PSMTXMultVec(matrix, src, dst);
    }

    JGeometry::TVec3f edges[5];
    edges[0].sub(t[1], t[0]);
    edges[1].sub(t[2], t[1]);
    edges[2].sub(t[3], t[2]);
    edges[3].sub(t[0], t[3]);
    edges[4].sub(t[4], t[0]);

    // edge lengths
    mDimensions[0] = edges[0].length();
    mDimensions[1] = edges[1].length();
    mDimensions[2] = edges[4].length();

    const u8 topIndex = 4;

    JGeometry::TVec3f normal, *edge = edges;
    for (u8 i = 0; i < topIndex; i++, edge++) {
        normal.cross(edges[topIndex], *edge);
        stMakePlaneParam(mSidePlanes[i], normal, t[i]);
    }

    stMakePlaneParam(mTopPlane, edges[topIndex], t[topIndex]);
}

void ObjColCube::updateParameter(Mtx matrix)
{
    // transform verts
    JGeometry::TVec3f t[5];
    JGeometry::TVec3f *src = mVertices, *dst = t;
    for (u8 i = 0; i < 5; i++, src++, dst++) {
        PSMTXMultVec(matrix, src, dst);
    }

    JGeometry::TVec3f normal;

    JGeometry::TVec3f *vertex = t;
    stPlaneParam *plane = mSidePlanes;
    for (u8 i = 0; i < 4; i++, plane++, vertex++) {
        normal.set(plane->x, plane->y, plane->z);
        stMakePlaneParam(*plane, normal, *vertex);
    }

    normal.set(mTopPlane.x, mTopPlane.y, mTopPlane.z);
    stMakePlaneParam(mTopPlane, normal, t[4]);
}

bool ObjColCube::chkIsHitQuad(const JGeometry::TVec3f &spherePos, const f32 &radius)
{
    f32 penetrations[4];
    for (u8 i = 0; i < 4; i++) {
        f32 depth;
        if (stCollideSurfaceAndSphere(spherePos, radius, mSidePlanes[i], depth) == 1) {
            penetrations[i] = depth;
        } else {
            return false;
        }
    }
    u8 closeFaces[4];
    u8 farFaces[2];
    u8 mDimIndices[2] = {1, 0};

    if (penetrations[0] > penetrations[2]) {
        closeFaces[0] = 0;
        farFaces[0] = 2;
    } else {
        closeFaces[0] = 2;
        farFaces[0] = 0;
    }

    if (penetrations[1] > penetrations[3]) {
        closeFaces[1] = 1;
        farFaces[1] = 3;
    } else {
        closeFaces[1] = 3;
        farFaces[1] = 1;
    }

    for (u8 i = 0; i < 2; i++) {
        f32 adjusted = penetrations[closeFaces[i]] - (2.0f * radius);
        if (adjusted > mDimensions[mDimIndices[i]]) { return false; }
    }

    u8 farFaceIdx;
    if (penetrations[farFaces[0]] > penetrations[farFaces[1]]) {
        farFaceIdx = 1;
    } else {
        farFaceIdx = 0;
    }

    u8 minFaceIdx = farFaces[farFaceIdx];

    stPlaneParam &plane = mSidePlanes[minFaceIdx];
    mPos.set(plane.x, plane.y, plane.z);
    mBoundDepth = penetrations[minFaceIdx];

    return true;
}

bool ObjColCube::IsHitSphere(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, f32 radius)
{
    JGeometry::TVec3f deltaPos;
    deltaPos.sub(posOther, posThis);

    f32 dotProduct = stDot(mTopPlane, deltaPos);

    if ((dotProduct < 0.0f && -dotProduct > radius) || dotProduct > mDimensions[2] + radius) { return false; }

    return chkIsHitQuad(posOther, radius);
}

bool ObjColCube::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, const ObjColCylinder &cylinder)
{
    f32 cylScale = cylinder.getScale();
    f32 cylHeight = cylScale * cylinder.getCylinderHeight();
    f32 thisY = posThis.y;
    f32 cylY = posOther.y;

    if ((thisY > cylY + cylHeight) || (thisY + mDimensions[2] < cylY)) { return false; }

    return chkIsHitQuad(posOther, cylScale * cylinder.getCylinderRadius());
}

bool ExObjColBase::IsHitBound(JGeometry::TVec3f objPos, JGeometry::TVec3f boundPos)
{
    bool isHit = false;
    if (IsHitBoundPos(objPos, boundPos)) {
        u8 nHits = 0;
        if (boundPos.y < mBottom) { return 0; }
        for (u8 i = 0; i < 4; i++) {
            nHits += stSearchInSurface(boundPos, mSidePlanes[i]);
        }
        if (nHits == 4) { isHit = true; }
    }
    return isHit;
}

u8 ExObjColBase::IsHitBoundRadius(JGeometry::TVec3f pos, f32 radius)
{
    u8 isHit = 0;
    for (u8 i = 0; i < 4; i++) {
        if (stCollideSurfaceAndSphere(pos, radius, mSidePlanes[i], mColDepth)) {
            isHit = 1;
            break;
        }
    }
    return isHit;
}

void ExObjColBase::setBottom(f32 bottom)
{
    mBottom = bottom - 100.0f;
}

// UNUSED, needed to generate assert string in .rodata
// educated guess at the exact code - size matches (0xd0)
void ExObjColBase::setWallParam(int index)
{
    JUT_MINMAX_ASSERT(0, index, 4);

    const JGeometry::TVec3f upVector(0.0f, 1.0f, 0.0f);

    JGeometry::TVec3f normal;
    
    normal.cross(upVector, mWallNormal);
    normal.y = 0.0f;
    stMakePlaneParam(mSidePlanes[index], normal, upVector);
}

void ObjColJump1::makeParameter(J3DModelData *modelData, Mtx matrix)
{
    JGeometry::TVec3f vertices[6];
    vertices[0].set(249.3f, 2.25f, -500.6f);
    vertices[1].set(249.3f, 2.25f, 500.6f);
    vertices[2].set(-249.3f, 2.25f, 500.6f);
    vertices[3].set(-249.3f, 2.25f, -500.6f);
    vertices[4].set(249.3f, 267.0f, 500.6f);
    vertices[5].set(-249.3f, 267.0f, 500.6f);

    // transform verts in-place
    JGeometry::TVec3f *v = vertices;
    for (u8 i = 0; i < 6; i++, v++) {
        PSMTXMultVec(matrix, v, v);
    }

    JGeometry::TVec3f edges[5];
    edges[0].sub(vertices[1], vertices[0]);
    edges[1].sub(vertices[2], vertices[1]);
    edges[2].sub(vertices[3], vertices[2]);
    edges[3].sub(vertices[0], vertices[3]);
    edges[4].sub(vertices[5], vertices[3]);

    const JGeometry::TVec3f upVector(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f normal;

    JGeometry::TVec3f *edge = edges;
    for (u8 i = 0; i < 4; i++, edge++) {
        normal.cross(upVector, *edge);
        normal.y = 0.0f;
        stMakePlaneParam(mSidePlanes[i], normal, vertices[i]);
    }

    normal.cross(edges[4], edges[3]);
    stMakePlaneParam(mTopPlane, normal, vertices[3]);

    normal.cross(edges[1], edges[0]);
    stMakePlaneParam(mBottomPlane, normal, vertices[1]);

    mDirection.sub(vertices[4], vertices[1]);
    mDirection.normalize();
}

f32 ObjColJump1::SearchWall(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2)
{
    mHitNum = 0;

    u32 surfaceIndices[4] = {1, 4, 2, 0};

    f32 t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[0]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 2; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices[i]]);
        }
        if (hitCount == 2) {
            mWallNormal.set(mSidePlanes[0].x, mSidePlanes[0].y, mSidePlanes[0].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[2]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 2; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices[i]]);
        }
        if (hitCount == 2) {
            mWallNormal.set(mSidePlanes[2].x, mSidePlanes[2].y, mSidePlanes[2].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    surfaceIndices[0] = 0;
    t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[1]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 3; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices[i]]);
        }
        if (hitCount == 3) {
            mWallNormal.set(mSidePlanes[1].x, mSidePlanes[1].y, mSidePlanes[1].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    return -1.0f;
}

void ObjColJump1::Search(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2)
{
    f32 t = SearchWall(pos1, pos2);
    if (mHitNum != 2) {
        mNormal.set(mTopPlane.x, mTopPlane.y, mTopPlane.z);
        stVecNormalize(mNormal);
        mWallNormal.zero();

        f32 planeEval = mTopPlane.x * pos2.x + mTopPlane.z * pos2.z + mTopPlane.d;
        mHeight = -planeEval / mTopPlane.y;

        mHitNum = 1;
        mAttr = 6;
    } else {
        mColDepth = stGetCollideDepthFromT(pos1, pos2, t);
        JGeometry::TVec3f delta;
        delta.sub(pos1, pos2);
    }
}

bool ObjColJump1::IsHitSphere(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, f32 radius)
{
    return false;
}

bool ObjColJump1::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, const ObjColCylinder &cylinder)
{
    return false;
}

// UNUSED, size 0x100
bool ObjColJump2::IsHitBound(JGeometry::TVec3f, JGeometry::TVec3f)
{
    return false;
}

// UNUSED, size 0x64c
void ObjColJump2::makeParameter(J3DModelData *, Mtx)
{
    // Need 2x12 0-bytes in .rodata somewhere in this area
    f32 unused1[3] = {0.0f, 0.0f, 0.0f};
    f32 unused2[3] = {0.0f, 0.0f, 0.0f};
}

// UNUSED, size 0x3b8
void ObjColJump2::Search(JGeometry::TVec3f, JGeometry::TVec3f) {}

// UNUSED, size 0x5c
void ObjColJump2::makeColParam(JGeometry::TVec3f, stPlaneParam, CrsGround::EAttr) {}

// UNUSED, size 0xac
u8 ObjColJump2::IsOnDashPlane(JGeometry::TVec3f)
{
    return mIsDashPlane;
}

bool ObjColJump2::IsHitSphere(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, f32 radius)
{
    return false;
}

bool ObjColJump2::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, const ObjColCylinder &cylinder)
{
    return false;
}

void ObjColBlock::makeParameter(J3DModelData *modelData, Mtx matrix)
{
    // bounding-box verts
    J3DJoint *joint = modelData->mJointTree.mJoints[0];
    JGeometry::TVec3f min(joint->mMin);
    JGeometry::TVec3f max(joint->mMax);

    JGeometry::TVec3f vertices[5];
    vertices[0].set(min.x, min.y, max.z);
    vertices[1].set(min.x, min.y, min.z);
    vertices[2].set(max.x, min.y, min.z);
    vertices[3].set(max.x, min.y, max.z);
    vertices[4].set(min.x, max.y, max.z);

    // transform verts in-place
    JGeometry::TVec3f *v = vertices;
    for (u8 i = 0; i < 5; i++, v++) {
        PSMTXMultVec(matrix, v, v);
    }

    JGeometry::TVec3f edges[5];
    edges[0].sub(vertices[1], vertices[0]);
    edges[1].sub(vertices[2], vertices[1]);
    edges[2].sub(vertices[3], vertices[2]);
    edges[3].sub(vertices[0], vertices[3]);
    edges[4].sub(vertices[4], vertices[0]);

    const JGeometry::TVec3f upVector(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f normal;
    JGeometry::TVec3f *edge = edges;
    for (u8 i = 0; i < 4; i++, edge++) {
        normal.cross(upVector, *edge);
        normal.y = 0.0f;
        stMakePlaneParam(mSidePlanes[i], normal, vertices[i]);
    }

    normal.cross(edges[1], edges[0]);
    stMakePlaneParam(mTopPlane, normal, vertices[4]);

    normal.cross(edges[0], edges[1]);
    stMakePlaneParam(mBottomPlane, normal, vertices[1]);

    f32 heightDiff = max.y - min.y;
    f32 matrixY = matrix[1][3];
    mBottom = matrixY - 0.5f * heightDiff;

    mDirection.sub(vertices[4], vertices[0]);
    mDirection.normalize();
}

f32 ObjColBlock::SearchWall(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2)
{
    mHitNum = 0;

    u32 surfaceIndices1[4] = {0, 2, 4, 0};

    f32 t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[3]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 3; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices1[i]]);
        }
        if (hitCount == 3) {
            mWallNormal.set(mSidePlanes[3].x, mSidePlanes[3].y, mSidePlanes[3].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[1]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 3; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices1[i]]);
        }
        if (hitCount == 3) {
            mWallNormal.set(mSidePlanes[1].x, mSidePlanes[1].y, mSidePlanes[1].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    u32 surfaceIndices2[4] = {1, 3, 4, 0};

    t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[0]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 3; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices2[i]]);
        }
        if (hitCount == 3) {
            mWallNormal.set(mSidePlanes[0].x, mSidePlanes[0].y, mSidePlanes[0].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    t = stCollideLineToPlaneIn(pos1, pos2, mSidePlanes[2]);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 3; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices2[i]]);
        }
        if (hitCount == 3) {
            mWallNormal.set(mSidePlanes[2].x, mSidePlanes[2].y, mSidePlanes[2].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            return t;
        }
    }

    u32 surfaceIndices3[4] = {0, 1, 2, 3};

    t = stCollideLineToPlaneIn(pos1, pos2, mBottomPlane);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 4; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices3[i]]);
        }
        if (hitCount == 4) {
            mWallNormal.set(mSidePlanes[2].x, mSidePlanes[2].y, mSidePlanes[2].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            mNormal.y *= -1.0f;
            return t;
        }
    }

    return -1.0f;
}

void ObjColBlock::Search(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2)
{
    f32 t = SearchWall(pos1, pos2);
    if (mHitNum != 2) {
        mNormal.set(mTopPlane.x, mTopPlane.y, mTopPlane.z);
        stVecNormalize(mNormal);
        mWallNormal.zero();

        f32 planeEval = mTopPlane.x * pos2.x + mTopPlane.z * pos2.z + mTopPlane.d;
        mHeight = -planeEval / mTopPlane.y;

        mHitNum = 1;
        mAttr = 1;
    } else {
        mColDepth = stGetCollideDepthFromT(pos1, pos2, t);
    }
}

bool ObjColBlock::IsHitSphere(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, f32 radius)
{
    return false;
}

bool ObjColBlock::IsHitCylinder(JGeometry::TVec3f posThis, JGeometry::TVec3f posOther, const ObjColCylinder &cylinder)
{
    return false;
}

// UNUSED, size 0x144
f32 ObjColBoard::SearchWall(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2)
{
    u32 surfaceIndices[4] = {0, 1, 2, 3};

    f32 t = stCollideLineToPlaneIn(pos1, pos2, mBottomPlane);
    if (0.0f <= t && 1.0f >= t) {
        u8 hitCount = 0;
        for (u8 i = 0; i < 4; i++) {
            hitCount += stSearchInSurface(pos2, mSidePlanes[surfaceIndices[i]]);
        }
        if (hitCount == 4) {
            mWallNormal.set(mSidePlanes[2].x, mSidePlanes[2].y, mSidePlanes[2].z);
            mHitNum = 2;
            mAttr = 2;
            mNormal = mDirection;
            mHeight = mBottom;
            mNormal.y *= -1.0f;
            return t;
        }
    }

    return -1.0f;
}

// UNUSED, size 0xf4
void ObjColBoard::Search(JGeometry::TVec3f pos1, JGeometry::TVec3f pos2) {}

#include "JSystem/JAudio/JASFakeMatch2.h"
