#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "types.h"

template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

JASBasicBank::JASBasicBank() {
    JASCalc::bzero(mInstTable, sizeof(mInstTable));
}

void JASBasicBank::setInst(int prg_no, JASInst *inst) {
#line 22
    JUT_ASSERT(prg_no < INST_MAX);
    JUT_ASSERT(prg_no >= 0);

    JUT_ASSERT(mInstTable[prg_no] == 0);
    mInstTable[prg_no] = inst;
}

JASInst *JASBasicBank::getInst(u32 id) const {
    if (id >= INST_MAX) {
        return nullptr;
    }
    return mInstTable[id];
}
