#ifndef JSULIST_H
#define JSULIST_H

#include "types.h"

class JSUPtrLink;

class JSUPtrList {
public:
    JSUPtrList() {
        initiate();
    }

    JSUPtrList(bool);
    ~JSUPtrList();

    void initiate();
    void setFirst(JSUPtrLink *);
    bool append(JSUPtrLink *);
    bool prepend(JSUPtrLink *);
    bool insert(JSUPtrLink *, JSUPtrLink *);
    bool remove(JSUPtrLink *);
    JSUPtrLink *getNthLink(u32 idx) const;

    JSUPtrLink *mHead;  // _0
    JSUPtrLink *mTail;  // _4
    u32 mNodeCount;     // _8
};

class JSUPtrLink {
public:
    JSUPtrLink(void *);
    ~JSUPtrLink();

    void *mData;            // _0
    JSUPtrList *mPtrList;   // _4
    JSUPtrLink *mPrev;      // _8
    JSUPtrLink *mNext;      // _C
};

template<class T>
class JSULink : public JSUPtrLink {
public:
    JSULink(void *pData) : JSUPtrLink(pData) {

    }

    ~JSULink();
};

template<class T>
class JSUList : public JSUPtrList {
public:
    JSUList() : JSUPtrList() {

    }

    JSUList(bool thing) : JSUPtrList(thing) {

    }
};

template <typename T>
class JSUTree : public JSUList<T>, public JSULink<T>
{
public:
    JSUTree(T *owner) : JSUList<T>(), JSULink<T>(owner) {}
    ~JSUTree() {}

    bool appendChild(JSUTree<T> *child) { return this->append(child); }

    bool removeChild(JSUTree<T> *child) { return this->remove(child); }

    bool insertChild(JSUTree<T> *before, JSUTree<T> *child) { return this->insert(before, child); }

    JSUTree<T> *getEndChild() const { return NULL; }

    JSUTree<T> *getFirstChild() const { return (JSUTree<T> *)this->getFirst(); }

    JSUTree<T> *getLastChild() const { return (JSUTree<T> *)this->getLast(); }

    JSUTree<T> *getNextChild() const { return (JSUTree<T> *)this->getNext(); }

    JSUTree<T> *getPrevChild() const { return (JSUTree<T> *)this->getPrev(); }

    u32 getNumChildren() const { return this->getNumLinks(); }

    T *getObject() const { return (T *)this->getObjectPtr(); }

    JSUTree<T> *getParent() const { return (JSUTree<T> *)this->getList(); }
};

template <typename T>
class JSUTreeIterator
{
public:
    JSUTreeIterator() : mTree(NULL) {}
    JSUTreeIterator(JSUTree<T> *tree) : mTree(tree) {}

    JSUTreeIterator<T> &operator=(JSUTree<T> *tree)
    {
        this->mTree = tree;
        return *this;
    }

    T *getObject() { return this->mTree->getObject(); }

    bool operator==(JSUTree<T> *other) { return this->mTree == other; }

    bool operator!=(JSUTree<T> *other) { return this->mTree != other; }

    JSUTreeIterator<T> operator++(int)
    {
        JSUTreeIterator<T> prev = *this;
        this->mTree = this->mTree->getNextChild();
        return prev;
    }

    JSUTreeIterator<T> &operator++()
    {
        this->mTree = this->mTree->getNextChild();
        return *this;
    }

    T &operator*() { return *this->getObject(); }

    T *operator->() { return this->getObject(); }

private:
    JSUTree<T> *mTree;
};

#endif /* JSULIST_H */