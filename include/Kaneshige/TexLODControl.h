#ifndef TEXLODCONTROL_H
#define TEXLODCONTROL_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "types.h"

class TexLODControl : JKRDisposer
{
public:
    TexLODControl(int);
    virtual ~TexLODControl();
    void setGeographyLODBiasOn(bool p1) {
        _1a = p1;
    }

private:
    s16 _18;
    bool _1a;
};

#endif