#ifndef STATEOBSERVER_H
#define STATEOBSERVER_H

#include "types.h"

class StateObserver
{
public:
    StateObserver();

    void setState(u16);
    u16 getState() const { return mState; }

    // Vtable
    virtual ~StateObserver();
    virtual void MoveExec() = 0;
    virtual void InitExec() = 0;

    typedef void (StateObserver::*StateFunc)();
    template <typename T>
    struct StateFuncSet
    {
        u16 mId;
        StateFunc mInitFunc;
        StateFunc mExecFunc;
    };

    template <typename T>
    static StateFuncSet<T> *FindStateFunc(u16 id, const StateFuncSet<T> *funcTable, u16 max) // OK?
    {
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