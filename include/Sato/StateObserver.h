#ifndef STATEOBSERVER_H
#define STATEOBSERVER_H

#include "types.h"
#include <JSystem/JUtility.h>

// TODO: check if this matches in every instance

#define Observer_FindAndInit(cname, max)                                                                                                 \
    const StateFuncSet<cname> *func = FindStateFunc<cname>(getState(), cname::sTable, max); \
    (this->*func->mInitFunc)();

#define Observer_FindAndExec(cname, max)                                                                                                 \
    const StateFuncSet<cname> *func = FindStateFunc<cname>(getState(), cname::sTable, max); \
    (this->*func->mExecFunc)();

class StateObserver
{
public:
    StateObserver();     // 0x802714b0
    void ExecuteState(); // 0x80271518
    void ResetState();   // 0x80271570

    void setState(u16);
    u16 getState() const { return mState; }

    // Vtable
    virtual ~StateObserver();
    virtual void MoveExec() = 0;
    virtual void InitExec() = 0;

    template <typename T>
    struct StateFuncSet
    {
        typedef void (T::*StateFunc)();
        u16 mId;
        StateFunc mInitFunc;
        StateFunc mExecFunc;
    };

    template <typename T>
    static const StateFuncSet<T> *FindStateFunc(u16 id, const StateFuncSet<T> *funcTable, u16 max)
    {
#line 55
        JUT_MINMAX_ASSERT(0, max, 32);
        for (u16 i = 0; i < max; i++)
        {
            if (id == funcTable[i].mId)
                return &funcTable[i];
        }
        return nullptr;
    }

private:
    u16 _4;     // confusion, this one gets set by the setter, _06 for the getter
    u16 mState; // 6
    u32 _8;     //
};

template <typename T>
class StateObserverT : public StateObserver
{
public:
    // TODO
};

#endif
