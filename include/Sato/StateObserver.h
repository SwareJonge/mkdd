#ifndef STATEOBSERVER_H
#define STATEOBSERVER_H

#include "types.h"

class StateObserver {
public:
    StateObserver();

    void setState(u16);

private:
    u16 _4;
    u16 _6;
    u32 _8;
};

#endif