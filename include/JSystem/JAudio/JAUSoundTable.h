#ifndef JAUDIO_JAUSOUNDTABLE_H
#define JAUDIO_JAUSOUNDTABLE_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASGadget.h"

struct JAUSoundTableItem
{
    u8 mPriority;
    u8 _1;
    u16 mResourceId;
    u32 _4;
    f32 _8;
};

template <typename Root, typename Section, typename Group, typename Typename_0>
struct JAUSoundTable_
{
    JAUSoundTable_()
    {
        _0 = NULL;
        _4 = 0;
    }

    void reset()
    {
        _0 = NULL;
        _4 = NULL;
    }

    void init(const void *param_0)
    {
        _0 = param_0;
        // magic number is not in debug rom. I'm not sure what this comparison is (maybe some sort of '' number?)
        // I also do not know how it is different between JAUSoundTable and JAUSoundNameTable
        if (*(u32 *)_0 + 0xbdad0000 != Root::magicNumber())
        {
            _0 = NULL;
        }
        else
        {
            _4 = (Root *)((u8 *)_0 + *((u32 *)_0 + 3));
        }
    }

    Section *getSection(int index) const
    {
        if (index < 0)
        {
            return NULL;
        }
        if ((u32)index >= _4->mSectionNumber)
        {
            return NULL;
        }
        u32 offset = _4->mSectionOffsets[index];
        if (offset == 0)
        {
            return NULL;
        }
        return (Section *)((u8 *)_0 + offset);
    }

    Group *getGroup(Section *param_1, int index) const
    {
        int iVar1;

        if (index < 0)
        {
            return NULL;
        }
        if ((u32)index >= param_1->mNumGroups)
        {
            return NULL;
        }
        u32 offset = param_1->getGroupOffset(index);
        if (offset == 0)
        {
            return NULL;
        }
        return (Group *)((u8 *)_0 + offset);
    }

    const void *_0;
    Root *_4;
    //u32 _8;
    //u32 _c;
};

struct JAUSoundTableRoot
{
    static inline u32 magicNumber() { return 0x5420; }
    u32 mSectionNumber;
    u32 mSectionOffsets[0];
};

struct JAUSoundTableSection
{
    int getGroupOffset(int index) const
    {
        if (index < 0)
        {
            return 0;
        }
        if (index >= mNumGroups)
        {
            return 0;
        }
        return mGroupOffsets[index];
    }

    u32 mNumGroups;
    u32 mGroupOffsets[0];
};

struct JAUSoundTableGroup
{
    u8 getTypeID(int index) const
    {
        if (index < 0)
        {
            return 0;
        }
        if (index >= mNumItems)
        {
            return 0xff;
        }
        return mTypeIds[index * 4];
    }

    u32 getItemOffset(int index) const
    {
        if (index < 0)
        {
            return 0;
        }
        if (index >= mNumItems)
        {
            return 0;
        }
        return *(u32 *)(mTypeIds + index * 4) & 0xffffff;
    }

    u32 mNumItems;
    u32 _4;
    u8 mTypeIds[0];
};

struct JAUSoundTable : public JASGlobalInstance<JAUSoundTable>
{
    JAUSoundTable(bool p1) : JASGlobalInstance<JAUSoundTable>(p1)
    {
    }
    ~JAUSoundTable() {}

    void init(void const *);
    u8 getTypeID(JAISoundID) const;
    JAUSoundTableItem *getData(JAISoundID) const;

    JAUSoundTableItem *getItem(JAUSoundTableGroup *group, int index) const
    {
        u32 offset = group->getItemOffset(index);
        if (offset == 0)
        {
            return NULL;
        }
        return (JAUSoundTableItem *)((u8 *)_0._0 + offset);
    }

    const void *getResource() { return _0._0; }

    JAUSoundTable_<JAUSoundTableRoot, JAUSoundTableSection, JAUSoundTableGroup, void> _0;
};

struct JAUSoundNameTableRoot
{
    static inline u32 magicNumber() { return 0x544e; }
    u32 mSectionNumber;
    u32 mSectionOffsets[0];
};

struct JAUSoundNameTableSection
{
};

struct JAUSoundNameTableGroup
{
};

struct JAUSoundNameTable : public JASGlobalInstance<JAUSoundNameTable>
{
    JAUSoundNameTable(bool p1) : JASGlobalInstance<JAUSoundNameTable>(p1)
    {
    }
    ~JAUSoundNameTable() {}
    void init(void const *);

    JAUSoundTable_<JAUSoundNameTableRoot, JAUSoundNameTableSection, JAUSoundNameTableGroup, char> _0;
};

#endif