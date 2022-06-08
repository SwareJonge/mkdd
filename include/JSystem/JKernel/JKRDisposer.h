#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer
{
public:
    JKRDisposer();
    virtual ~JKRDisposer();
private:
    JKRHeap* mRootHeap; // _4
    JSULink<JKRDisposer> mPointerLinks; // _8
};

#endif