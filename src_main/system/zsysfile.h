#ifndef ZSYSFILE_H
#define ZSYSFILE_H

#include "common.h"
#include "zstdlib/reftab/linkreftab.h"

#pragma pack(push, 1)

class ZSysFile
{
public:
    LinkRefTab *m_linkRefTab;  // 4
    StrRefTab *m_strRefTab;    // 8
    LinkRefTab *m_linkRefTab2; // 12
    RefTab *m_refTab;          // 16

    ZSysFile();

    virtual void UnkFunc1(char *p_unkStr);
    virtual void Restart();
    virtual void PrintStatus();
    virtual HANDLE Open(const char *p_fileName);
    virtual HANDLE OpenForAppend(const char *p_fileName);
    virtual HANDLE OpenForRandomAccess(const char *p_fileName);
    virtual HANDLE Create(const char *p_fileName);
    virtual boolean CreateAlt(const char *p_fileName);
    virtual void Close(HANDLE p_handle);
    virtual boolean GetFileTime(char *p_unkStr, LPFILETIME p_lastWriteTime, boolean p_unkFlag);
    virtual u32 GetSize(const char *p_filePath, boolean p_unkFlag);
    virtual boolean Exists(char *p_fileName, char p_unkFlag);
    virtual boolean WriteTo(HANDLE p_fileHandle, const void *p_buffer, u32 p_bytesWritten);
    virtual void *ReadFrom(HANDLE p_fileHandle, void *p_buffer, char *p_bytesWritten);
    virtual void CD(const char *p_fileName);
    virtual void MakeDir(const char *p_unkStr);
    virtual void Rename(const char *p_fileName, const char *p_newFileName);
    virtual void Copy(const char *p_oldFileLoc, const char *p_newFileLoc);
    virtual boolean Delete(const char *p_fileName);

    virtual u32 UnkFunc2(const char *p_unkStr, void *p_buffer, u32 p_elemCount, i32 p_offset, boolean p_unkFlag);
    virtual void UnkFunc3();
    virtual i32 UnkFunc4(char *p_unkStr, u32 *p_unkInt);
    virtual i32 UnkFunc5(i32 p_unkInt1, i32 p_unkInt2, i32 p_unkInt3, i32 p_unkInt4);
    virtual i32 UnkFunc6(i32 p_unkInt1, i32 p_unkInt2, i32 p_unkInt3);
    virtual void UnkFunc7(const char *p_unkStr);
    virtual void UnkFunc8(i32 p_unkInt1, i32 p_unkInt2, i32 *p_unkInt3);
    virtual i32 UnkFunc9();
    virtual void UnkFunc10(char *p_unkStr);
    virtual boolean UnkFunc11(i32 p_unkInt);
    virtual boolean UnkFunc12(i32 p_unkInt);
    virtual boolean UnkFunc13(char *p_unkStr);
    virtual void UnkFunc14(char *p_unkStr);
    virtual void UnkFunc15(char *p_unkStr);
    virtual boolean UnkFunc16(char *p_unkStr);
    virtual void UnkFunc17(boolean p_unkFlag);
    virtual HMODULE LoadLib(char *p_unkStr);
    virtual boolean FreeLib(HMODULE p_hModule);
    virtual i32 UnkFunc20(char *p_unkStr, i32 *p_unkInt1, i32 *p_unkInt2);

    virtual ~ZSysFile();

    virtual i32 UnkFunc21(i32 p_unkInt);
    virtual i32 UnkFunc22(i32 p_unkInt1, i32 p_unkInt2);
    virtual boolean UnkFunc23(i32 p_unkInt1, const char *p_unkStr);
    virtual u32 UnkFunc24(FILE *p_stream, i32 p_offset, u32 p_buffer);
    virtual void UnkFunc25(i32 *p_unkInt1, i32 p_unkInt2, char *p_unkStr, i32 p_unkInt3, i32 p_unkInt4);
    virtual HANDLE AddBig(const char *p_unkStr);

}; // 20 in size.

#pragma pack(pop)

extern __declspec(dllimport) ZSysFile *g_pSysFile;

#endif