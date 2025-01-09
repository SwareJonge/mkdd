#ifndef JSYSTEM_LIST_H
#define JSYSTEM_LIST_H

#include "JSystem/JGadget/define.h"
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JGadget/std-memory.h"
#include "types.h"

// massive TODO, pls help me

namespace JGadget {

template <typename T, typename Allocator = JGadget::TAllocator<T> >
struct TList {
    struct TNode_ {
        TNode_ *pPrev_;
        TNode_ *pNext_;
    };

    class iterator {
    public:
        iterator() { this->p_ = nullptr; }
        iterator(TNode_* node) { this->p_ = node; }

        iterator& operator++() {
            this->p_ = this->p_->getNext();
            return *this;
        }

        iterator& operator--() {
            this->p_ = this->p_->getPrev();
            return *this;
        }

        TNode_& operator*() const {
            JGADGET_ASSERT(p_!=0);
            return *this->p_;
        }

        TNode_* operator->() const { return this->p_; }

        TNode_* p_;
    };

    class const_iterator {
    public:
        const_iterator(const TNode_* node) { this->p_ = node; }
        const_iterator(iterator it) { this->p_ = it.p_; }

        const const_iterator& operator++() {
            this->p_ = this->p_->getNext();
            return *this;
        }

        const const_iterator& operator--() {
            this->p_ = this->p_->getPrev();
            return *this;
        }

        const TNode_* operator->() const { return this->p_; }

        const TNode_* p_;
    };

    TList(const TAllocator<T> &allocator=0) {
        oSize_ = 0;
        Initialize_();
    }

    void Initialize_() {
        oEnd_.pNext_ = &this->oNode_;
        oEnd_.pPrev_ = &this->oNode_;
    }

    void DestroyNode_(TNode_ *p) {
    // probably doesn't match
        JGADGET_ASSERT(p!=0);
        JGADGET_ASSERT(p!=&oEnd_);
        JGADGET_ASSERT(p->pNext_->pPrev_!=p);
        JGADGET_ASSERT(p->pPrev_->pNext_!=p);
        oAllocator_.destroy(p + 1);
        oAllocator_.DeallocateRaw(p);
    }

  iterator end() {return &this->oNode_; }
  const_iterator end() const { return &this->oNode_; }

private:
    TAllocator<T> oAllocator_; // 0
    u32 oSize_;                // 4
    TNode_ oEnd_;              // 8
};

}



#endif
