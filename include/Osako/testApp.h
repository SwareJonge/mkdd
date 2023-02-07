#ifndef TESTAPP_H
#define TESTAPP_H

#include "Osako/GameApp.h"
#include "types.h"

class TestApp : public GameApp
{
public:
    static TestApp *create();
    static TestApp *mspTestApp;

    TestApp();
    //static void call(); // inline auto behaviour?
    virtual ~TestApp();
    virtual void draw();
    virtual void calc();
    private:
    int _8;
};

#endif