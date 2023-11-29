#include <JSystem/J3D/J3DSys.h>
#include <JSystem/JKernel/JKRHeap.h>
#include "Kaneshige/DrawBuffer.h"

J3DDrawBuffer DrawBuffer::sDummyBuf;

DrawBuffer::DrawBuffer()
{
    mLockFlag = 0;
    mOpaBuf = nullptr;
    mXluBuf = nullptr;    
}

DrawBuffer::DrawBuffer(u32 bufSize)
{
    mLockFlag = 0;
    mDrawFlag = 1;
    mOpaBuf = nullptr;
    mXluBuf = nullptr;    
    create(bufSize, bufSize);
}

void DrawBuffer::reset()
{
    mLockFlag = 0;
    mDrawFlag = 1;
    frameInit();
}

DrawBuffer::~DrawBuffer()
{
    if (mOpaBuf)
        delete mOpaBuf;
    if (mXluBuf)
        delete mXluBuf;
}

void DrawBuffer::create(u32 opaBufSize, u32 xluBufSize)
{
    mOpaBuf = new (32) J3DDrawBuffer(opaBufSize);
    mXluBuf = new (32) J3DDrawBuffer(xluBufSize);

    mXluBuf->setDrawMode(1);
}

void DrawBuffer::drawIn(u32 idx) {}

void DrawBuffer::draw(u32 idx)
{
    mDrawFlag = 1;
    drawIn(idx);

    if (isDraw())
    {
        j3dSys.setDrawModeOpaTexEdge();
        getOpaBuf()->draw();
        j3dSys.setDrawModeXlu();
        getXluBuf()->draw();
    }
}

void DrawBuffer::drawOpa(u32 idx)
{
    mDrawFlag = 1;
    drawIn(idx);

    if (isDraw())
    {
        j3dSys.setDrawModeOpaTexEdge();
        getOpaBuf()->draw();
    }
}

void DrawBuffer::drawXlu(u32 idx)
{
    if (isDraw())
    {
        j3dSys.setDrawModeXlu();
        getXluBuf()->draw();
    }
}

void DrawBuffer::update()
{
    J3DDrawBuffer *opaBuf;
    J3DDrawBuffer *xluBuf;

    if (!isLock())
    {
        opaBuf = getOpaBuf();
        xluBuf = getXluBuf();
        frameInit();
    }
    else
    {
        opaBuf = &sDummyBuf;
        xluBuf = &sDummyBuf;
    }
    j3dSys.setDrawBuffer(opaBuf, 0);
    j3dSys.setDrawBuffer(xluBuf, 1);
}

void DrawBuffer::viewCalc(u32 idx) {}