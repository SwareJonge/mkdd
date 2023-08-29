#ifndef KARTLOCALE_H
#define KARTLOCALE_H

// Country and Language regions seem alphabetically sorted, so that means UNITED_STATES and UNITED_KINGDOM are incorrectly named

enum Country {
    FRANCE = 0,
    GERMANY = 1,
    ITALY = 2,
    JAPAN = 3,
    SPAIN = 4,
    UNITED_STATES = 5, // WORLD_AMERICA?
    UNITED_KINGDOM = 6 // WORLD_EUROPE?
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

enum SpeedUnit
{
    SpeedUnit_Metric = 0,
    SpeedUnit_Impractical = 1
};

enum VideoFrameMode
{
    VideoFrameMode_60Hz = 0,
    VideoFrameMode_50Hz = 1
};

enum VideoMode
{
    VideoMode_NTSC = 0,
    VideoMode_PAL = 1
};

#ifdef VIDEO_PAL
#define NORMAL_RMODE 4         // PAL 50hz
#define MOVIE_RMODE 5          // PAL 50hz
#define ENHANCED_RMODE 6       // EURGB 60hz
#define ENHANCED_MOVIE_RMODE 7 // EURGB 60hz
#else
#define NORMAL_RMODE 0         // NTSC 480i
#define MOVIE_RMODE 1          // NTSC 480i
#define ENHANCED_RMODE 2       // 480p
#define ENHANCED_MOVIE_RMODE 3 // 480p
#endif

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

    static void setCountry(Country country) {
        msCountry = country;
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

    static void setVideoFrameMode(VideoFrameMode videoFrameMode) {
        msVideoFrameMode = videoFrameMode;
    }

    static VideoMode getVideoMode() {
        return msVideoMode;
    }

};

#endif // !KARTLOCALE_H
