#ifndef ZMASTERCONTROL_H
#define ZMASTERCONTROL_H

#include "common.h"
#include "zstdlib/utilities/mystr.h"
#include "zstdlib/reftab/equreftab.h"
#include "system/zsysfile.h"

#pragma pack(push, 1)

// only two methods are ever called in this class, UnkFunc8 & UnkFunc1.
// Furthermore, the file path passed into UnkFunc1, doesn't even exist.
// So the GetSize method returns -1 and nothing happens.
// Looks like maybe this was used during testing but then never again?

// TODO: Fully implement this class.

class ZMasterControl
{
public:
    i32 *m_unkInt1;      // 4
    EquRefTab *m_equTab; // 8
    RefTab *m_refTab;    // 12
    MyStr m_myStr;       // 16

    ZMasterControl();
    virtual ~ZMasterControl();

    virtual void UnkFunc1(char *p_bankName, char *p_fileName, bool p_flag);
    virtual bool UnkFunc2(i32 p_bankId, i32 p_objectId);
    virtual bool UnkFunc3(i32 p_masterBank, char *p_str);
    virtual u32 *UnkFunc4(char *p_str, i32 p_unkInt);
    virtual MyStr *UnkFunc5(MyStr *p_str, char *p_str2);
    virtual char *UnkFunc6(i32 p_unkInt1, i32 p_unkInt2);
    virtual MyStr *UnkFunc7(MyStr *p_str, i32 p_unkInt1, i32 p_unkInt2);
    virtual void UnkFunc8();

}; // 144 in size.

#pragma pack(pop)

#endif