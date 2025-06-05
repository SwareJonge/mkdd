#ifndef JAUDIO_JASBANKTABLE_H
#define JAUDIO_JASBANKTABLE_H

#include "JSystem/JAudio/System/JASBankList.h"
#include "JSystem/JAudio/System/JASGadget.h"

template <size_t N>
class JASBankTable : public JASBankList
{
public:
    void registBank(u32 index, JASBank *bank) { mArray.set(index, bank); }
    virtual JASBank *getBank(u32 index) const { return mArray.get(index); }
    JASBank *getBank(u32 index) { return mArray.get(index); }
private:
    JASPtrArray<JASBank, N> mArray;
};

class JASDefaultBankTable : public JASBankTable<256>, public JASGlobalInstance<JASDefaultBankTable>
{
public:
    JASDefaultBankTable() : JASGlobalInstance(true) {}
};

#endif
