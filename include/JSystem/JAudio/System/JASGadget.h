#ifndef JAUDIO_JASGADGET_H
#define JAUDIO_JASGADGET_H

#include <string.h>
#include "JSystem/JUtility/JUTDbg.h"

template <class T>
class JASGlobalInstance
{
public:
    JASGlobalInstance(T *inst)
    {
        sInstance = inst;
    }

    JASGlobalInstance(bool param_1)
    {
        if (param_1)
        {
#line 186
            JUT_ASSERT(sInstance == 0);
            sInstance = (T *)this;
        }
    }

    ~JASGlobalInstance()
    {
        if (sInstance == (T *)this)
        {
            sInstance = NULL;
        }
    }

    static T *getInstance() { return sInstance; }

    static T *sInstance;
};

template <class T>
class JASPtrTable
{
public:
    JASPtrTable(T **table, u32 size)
    {
        mTable = table;
        mSize = size;
        memset(mTable, 0, size * 4);
    }
    T *get(u32 index)
    {
        if (index >= mSize)
            return NULL;
        return mTable[index];
    }
    T *get(u32 index) const
    {
        if (index >= mSize)
            return NULL;

        return mTable[index];
    }
    void set(u32 index, T *value)
    {
#line 229
        JUT_ASSERT(index < mSize);
        mTable[index] = value;
    }

private:
    T **mTable;
    u32 mSize;
};

template <class T, size_t N>
class JASPtrArray : public JASPtrTable<T>
{
public:
    JASPtrArray() : JASPtrTable<T>(mArray, N) {}

private:
    T *mArray[N];
};

#endif /* JAUDIO_JASGADGET_H */