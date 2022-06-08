#ifndef JSULIST_H
#define JSULIST_H

#include <types.h>

class JSUPtrLink;

class JSUPtrList {
public:
    inline JSUPtrList() {
        initiate();
    }

    JSUPtrList(bool hasBeenInit);
    ~JSUPtrList();

    void initiate();
    void setFirst(JSUPtrLink *pNode);
    void append(JSUPtrLink *pNode);
    void prepend(JSUPtrLink *pNode);
    void insert(JSUPtrLink *, JSUPtrLink *);
    void remove(JSUPtrLink *pNode);

    JSUPtrLink* mHead; // _0
    JSUPtrLink* mTail; // _4
    u32 mNodeCount;    // _8
};

class JSUPtrLink {
public:
    JSUPtrLink(void *pData);
    ~JSUPtrLink();

    void *mData;          // _0
    JSUPtrList* mPtrList; // _4
    JSUPtrLink* mPrev;    // _8
    JSUPtrLink* mNext;    // _C
};

template<typename T>
class JSULink : public JSUPtrLink {
public:
    JSULink(void *pData) : JSUPtrLink(pData) { }

    ~JSULink();
};

template<typename T>
class JSUList : public JSUPtrList {
public:
    JSUList() : JSUPtrList() { }
    JSUList(bool hasBeenInit) : JSUPtrList(hasBeenInit) { }

    ~JSUList();
};

#endif