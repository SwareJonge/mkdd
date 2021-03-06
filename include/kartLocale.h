#ifndef KARTLOCALE_H
#define KARTLOCALE_H

enum Country {
    FRANCE = 0,
    GERMANY = 1,
    ITALY = 2,
    JAPAN = 3,
    SPAIN = 4,
    UNITED_STATES = 5,
    UNITED_KINGDOM = 6
};

enum DatingMode { // not too sure of this but this makes most sense
    YYYY_MM_DD = 0,
    MM_DD_YYYY = 1,
    DD_MM_YYYY = 2
};

enum Language {
    ENGLISH = 0,
    FRENCH = 1,
    GERMAN = 2,
    ITALIAN = 3,
    JAPANESE = 4,
    SPANISH = 5
};

enum Region {
    JP = 0,
    NA = 1,
    EU = 2
};

enum SpeedUnit {
    KMH = 0, // correct
    MPH = 1 // incorrect
};

enum VideoFrameMode {
    EURGB60 = 0,
    PAL50 = 1
};

enum VideoMode {
    NTSC = 0,
    PAL = 1
};

// is this even a class?
class KartLocale {
private:
    static Country msCountry;
    static Region msRegion;
    static DatingMode msDatingMode;
    static const char *mscpaLanguageName[];

    static Language msLanguage;
    static SpeedUnit msSpeedUnit;
    static VideoMode msVideoMode;
    static VideoFrameMode msVideoFrameMode;
public:
    static void localize();

    static Country getCountry() {
        return msCountry;
    }

    static DatingMode getDatingMode() {
        return msDatingMode;
    }

    static Region getRegion() {
        return msRegion;
    }

    static Language getLanguage() {
        return msLanguage;
    }

    static const char * getLanguageName() {
        return mscpaLanguageName[(int)getLanguage()];
    }

    static SpeedUnit getSpeedUnit() {
        return msSpeedUnit;
    }

    static VideoFrameMode getVideoFrameMode() {
        return msVideoFrameMode;
    }

    static VideoMode getVideoMode() {
        return msVideoMode;
    }

};

#endif // !KARTLOCALE_H
