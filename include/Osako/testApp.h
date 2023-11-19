#ifndef TESTAPP_H
#define TESTAPP_H

#include "Osako/GameApp.h"
#include "types.h"

class TestApp : public GameApp
{
public:
    // static void call(); // inline auto behaviour?
    static TestApp *create();
    TestApp();
    
    virtual ~TestApp();
    virtual void draw();
    virtual void calc();

    static TestApp *ptr() { return mspTestApp; }

private:
    static TestApp *mspTestApp;
    int _8;
};

#endif