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
        TNode_ *pNext_;
        TNode_ *pPrev_;
    };

    class iterator {
    public:
        iterator() { this->p_ = nullptr; }
        iterator(TNode_* node) { this->p_ = node; }

        iterator& operator++() {
            this->p_ = this->p_->pNext_;
            return *this;
        }

        iterator& operator--() {
            this->p_ = this->p_->pPrev_;
            return *this;
        }

        T& operator*() const {
            JGADGET_ASSERT(p_!=0);
            return *(T*)(this->p_ + 1);
        }

        T* operator->() const { 
            return (T*)(this->p_ + 1); 
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            return a.p_ == b.p_;
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return a.p_ != b.p_;
        }

        TNode_* p_;
    };

    class const_iterator {
    public:
        const_iterator(const TNode_* node) { this->p_ = node; }
        const_iterator(iterator it) { this->p_ = it.p_; }

        const const_iterator& operator++() {
            this->p_ = this->p_->pNext_;
            return *this;
        }

        const const_iterator& operator--() {
            this->p_ = this->p_->pPrev_;
            return *this;
        }

        const T& operator*() const {
            JGADGET_ASSERT(p_!=0);
            return *(const T*)(this->p_ + 1);
        }

        const T* operator->() const { 
            return (const T*)(this->p_ + 1); 
        }

        friend bool operator==(const const_iterator& a, const const_iterator& b) {
            return a.p_ == b.p_;
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b) {
            return a.p_ != b.p_;
        }

        const TNode_* p_;
    };

    ~TList() { clear(); }

    TList(const TAllocator<T>& alloc = TAllocator<T>())
	{
        oAllocator_ = alloc;
        oSize_ = 0;
		Initialize_();
	}

    void Initialize_() {
        oEnd_.pNext_ = &this->oEnd_;
        oEnd_.pPrev_ = &this->oEnd_;
    }

    TNode_* CreateNode_(TNode_* pNext, TNode_* pPrev, const T& value)
    {
        JGADGET_ASSERT(pNext!=0);
        JGADGET_ASSERT(pPrev!=0);

        void* raw = oAllocator_.AllocateRaw(sizeof(TNode_) + sizeof(T));

        if (raw == nullptr) {
            JGADGET_WARN("can't allocate memory");
            return nullptr;
        }

        TNode_* const ret = (TNode_*)raw;

        ret->pNext_ = pNext;
        ret->pPrev_ = pPrev;
        oAllocator_.construct((T*)(ret + 1), value);
        return ret;
    }

    void DestroyNode_(TNode_ *p) {
        JGADGET_ASSERT(p!=0);
        JGADGET_ASSERT(p!=&oEnd_);
        JGADGET_ASSERT(p->pNext_->pPrev_!=p);
        JGADGET_ASSERT(p->pPrev_->pNext_!=p);
        
        oAllocator_.destroy((T*)(p + 1));
        oAllocator_.DeallocateRaw(p);
    }

    iterator insert(iterator where, const T& what)
    {
        TNode_* const pNext = where.p_;
        JGADGET_ASSERT(pNext!=0);

        TNode_* const pPrev = pNext->pPrev_;
        
        TNode_* const newNode = CreateNode_(pNext, pPrev, what);

        if (newNode == nullptr) {
            return end();
        }

        pNext->pPrev_ = newNode;
        pPrev->pNext_ = newNode;
        ++oSize_;
        return iterator(newNode);
    }

    iterator erase(iterator what)
	{
		TNode_* p       = what.p_;
		TNode_* next       = p->pNext_;
		p->pPrev_->pNext_ = next;
		next->pPrev_        = p->pPrev_;
		DestroyNode_(p);
		oSize_--;
		return iterator(next);
	}

	iterator erase(iterator start, iterator end)
	{
		while (start != end) {
			start = erase(start);
		}
		return start;
	}
    
    void push_back(const T& what) { 
        insert(end(), what); 
    }

    iterator begin() { return iterator(oEnd_.pNext_); }
    const_iterator begin() const { return const_iterator(oEnd_.pNext_); }

    iterator end() { return iterator(&this->oEnd_); }
    const_iterator end() const { return const_iterator(&this->oEnd_); }

    iterator clear() { return erase(begin(), end()); }

private:
    TAllocator<T> oAllocator_; // 0
    u32 oSize_;                // 4
    TNode_ oEnd_;              // 8
};

}

#endif
