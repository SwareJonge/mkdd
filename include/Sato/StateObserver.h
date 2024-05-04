#ifndef STATEOBSERVER_H
#define STATEOBSERVER_H

#include "types.h"

class StateObserver {
public:
    StateObserver();

    void setState(u16);

    u16 getState() const { return mState; }

    // Vtable
    virtual ~StateObserver();
    virtual void MoveExec() = 0;
    virtual void InitExec() = 0;

private:
    u16 _4;
    u16 mState;
    u32 _8;
};

#endif