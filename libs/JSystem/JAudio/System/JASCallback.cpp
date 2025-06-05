#include "JSystem/JAudio/System/JASCallback.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"


bool JASCallbackMgr::regist(JASCallback *function, void *argument)
{
    JAS_CS_START
    for (int i = 0; i < NUM_CALLBACKS; i++) {
        if (mCallbacks[i].mFunction == NULL) {
            mCallbacks[i].mFunction = function;
            mCallbacks[i].mArgument = argument;
            return true;
        }
    }
    JAS_CS_END
    return false;
}

int JASCallbackMgr::reject(JASCallback *function, void *argument) {
    int rejectNum = 0;
    
    JAS_CS_START
    for (int i = 0; i < NUM_CALLBACKS; i++) {
        if (mCallbacks[i].mFunction == function && mCallbacks[i].mArgument == argument) {
            mCallbacks[i].mFunction = NULL;
            mCallbacks[i].mArgument = NULL;
            rejectNum++;
        }
    }
    JAS_CS_END
    return rejectNum;
}

void JASCallbackMgr::callback() {
    for (int i = 0; i < NUM_CALLBACKS; i++) {
        if (mCallbacks[i].mFunction && mCallbacks[i].mFunction(mCallbacks[i].mArgument) < 0) {
            mCallbacks[i].mFunction = NULL;
            mCallbacks[i].mArgument = NULL;
        }
    }
}
