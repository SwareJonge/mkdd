#ifndef LANSELECTMODE_H
#define LANSELECTMODE_H

#include <JSystem/J2D/J2DPane.h>
#include <JSystem/JKernel/JKRArchive.h>

class LANSelectMode { // Autogenerated
public:
    class Color {
    public:

    };

    class Background {
    public:
        void setup(JKRArchive *);
        void calc();

        // Inline/Unused
        ~Background();
    };

    class Item {
    public:
        void changeColor(Color); // 0x80212dac
        void calc(); // 0x80212e50
        // Inline/Unused
        Item(u32, J2DScreen *, void *);
        void selectIn();
        void selectOut();
        void unselect();
        // Inline
        void update(); // 0x802158b0
        void changeValueUp(); // 0x80216018
        void changeValueDown(); // 0x80216020
    };

    class SelectableItem : public Item {
    public:
        SelectableItem(u32, J2DScreen *, void *, void *, void *, int, int, u32); // 0x80212f00
        void changeValueUp(); // 0x802132d8
        void changeValueDown(); // 0x802133c8

        void update(); // 0x802134b4
        void calc(); // 0x8021350c

        // Inline/Unused
        void setMask(u32);
        void setMax(int);
    };

    class SelectableItem1 : public SelectableItem {
    public:
        void changeColor(Color); // 0x80213770
        void calc(); // 0x80213884

        // Inline/Unused 
        SelectableItem1(u32, J2DScreen *, void *, void *, void *, ResTIMG **, int);
    };

    class SelectableItem2 : public SelectableItem {
    public:
        void changeColor(Color); // 0x80213b3c
        void calc(); // 0x80213cc0

        // Inline/Unused
        SelectableItem2(u32, J2DScreen *, void *, void *, void *);
    };

    class SelectableItemDisp : public SelectableItem {
    public:
        void changeColor(Color); // 0x80214088
        void calc(); // 0x802142ec

        // Inline/Unused
        SelectableItemDisp(u32, J2DScreen *, void *, void *, void *);
    };

    class SelectableItemSw {
    public:
        enum ValueMode {

        };
        void changeValueMode(ValueMode); // 0x80213fc0

        // Inline/Unused
        SelectableItemSw(u32, J2DScreen *, void *, void *, void *, ResTIMG **, ResTIMG **, int, ResTIMG **, int);
        void setMaskSw(u32);
    };
    
    LANSelectMode(JKRArchive *); // 0x8021462c
    void directView(int); // 0x802158b4
    int calc(); // 0x80215c9c
    void calcAnm(); // 0x80216028
    void draw(const J2DGrafContext *); // 0x802161ac
    void start(bool); // 0x80216214
    void setDisplayMax(); // 0x80216358    
    
private: 
    u8 _0[0x350];
}; // class LANSelectMode

extern const char *cpaGameModeTex[];
extern const char *cpaCrsOrderTex[];
extern const char *cpaCrsNameTex[];
extern const char *cpaStageOrderTex[];
extern const char *cpaLabelNameTex[];
extern const char *cpaTagTex[];

#endif // LANSELECTMODE_H
