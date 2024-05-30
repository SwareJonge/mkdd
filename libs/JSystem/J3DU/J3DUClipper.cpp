#include <stdio.h>
#include <std/math.h>
#include "JSystem/J3DU/J3DUClipper.h"
#include "JSystem/JUtility/JUTDbPrint.h"

J3DUClipper::J3DUClipper(f32 fovY, f32 aspect, f32 near, f32 far)
{
    // UNUSED
    mFovY = fovY;
    mAspect = aspect;
    mNear = near;
    mFar = far;
}

void J3DUClipper::init()
{
    mNear = 1.0f;
    mFar = 100000.0f;
}

// NOTE: needs to be just tanf
void J3DUClipper::calcViewFrustum()
{
    // Calculate dimensions of the near plane
    f32 nearHeight = mNear * std::tanf(MTXDegToRad(0.5f * mFovY)); // Half height of the near plane
    f32 nearWidth = mAspect * nearHeight;                          // Half width of the near plane

    Vec unused = {0, 0, 0};

    // Define the 4 corners of the near plane
    Vec bottomLeft = {-nearWidth, -nearHeight, -mNear};
    Vec topLeft = {-nearWidth, nearHeight, -mNear};
    Vec topRight = {nearWidth, nearHeight, -mNear};
    Vec bottomRight = {nearWidth, -nearHeight, -mNear};

    // Calculate the frustum plane normals using cross products
    J3DVecCrossProduct(&topLeft, &bottomLeft, &mLeftPlane);
    J3DVecCrossProduct(&topRight, &topLeft, &mRightPlane);
    J3DVecCrossProduct(&bottomRight, &topRight, &mTopPlane);
    J3DVecCrossProduct(&bottomLeft, &bottomRight, &mBottomPlane);

    // Normalize the plane normals
    PSVECNormalize(&mLeftPlane, &mLeftPlane);
    PSVECNormalize(&mRightPlane, &mRightPlane);
    PSVECNormalize(&mTopPlane, &mTopPlane);
    PSVECNormalize(&mBottomPlane, &mBottomPlane);
}

int J3DUClipper::clip(const Mtx transMtx, Vec point, f32 buffer) const
{
    Vec transformedPoint;
    PSMTXMultVec(transMtx, &point, &transformedPoint);

    // Check against near and far planes
    if (-transformedPoint.z < mNear - buffer)
    {
        return 1;
    }

    if (-transformedPoint.z > mFar + buffer)
    {
        return 1;
    }

    // Check against the four side planes of the frustum
    if (transformedPoint.x * mLeftPlane.x + transformedPoint.y * mLeftPlane.y + transformedPoint.z * mLeftPlane.z > buffer)
    {
        return 1;
    }

    if (transformedPoint.x * mRightPlane.x + transformedPoint.y * mRightPlane.y + transformedPoint.z * mRightPlane.z > buffer)
    {
        return 1;
    }

    if (transformedPoint.x * mTopPlane.x + transformedPoint.y * mTopPlane.y + transformedPoint.z * mTopPlane.z > buffer)
    {
        return 1;
    }

    if (transformedPoint.x * mBottomPlane.x + transformedPoint.y * mBottomPlane.y + transformedPoint.z * mBottomPlane.z > buffer)
    {
        return 1;
    }

    return 0;
}

int J3DUClipper::clip(const Mtx transMtx, Vec *minCorner, Vec *maxCorner) const
{
    // Array to count the number of vertices outside each frustum plane
    int outsideCount[6];
    for (int i = 0; i < 6; i++)
    {
        outsideCount[i] = 0;
    }

    // Array to store the 8 vertices of the bounding box
    Vec vertices[8];
    vertices[0].x = maxCorner->x;
    vertices[0].y = maxCorner->y;
    vertices[0].z = minCorner->z;
    vertices[1].x = maxCorner->x;
    vertices[1].y = maxCorner->y;
    vertices[1].z = maxCorner->z;
    vertices[2].x = minCorner->x;
    vertices[2].y = maxCorner->y;
    vertices[2].z = maxCorner->z;
    vertices[3].x = minCorner->x;
    vertices[3].y = maxCorner->y;
    vertices[3].z = minCorner->z;
    vertices[4].x = maxCorner->x;
    vertices[4].y = minCorner->y;
    vertices[4].z = minCorner->z;
    vertices[5].x = maxCorner->x;
    vertices[5].y = minCorner->y;
    vertices[5].z = maxCorner->z;
    vertices[6].x = minCorner->x;
    vertices[6].y = minCorner->y;
    vertices[6].z = maxCorner->z;
    vertices[7].x = minCorner->x;
    vertices[7].y = minCorner->y;
    vertices[7].z = minCorner->z;

    // Check each vertex against the frustum planes
    for (u32 i = 0; i < 8; i++)
    {
        Vec transformedVertex;
        PSMTXMultVec(transMtx, &vertices[i], &transformedVertex);
        int outOfFrustum = 0;

        // Check against near plane
        if (-transformedVertex.z < mNear)
        {
            outOfFrustum++;
            outsideCount[4]++;
        }

        // Check against far plane
        if (-transformedVertex.z > mFar)
        {
            outOfFrustum++;
            outsideCount[5]++;
        }

        // Check against left plane
        if (transformedVertex.x * mLeftPlane.x + transformedVertex.y * mLeftPlane.y + transformedVertex.z * mLeftPlane.z > 0.0f)
        {
            outOfFrustum++;
            outsideCount[0]++;
        }

        // Check against right plane
        if (transformedVertex.x * mRightPlane.x + transformedVertex.y * mRightPlane.y + transformedVertex.z * mRightPlane.z > 0.0f)
        {
            outOfFrustum++;
            outsideCount[1]++;
        }

        // Check against top plane
        if (transformedVertex.x * mTopPlane.x + transformedVertex.y * mTopPlane.y + transformedVertex.z * mTopPlane.z > 0.0f)
        {
            outOfFrustum++;
            outsideCount[2]++;
        }

        // Check against bottom plane
        if (transformedVertex.x * mBottomPlane.x + transformedVertex.y * mBottomPlane.y + transformedVertex.z * mBottomPlane.z > 0.0f)
        {
            outOfFrustum++;
            outsideCount[3]++;
        }

        // If any vertex is inside all planes, the box is not completely outside
        if (outOfFrustum == 0)
        {
            return 0;
        }
    }

    // If all vertices are outside any single plane, the box is completely outside the frustum
    if (outsideCount[0] == 8)
    {
        return 1;
    }
    if (outsideCount[2] == 8)
    {
        return 1;
    }
    if (outsideCount[1] == 8)
    {
        return 1;
    }
    if (outsideCount[3] == 8)
    {
        return 1;
    }
    if (outsideCount[4] == 8)
    {
        return 1;
    }
    if (outsideCount[5] == 8)
    {
        return 1;
    }

    return 0;
}

void J3DUClipper::report() const {}

void J3DUClipper::debugPrint(JUTDbPrint *print) const // not sure what print function is being used here
{
    char buf[256];
    sprintf(buf, " J3DUClipper::mFovy = %f", mFovY);
    //print->getFont()->drawString(20, 20, buf, true);
    sprintf(buf, " J3DUClipper::mAspect = %f", mAspect);
    //print->getFont()->drawString(20, 20, buf, true);
    sprintf(buf, " J3DUClipper::mNear = %f", mNear);
    //print->getFont()->drawString(20, 20, buf, true);
    sprintf(buf, " J3DUClipper::mFar = %f", mFar);
    //print->getFont()->drawString(20, 20, buf, true);
}