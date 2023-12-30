#ifndef GAMESOUNDTABLE_H
#define GAMESOUNDTABLE_H

#include <JSystem/JAudio/Interface/JAISoundInfo.h>
#include <JSystem/JAudio/Interface/JAIStreamDataMgr.h>
#include <JSystem/JAudio/JAUSoundInfo.h>

namespace GameAudio
{
    class CustomSoundTable : public JAISoundInfo, public JAUSoundInfo, public JAIStreamDataMgr, public JASGlobalInstance<CustomSoundTable>
    {
    public:
        virtual u32 getSoundType(JAISoundID) const;
        virtual int getCategory(JAISoundID) const;
        virtual u32 getPriority(JAISoundID) const;
        virtual void getSeInfo(JAISoundID, JAISe *) const;
        virtual void getSeqInfo(JAISoundID, JAISeq *) const ;
        virtual void getStreamInfo(JAISoundID, JAIStream *) const;

        virtual ~CustomSoundTable();
        virtual void getAudibleSw(JAISoundID) const;
        virtual u16 getBgmSeqResourceID(JAISoundID) const;
        
        virtual u8 getPTrack(JAISoundID) const;
        virtual u32 getSwBit(JAISoundID) const;
        virtual u8 getLevelFlag(JAISoundID) const;
        virtual s32 getStreamFileEntry(JAISoundID);
    };
}

#endif