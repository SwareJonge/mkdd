#ifndef JAUDIO_JASBANKLIST_H
#define JAUDIO_JASBANKLIST_H

#include "types.h"

class JASBank;

class JASBankList
{
public:
    JASBankList() {}
    virtual JASBank *getBank(u32 param_0) const = 0;
};

#endif /* JAUDIO_JASBANKLIST_H */