#ifndef PRINTDVDERROR_H
#define PRINTDVDERROR_H

#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JUtility/TColor.h>

#include "Kameda/PrintWindow.h"

class PrintDvdError
{
public:
    enum MessageID
    {
        // TODO
        mcError0,
        mcError1,
        mcError2,
        mcError3,
        mcError4,
        mcError5,
        mcError6,
        mcError7,
        mcError8,
        mcError9,
        mcError10
    };

    PrintDvdError(JKRHeap *heap);
    ~PrintDvdError();

    void init(int errorId);
    void draw();
    void calc();

private:
    void *mColorFile;
    void *mBmgFile;
    PrintWindow *mPrintWindow;
    int mBmgId;
    JUtility::TColor mColor;
};

#endif
