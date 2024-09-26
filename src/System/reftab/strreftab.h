#pragma once

#include "reftab.h"

class StrRefTab : public RefTab
{
public:
    StrRefTab(int p_poolSize, int p_size);

    virtual ~StrRefTab();

    // Overriden
    void Clear();
    void ClearThis();
    void PrintStatus();
    void RunDelRef(RefRun *p_refRun);

    // Added
    virtual char *FindStr(char *const p_str);
    virtual char *AddStr(char *const p_str);
    virtual char *AddAlways(char *const p_str);
    virtual void RemoveStr(char *const p_str);
    virtual bool Exists(char *const p_str);
    virtual void Sort();

    void Destroy();
};

int Compare(const void *p_a, const void *p_b);
bBool CompareCall(const char *p_str1, const char *p_str2);
