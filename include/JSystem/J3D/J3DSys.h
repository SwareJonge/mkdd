#ifndef _JSYSTEM_J3D_J3DSYS_H
#define _JSYSTEM_J3D_J3DSYS_H

#include <dolphin/mtx.h>
#include "JSystem/J3D/J3DPacket.h"
#include "JSystem/J3D/J3DTypes.h"
#include "JSystem/JGeometry.h"
#include "types.h"

struct J3DDrawBuffer;
struct J3DMatPacket;
struct J3DModel;
struct J3DMtxCalc;
struct J3DShapePacket;
struct J3DTexture;

struct J3DSys
{
    J3DSys();
    void drawInit();
    void reinitGX();
    void reinitGenMode();
    void reinitIndStages();
    void reinitLighting();
    void reinitPixelProc();
    void reinitTexture();
    void reinitTevStages();
    void reinitTransform();
    void loadPosMtxIndx(int, u16) const;
    void loadNrmMtxIndx(int, u16) const;
    void ErrorReport(J3DErrType) const;
    void setTexCacheRegion(GXTexCacheSize);

    void setDrawBuffer(J3DDrawBuffer *drawBuffer, int idx) { mDrawBuffer[idx] = drawBuffer; }
    void setDrawModeOpaTexEdge() { mDrawMode = 3; }
    void setDrawModeXlu() { mDrawMode = 4; }
    void setViewMtx(const Mtx mtx) { PSMTXCopy(mtx, mViewMtx); }
    Mtx &getViewMtx() { return mViewMtx; }
    J3DDrawBuffer *getDrawBuffer(int idx) { return mDrawBuffer[idx]; }

    Mtx mViewMtx;                  // _000
    J3DMtxCalc *mMtxCalc;          // _030
    u32 _34;                       // _034 /* bitfield */
    J3DModel *_38;                 // _038
    J3DMatPacket *mMatPacket;      // _03C
    J3DShapePacket *mShapePacket;  // _040
    u32 _44;                       // _044
    J3DDrawBuffer *mDrawBuffer[2]; // _048
    int mDrawMode;                 // _050, TODO: enum/defines
    u32 _54;                       // _054
    J3DTexture *_58;               // _058
    u8 _5C[4];                     // _05C
    u32 _60;                       // _060
    u8 _64[0xA0];                  // _064
    Mtx *_104;                     // _104
    Mtx *_108;                     // _108
    /*
     * These three might be part of a J3DVertexBuffer.
     * See setArray__15J3DVertexBufferCFv.
     */
    u32 _10C; // _10C
    u32 _110; // _110
    u32 _114; // _114 /* bitfield */

    Vec *_118;

    static Mtx mCurrentMtx;
    static JGeometry::TVec3f mCurrentS;
    static JGeometry::TVec3f mParentS;
    static u16 sTexCoordScaleTable[8][4];
};

extern J3DSys j3dSys;

extern s32 j3dDefaultViewNo;

#endif
