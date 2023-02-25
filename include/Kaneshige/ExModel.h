#ifndef EXMODEL_H
#define EXMODEL_H

#include <dolphin/mtx.h>
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "types.h"

class ExModel
{
public:
    static bool isMtxCombinationOn() { return sMtxCombinationOn; };
    Mtx *getBaseTRMtx() {
        return &mBaseTRMtx;
    }

    static bool sMtxCombinationOn;
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

