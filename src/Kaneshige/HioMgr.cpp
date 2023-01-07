#include "Kaneshige/HioMgr.h"
#include "types.h"

HioNode::HioNode(const char *nodeName, JORReflexible *jor, u32 p3, u32 p4) : mTree(this)
{
    mName = nodeName;
    mReflexible = jor;
    _20 = p3;
    _24 = p4;
}

HioNode::~HioNode() {
    JSUTree<HioNode> *parent = mTree.getParent();
    if(parent)
        parent->removeChild(&mTree);        
}