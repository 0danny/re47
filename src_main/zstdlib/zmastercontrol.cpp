#include "zmastercontrol.h"

ZMasterControl::ZMasterControl() : m_myStr()
{
    m_unkInt1 = 0;

    m_equTab = new EquRefTab(8, 0);
    m_refTab = new RefTab(8, 2);
}

ZMasterControl::~ZMasterControl()
{
    UnkFunc8();

    if (m_equTab)
        delete m_equTab;

    if (m_refTab)
        delete m_refTab;
}

void ZMasterControl::UnkFunc1(char *p_bankName, char *p_fileName, bool p_flag)
{
    printf("ZMasterControl::UnkFunc1(%s, %s, %d)\n", p_bankName, p_fileName, p_flag);

    MyStr l_myStr3;
    MyStr l_myStr2 = MyStr(p_bankName);

    m_myStr = l_myStr2;

    l_myStr2.DeleteString();

    if (*p_fileName)
    {
        MyStr l_filePath = MyStr();

        if (!strchr(p_fileName, '\\'))
        {
            l_myStr3 = MyStr(p_bankName);

            l_filePath.DeleteString();

            l_filePath = MyStr(l_myStr3);

            l_myStr3.DeleteString();

            l_myStr2 = MyStr("\\");

            l_filePath += l_myStr2;
        }

        l_myStr3 = MyStr(p_fileName);

        l_filePath += l_myStr3;

        u32 l_size = g_pSysFile->GetSize(*l_filePath, p_flag); // always returns -1, because the file doesn't exist.

        if (m_unkInt1)
            delete m_unkInt1;

        if (l_size == -1)
        {
            m_unkInt1 = 0;
        }
        else
        {
            m_unkInt1 = (i32 *)operator new(l_size);

            if (g_pSysFile->UnkFunc2(*l_filePath, m_unkInt1, l_size, 0, p_flag) != l_size)
            {
                delete m_unkInt1;
                m_unkInt1 = 0;
            }
        }
    }
}

bool ZMasterControl::UnkFunc2(i32 p_bankId, i32 p_objectId)
{
    printf("ZMasterControl::UnkFunc2(%d, %d)\n", p_bankId, p_objectId);

    if (m_equTab->FindEqu(p_objectId))
        return FALSE;

    m_equTab->AddEqu(1, p_objectId);

    return TRUE;
}

bool ZMasterControl::UnkFunc3(i32 p_masterBank, char *p_str)
{
    printf("ZMasterControl::UnkFunc3(%d, %s)\n", p_masterBank, p_str);

    return NULL;
}

u32 *ZMasterControl::UnkFunc4(char *p_str, i32 p_unkInt)
{
    printf("ZMasterControl::UnkFunc4(%s, %d)\n", p_str, p_unkInt);

    return NULL;
}

MyStr *ZMasterControl::UnkFunc5(MyStr *p_str, char *p_str2)
{
    printf("ZMasterControl::UnkFunc5(%s, %s)\n", *p_str, p_str2);

    MyStr l_myStr1;

    MyStr l_myStr3 = MyStr("\\");

    m_myStr = l_myStr1 + l_myStr3;

    MyStr l_myStr2 = MyStr(p_str2);

    l_myStr1 += l_myStr2;

    l_myStr2.DeleteString();

    l_myStr2 = MyStr("\\MASTERBANK.PTH");

    l_myStr1 += l_myStr2;

    p_str->SetString(*l_myStr1);

    return p_str;
}

char *ZMasterControl::UnkFunc6(i32 p_unkInt1, i32 p_unkInt2)
{
    printf("ZMasterControl::UnkFunc6(%d, %d)\n", p_unkInt1, p_unkInt2);

    return NULL;
}

MyStr *ZMasterControl::UnkFunc7(MyStr *p_str, i32 p_unkInt1, i32 p_unkInt2)
{
    printf("ZMasterControl::UnkFunc7(%s, %d, %d)\n", *p_str, p_unkInt1, p_unkInt2);

    return NULL;
}

void ZMasterControl::UnkFunc8()
{
    printf("ZMasterControl::UnkFunc8()\n");
}
