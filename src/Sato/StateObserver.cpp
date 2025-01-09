#include "Sato/StateObserver.h"

StateObserver::StateObserver() {
    mState = 0;
    mPrevState = 0;
    mTimeExecuted = 0;
}

StateObserver::~StateObserver() {}

void StateObserver::ExecuteState() {
    if (mState != 0xffff) {
        changeState();
    }
    MoveExec();
    mTimeExecuted++;
}

void StateObserver::ResetState() {
    mState = 0;
    mPrevState = 0;
    mTimeExecuted = 0;
}

void StateObserver::setState(u16 state) {
    mState = state;
}

void StateObserver::changeState() {
    mPrevState = mState;
    mState = 0xffff;
    mTimeExecuted = 0;
    InitExec();
}
