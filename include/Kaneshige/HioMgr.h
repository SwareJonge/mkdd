#ifndef HIOMGR_H
#define HIOMGR_H

#include "JSystem/JORReflexible.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "types.h"

class HioNode : JKRDisposer
{
public:
    HioNode(const char *nodeName, JORReflexible *jor, u32 p3, u32 p4);
    virtual ~HioNode();

private:
    const char *mName;
    JORReflexible *mReflexible;
    u32 _20;
    u32 _24;
    JSUTree<HioNode> mTree;
};

#endif