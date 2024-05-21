#ifndef JAUBANKTABLE_H
#define JAUBANKTABLE_H

#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JAudio/System/JASBankList.h"
#include "JSystem/JSupport/JSUList.h"

class JASBank;
class JAUBankTable : public JASBankList
{
public:
    JAUBankTable(u32 param_0, JASBank **ppBank, u32 param_2) : mBankPtrTable(ppBank, param_2)
    {
        _c = param_0;
    }
    JASBank *getBank(u32) const;

    JASBank *getBank(u32 offset) { return mBankPtrTable.get(offset); }
    void registBank(u32 offset, JASBank *pBank) { mBankPtrTable.set(offset, pBank); }

    JASPtrTable<JASBank> mBankPtrTable; // 04
    u32 _c;
};

class JAUBankTableLink : public JSULink<JAUBankTable>, public JAUBankTable
{
public:
    JAUBankTableLink(u32 param_0, JASBank **ppBank, u32 param_2) : JSULink<JAUBankTable>(this), JAUBankTable(param_0, ppBank, param_2) {}
};

struct JAUBankTableDictionary : JSUList<JAUBankTable>
{
   void appendBankTable(JSULink<JAUBankTable> *);
};

class JASWaveBank;
class JAUWaveBankTable : private JASPtrArray<JASWaveBank, 255>
{
public:
    JASWaveBank *getWaveBank(u32 index) { return get(index); }
    JASWaveBank *getWaveBank(u32 index) const { return get(index); }
    void registWaveBank(u32 index, JASWaveBank *bank) { set(index, bank); }
};

#endif /* JAUBANKTABLE_H */