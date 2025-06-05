#ifndef JASCALLBACK_H
#define JASCALLBACK_H

#include "dolphin/types.h"

typedef s32 JASCallback(void*);

class JASCallbackMgr {
public:
    JASCallbackMgr() { }

    struct TCallback {
        TCallback() { 
            mFunction = NULL;
            mArgument = NULL;
        }

        /** @fabricated */
        bool isMatch(JASCallback* function, void* argument) { return (mFunction == function && mArgument == argument); }

        /** @fabricated */
        void clear() {
            mFunction = NULL;
            mArgument = NULL;
        }

        JASCallback* mFunction;
        void* mArgument;
    };

    bool regist(JASCallback*, void*);
    int reject(JASCallback*, void*);
    void callback();


    static const int NUM_CALLBACKS = 0x20;
private:
    TCallback mCallbacks[NUM_CALLBACKS];
};

#endif /* JASCALLBACK_H */
