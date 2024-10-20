#ifndef ZSYSMEM_H
#define ZSYSMEM_H

#include "zstdlib/zmalloc.h"
#include "zstdlib/reftab/allocreftab.h"
#include "zstdlib/reftab/equreftab.h"

#pragma pack(push, 1)

class ZSysMem
{
public:
    u8 unkByte1;
    AllocRefTab *m_allocTab;
    u32 *m_allocArray;
    u32 *m_refArray;
    u8 unkByte3;
    i32 unkInt4;
    ZMalloc m_zMalloc;
    EquRefTab *m_equTab2;
    EquRefTab *m_equTab;
    i32 unkInt7;
    i32 unkInt8;
    i32 unkInt9;
    i32 m_nrAllocs;
    i32 m_nrAllocsCheck;
    i32 m_memAllocated;
    i32 unkInt13;
    i32 unkInt14;

    ZSysMem();

    virtual ~ZSysMem();
    virtual void UnkFunc0(i32 param1, i32 param2);
    virtual char *UnkFunc1();
    virtual i32 UnkFunc2(const char *param1, u64 param2);
    virtual void AllocCheck();
    virtual char UnkFunc4(u64 *param);
    virtual i32 UnkFunc5();
    virtual void NullSub1();
    virtual void NullSub2();
    virtual i32 UnkFunc6(i32 param1, char *param2, i32 param3);
    virtual i32 UnkFunc7(i32 param);
    virtual void *New(u32 allocSize);
    virtual void *NewWithRef(u32 allocSize);
    virtual char *Delete(i32 *param);
    virtual u32 UnkFunc10(const void *param1, i32 param2, i32 param3);
    virtual u64 UnkFunc11(const void **param);
    virtual u32 UnkFunc12(u32 param);
    virtual char *UnkFunc13(u32 param);
    virtual i32 UnkFunc14(i32 param1, i32 param2);
    virtual i32 UnkFunc15(i32 param);
    virtual i32 UnkFunc16(i32 param);
    virtual i32 UnkFunc17();
    virtual i32 UnkFunc18(char *param);
    virtual i32 UnkFunc19(u64 *param1, i32 param2, i32 param3, i32 param4);
    virtual void UnkFunc20(i32 param);
    virtual u32 FindEqu(i32 value, boolean flag);
    virtual void AddEqu(u32 refNum, i32 refValue, boolean flag);
    virtual i32 UnkFunc23(u32 param);
    virtual u64 *UnkFunc24(u64 *param1, u64 *param2);
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysMem *g_pSysMem;

#endif