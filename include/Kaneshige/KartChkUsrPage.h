#ifndef KARTCHKUSRPAGE_H
#define KARTCHKUSRPAGE_H

#include "Kaneshige/KartChecker.h"
#include "Kaneshige/SysDebug.h"

class KartChkUsrPage : public SysDbUsrPage
{
public:
    KartChkUsrPage(KartChecker *kartChecker);
    virtual ~KartChkUsrPage(){};
    virtual void draw();

private:
    KartChecker *mKartChecker;
};

#endif