#ifndef PAUSEMANAGER_H
#define PAUSEMANAGER_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class PauseManager {
public:
    PauseManager(JKRHeap *);    
    void setExecFlag(bool);
    void reset();

    void enablePause();

    static PauseManager *getManager() {
        return mThis;
    } 
    static PauseManager *mThis;

private:
    u8 _0[0x3c];
};

inline PauseManager *GETPauseManager() {
    return PauseManager::getManager();
}

#endif