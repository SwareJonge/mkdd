#ifndef MAPSELECTAPP_H
#define MAPSELECTAPP_H

#include "Osako/GameApp.h"
#include "kartEnums.h"

class MapSelectApp : public GameApp
{ // Autogenerated
public:
    class Menu
    {
    public:
        enum Direction
        {
            // TODO
        };

        Menu(s16 x, s16 y, const char *name, ECourseID id); // 0x801f9f18, constructor for cup and first course entry
        Menu(const char *name, ECourseID id);               // 0x801f9fa4, constructor for course entries
        void link();                                        // 0x801fa038
        // Inline/Unused
        void init(const char *, ECourseID);
        void draw();
        void move(Direction);
        void decide();
        void cancel();

    private:
        static Menu *mspLastMenu;     // 0x804168f4
        static Menu *mspSelectedMenu; // 0x804168f8
        static int msItemNum;         // 0x804168fc
        static s16 msX;               // 0x80416900
        static s16 msY;               // 0x80416902
        // TODO
    }; // Size: 0x20

    static void call();       // 0x801fa318
    static GameApp *create(); // 0x801fa33c
    MapSelectApp();           // 0x801fa38c
    virtual ~MapSelectApp();  // 0x801fa3f0
    virtual void draw();      // 0x801fa450
    virtual void calc();      // 0x801fa698

    static MapSelectApp *ptr() { return mspMapSelectApp; }

private:
    static MapSelectApp *mspMapSelectApp; // 0x804168f0
    // TODO
}; // Size: 0x14

#endif // MAPSELECTAPP_H