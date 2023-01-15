#ifndef EXMODEL_H
#define EXMODEL_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSUpport/JSUList.h"
#include "dolphin/mtx.h"
#include "types.h"

class ExModel
{
public:
    static bool isMtxCombinationOn();
    Mtx *getBaseTRMtx();/* {
        return &mBaseTRMtx;
    }*/

private:
    u8 _0[0x2c];
    Mtx mBaseTRMtx;
};

class ExMDRecord;

class ExMdlDataMgr : JKRDisposer
{
public:
    ExMdlDataMgr(bool);

private:
    JSUList<ExMDRecord> mList;
    bool _18;
};

#endif // EXMODEL_H

