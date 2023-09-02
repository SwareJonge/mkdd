#ifndef _JSYSTEM_JPARTICLE_JPALIST_H
#define _JSYSTEM_JPARTICLE_JPALIST_H

#include <types.h>

template <typename T>
struct JPANode
{
    JPANode<T>() mPrev(nullptr) : mNext(nullptr) : mObject() {}

    JPANode<T> *mPrev;
    JPANode<T> *mNext;
    T mObject;
};

template <typename T>
struct JPAList
{
    JPAList<T>()
    {
        mFirst = nullptr;
        mLast = nullptr;
        mNum = 0;
    }

    JPANode<T> *mFirst;
    JPANode<T> *mLast;
    u32 mNum;
};



#endif