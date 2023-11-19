#ifndef MOVIEAPP_H
#define MOVIEAPP_H

#include "Osako/GameApp.h"

class GeographyObj;

class MovieApp : public GameApp
{ // Autogenerated
public:
    static void getArchive(const char *); // 0x801d799c
    static MovieApp *create();            // 0x801d79c4
    static void call();                   // 0x801d7a98
    MovieApp(void *);                     // 0x801d7abc
    virtual ~MovieApp();                  // 0x801d7b44
    virtual void draw();                  // 0x801d7ba4
    virtual void calc();                  // 0x801d7c08
    virtual void reset();                 // 0x801d7e18
    void isEnd();                         // 0x801d7d9c
    void loadOpeningData();               // 0x801d7e64
    void doRunning();                     // 0x801d7e88
    void doEnding();                      // 0x801d8004

    static MovieApp *getMovieApp() { return sMovieApp; }

private:
    static MovieApp *sMovieApp; // 0x80416840
    static void *mspHeapBuffer; // 0x80416844
};

class MVActor
{
public:
    MVActor();
    //~MVActor();
    virtual void setCurViewNo(u32) = 0;
    virtual void calc() = 0;
    virtual void update() = 0;
    virtual void viewCalc(u32) = 0;
};

class MovieJugemActor : public MVActor
{
public:
    void loadModelData(GeographyObj *);
    MovieJugemActor();
    virtual void setCurViewNo(u32);
    virtual void calc();
    virtual void update();
    virtual void viewCalc(u32);
    void endDemo();
    bool isDemoEnd();
};

#define MOVIE_GetHeap() \
    MovieApp::getMovieApp()->getHeap()

#define MOVIE_loadOpeningData() \
    MovieApp::getMovieApp()->loadOpeningData()

#endif // MOVIEAPP_H