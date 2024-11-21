#ifndef STRREFTAB_H
#define STRREFTAB_H

#include "reftab.h"

class StrRefTab : public RefTab
{
public:
    StrRefTab(i32 p_poolSize, i32 p_size);

    virtual ~StrRefTab();

    // Overriden
    virtual void Clear() override;
    virtual void ClearThis() override;
    virtual void PrintStatus() override;
    virtual void RunDelRef(RefRun *p_refRun) override;

    // Added
    virtual char *FindStr(char *const p_str);
    virtual char *AddStr(char *const p_str);
    virtual char *AddAlways(char *const p_str);
    virtual void RemoveStr(char *const p_str);
    virtual boolean Exists(char *const p_str);
    virtual void Sort();

    static i32 Compare(const void *p_a, const void *p_b);
    static boolean CompareCall(const char *p_str1, const char *p_str2);

    void Destroy();
};

#endif