#ifndef GAMESOUNDTABLE_H
#define GAMESOUNDTABLE_H

#include <JSystem/JAudio/Interface/JAISound.h>
#include <JSystem/JAudio/Interface/JAIStreamDataMgr.h>
#include <JSystem/JAudio/JAUSoundInfo.h>

namespace GameAudio
{
    class CustomSoundTable : public JAISoundInfo, public JAUSoundInfo, public JAIStreamDataMgr, public JASGlobalInstance<CustomSoundTable>
    {
    public:
        virtual u32 getSoundType(JAISoundID soundID) const;
        virtual int getCategory(JAISoundID soundID) const;
        virtual u32 getPriority(JAISoundID soundID) const;
        virtual void getSeInfo(JAISoundID soundID, JAISe *se) const;
        virtual void getSeqInfo(JAISoundID soundID, JAISeq *seq) const ;
        virtual void getStreamInfo(JAISoundID soundID, JAIStream *stream) const;

        virtual ~CustomSoundTable();
        virtual void getAudibleSw(JAISoundID soundID) const;
        virtual u16 getBgmSeqResourceID(JAISoundID soundID) const;
        
        virtual u8 getPTrack(JAISoundID soundID) const;
        virtual u32 getSwBit(JAISoundID soundID) const;
        virtual u8 getLevelFlag(JAISoundID soundID) const;
        virtual s32 getStreamFileEntry(JAISoundID soundID);

        void initAudibleSw();
    private:
        JAISoundInfo *getSoundInfo_(JAISoundID soundID, JAISound *sound) const;
    };
}

#endif
