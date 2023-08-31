#include <string.h>
#include "Kameda/PrintDvdError.h"
#include "Kameda/ReadPrintMessage.h"
#include "Osako/ResMgr.h"

using namespace ResMgr;
using namespace ReadPrintMessage;

PrintDvdError::PrintDvdError(JKRHeap *heap)
{
    mPrintWindow = new (heap, 0) PrintWindow(heap);
    mBmgFile = getPtr(mcDvd);
    mColorFile = getPtr(mcColor);
}

void PrintDvdError::init(int errorId) {
    switch (errorId)
    {
    case 1:
        mBmgId = getBmgMessageID(mcError3);
        mBmgFile = getPtr(mcDvd);
        break;
    case 2:
    case 3:
        mBmgId = getBmgMessageID(mcError4);
        mBmgFile = getPtr(mcDvd);
        break;
    case 4:
        mBmgId = getBmgMessageID(mcError1);
        mBmgFile = getPtr(mcDvd);
        break;
    case 5:
        mBmgId = getBmgMessageID(mcError0);
        mBmgFile = getPtr(mcDvd);
        break;
    case 6:
    case 7:
        mBmgId = getBmgMessageID(mcError7);
        mBmgFile = getPtr(mcLan);
        break;
    case 8:
        mBmgId = getBmgMessageID(mcError8);
        mBmgFile = getPtr(mcLan);
        break;
    case 9:
        mBmgId = getBmgMessageID(mcError6);
        mBmgFile = getPtr(mcLan);
        break;
    case 10:
        mBmgId = getBmgMessageID(mcError5);
        mBmgFile = getPtr(mcLan);
        break;
    case 11:
        mBmgId = getBmgMessageID(mcError9);
        mBmgFile = getPtr(mcLan);
        break;
    case 12:
        mBmgId = getBmgMessageID(mcError10);
        mBmgFile = getPtr(mcLan);
        break;
    default:
        mBmgId = 0;
        break;
    }

    mColor = getColor(mColorFile, getDefaultColor(mBmgFile));
    
    // Inline setters?
    mPrintWindow->_2e8 = 1;
    mPrintWindow->_2ec = 1;
    mPrintWindow->_2f0 = 1;
    mPrintWindow->_2f4 = 9.0f;
    mPrintWindow->_2f8 = 2;
    mPrintWindow->calc();
    
    char *msg = getMessage(mBmgFile, mBmgId);

    mPrintWindow->getTextBox()->setFontColor(mColor, mColor); // HUH

    J2DTextBox::TFontSize baseFontSize = mPrintWindow->getBaseFontSize();
    char buf[512];
    J2DTextBox::TFontSize fontSize;

    // Inline?
    mPrintWindow->getTextBox()->setFontSize(baseFontSize.x * getRate(mBmgFile, mBmgId) / 100, baseFontSize.y * getRate(mBmgFile, mBmgId) / 100);
    mPrintWindow->getTextBox()->setCharSpace(getCharSpace(mBmgFile, mBmgId));
    mPrintWindow->getTextBox()->getFontSize(fontSize);
    tagCnv(msg, mColorFile, fontSize, sizeof(buf), buf);
    strncpy(mPrintWindow->getTextBox()->getStringPtr(), buf, sizeof(buf));
}

PrintDvdError::~PrintDvdError() {}

void PrintDvdError::draw()
{
    mPrintWindow->draw();
}

void PrintDvdError::calc()
{
    mPrintWindow->calc();
}
