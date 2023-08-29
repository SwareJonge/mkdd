#ifndef TEXLODCONTROL_H
#define TEXLODCONTROL_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "types.h"

class TexLODControl : public JKRDisposer
{
public:
    TexLODControl(int);
    virtual ~TexLODControl();

    enum ELODBiasID
    {
        cLODBiasID_Min,
        cLODBiasID_Geography,
        cLODBiasID_Character,
        cLODBiasID_3, // used by StringObj::loadmodel(J3DModelData *)
        cLODBiasID_Max
    };

    f32 getLODBias(ELODBiasID id);
    void setGeographyLODBiasOn(bool p1) { mEnable = p1; }

    static f32 getGeographyLODBias() { return getManager()->getLODBias(cLODBiasID_Geography); }
    static f32 getCharacterLODBias() { return getManager()->getLODBias(cLODBiasID_Character); }
    static TexLODControl *getManager() { return sManager; }

    static const f32 cGeoObjLBTable[2];
    static const f32 cCharLBTable[2];
    static TexLODControl *sManager;

private:
    s16 mLevel;
    bool mEnable;
};

#endif