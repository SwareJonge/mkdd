#ifndef VIEWCTRLMODEL_H
#define VIEWCTRLMODEL_H

#include "Kaneshige/ExModel.h"

class ViewCtrlModel : public ExModel
{
public:
    ViewCtrlModel(u8 flags) { mFlags = flags; }

    // Not confirmed matching
    virtual void viewCalc(u32 p1)
    {
        if (isDetailFlg())
        {
            if (isVisible(p1))
                show(p1);
            else
                hide(p1);
        }
        ExModel::viewCalc(p1);
    }

    void clrVisibleAll() { mFlags &= 0xf0; }
    bool isDetailFlg() { return mFlags & 0x10; }
    bool isInvisibleAll() { return !(mFlags & 0xf); };
    bool isVisible(u32 p1) { return (mFlags & 1 << p1) != 0; }
    void setDetailFlg() { mFlags |= 0x10; }
    void setVisibleAll() { mFlags |= 0xf; }
    void setVisible(u32 cnsNo) { mFlags |= (1 << cnsNo);  }
    u8 getFlags() const { return mFlags; }

private:
    u8 mFlags; // 0x8c
};

#endif
