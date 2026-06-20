#include "zsysfile.h"
#include "zsysmem.h"
#include "zsysinterface.h"
#include "zstdlib/reftab/strreftab.h"

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif


ZSysFile::ZSysFile()
{
}

ZSysFile::~ZSysFile()
{
}

const char *ZSysFile::GetRelativePath(const char *p_path)
{
    if (m_strRefTab)
    {
        const char *l_dot = strrchr(p_path, '.');
        if (l_dot)
        {
            const char *l_ext = l_dot;
            if (_stricmp(l_ext, ".mor") == 0)
            {
                l_ext = ".anm";
            }

            RefRun l_refRun;
            m_strRefTab->RunInitNxtRef(&l_refRun);
            for (char *l_str = (char *)m_strRefTab->RunNxtRef(&l_refRun); l_refRun.prev; l_str = (char *)m_strRefTab->RunNxtRef(&l_refRun))
            {
                if (_stricmp(l_str, l_ext) == 0)
                {
                    return NULL;
                }
            }
        }
    }

    if (g_pSysInterface)
    {
        const char *l_projectDir = *g_pSysInterface->m_curProjectStr;
        u32 l_projectDirLen = strlen(l_projectDir);
        u32 l_pathLen = strlen(p_path);

        if (l_projectDirLen <= l_pathLen)
        {
            if (_strnicmp(l_projectDir, p_path, l_projectDirLen) == 0)
            {
                const char *l_relative = p_path + l_projectDirLen;
                if (l_relative[0] == '\\')
                {
                    l_relative++;
                }
                return l_relative;
            }
        }
    }

    return p_path;
}

void ZSysFile::UnkFunc1()
{
}

void ZSysFile::Restart()
{
}

void ZSysFile::PrintStatus()
{
}

HANDLE ZSysFile::Open(const char *p_fileName)
{
    HANDLE l_hFile = CreateFileA(p_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (l_hFile == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    return l_hFile;
}

HANDLE ZSysFile::OpenForAppend(const char *p_fileName)
{
    HANDLE l_hFile = CreateFileA(p_fileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (l_hFile == INVALID_HANDLE_VALUE)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 86)
                  ->LogMessage("ZSysFileWintel::OpenForAppend: Couldn't open file %s", p_fileName);
        return NULL;
    }
    if (SetFilePointer(l_hFile, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 90)
                  ->LogMessage("ZSysFileWintel::OpenForAppend: Couldn't set FilePointer correctly for %s", p_fileName);
    }
    return l_hFile;
}

HANDLE ZSysFile::OpenForRandomAccess(const char *p_fileName)
{
    HANDLE l_hFile = CreateFileA(p_fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL, NULL);
    if (l_hFile == INVALID_HANDLE_VALUE)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 75)
                  ->LogMessage("ZSysFileWintel::OpenForRandomAccess: Couldn't open file %s", p_fileName);
        return NULL;
    }
    return l_hFile;
}

HANDLE ZSysFile::Create(const char *p_fileName)
{
    HANDLE l_hFile = CreateFileA(p_fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (l_hFile == INVALID_HANDLE_VALUE)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 225)
                  ->LogMessage("ZSysFileWintel::Create: Couldn't create file %s", p_fileName);
        return NULL;
    }
    return l_hFile;
}

bool ZSysFile::CreateAlt(const char *p_fileName)
{
    HANDLE l_hFile = CreateFileA(p_fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (l_hFile == INVALID_HANDLE_VALUE)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 236)
                  ->LogMessage("ZSysFileWintel::Create: Couldn't create file %s", p_fileName);
        return false;
    }
    Close(l_hFile);
    return true;
}

void ZSysFile::Close(HANDLE p_handle)
{
    if (!CloseHandle(p_handle))
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 97)
                  ->LogMessage("ZSysFileWintel::Close: Couldn't close file with handle %d", p_handle);
    }
}

bool ZSysFile::GetFileTime(char *p_unkStr, LPFILETIME p_lastWriteTime, bool p_unkFlag)
{
    if (!p_unkFlag)
    {
        const char *l_relativePath = GetRelativePath(p_unkStr);
        if (l_relativePath)
        {
            i32 l_archive = UnkFunc21((i32)l_relativePath);
            if (l_archive)
            {
                class DummyArchive {};
                typedef bool (DummyArchive::*GetFileTime_t)(const char*, LPFILETIME);
                DummyArchive *l_archiveObj = reinterpret_cast<DummyArchive *>(l_archive);
                void **l_vtable = *reinterpret_cast<void ***>(l_archiveObj);
                GetFileTime_t GetFileTimeFunc;
                *reinterpret_cast<void **>(&GetFileTimeFunc) = l_vtable[10];
                if ((l_archiveObj->*GetFileTimeFunc)(l_relativePath, p_lastWriteTime))
                {
                    return true;
                }
            }
        }
    }

    HANDLE l_hFile = Open(p_unkStr);
    if (!l_hFile)
    {
        return false;
    }

    FILETIME l_creationTime;
    FILETIME l_lastAccessTime;
    bool l_success = ::GetFileTime(l_hFile, &l_creationTime, &l_lastAccessTime, p_lastWriteTime) != 0;
    Close(l_hFile);
    return l_success;
}

u32 ZSysFile::GetSize(const char *p_filePath, bool p_unkFlag)
{
    if (!p_unkFlag)
    {
        const char *l_relativePath = GetRelativePath(p_filePath);
        if (l_relativePath)
        {
            i32 l_archive = UnkFunc21((i32)l_relativePath);
            if (l_archive)
            {
                i32 l_size = UnkFunc22(l_archive, (i32)l_relativePath);
                if (l_size != -1)
                {
                    return l_size;
                }

                class DummyArchive {};
                typedef i32 (DummyArchive::*GetSizeInArchive_t)(const char*);
                DummyArchive *l_subObj = *reinterpret_cast<DummyArchive **>((char *)l_archive + 12);
                if (l_subObj)
                {
                    void **l_vtable = *reinterpret_cast<void ***>(l_subObj);
                    GetSizeInArchive_t GetSizeInArchive;
                    *reinterpret_cast<void **>(&GetSizeInArchive) = l_vtable[3];
                    l_size = (l_subObj->*GetSizeInArchive)(l_relativePath);
                    if (l_size != -1)
                    {
                        return l_size;
                    }
                }
            }
        }
    }

    HANDLE l_hFile = Open(p_filePath);
    if (!l_hFile)
    {
        return -1;
    }

    DWORD l_fileSize = GetFileSize(l_hFile, NULL);
    if (l_fileSize == INVALID_FILE_SIZE)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 212)
                  ->LogMessage("ZSysFileWintel::GetSize failed");
    }

    Close(l_hFile);
    return l_fileSize;
}

bool ZSysFile::Exists(char *p_fileName, char p_unkFlag)
{
    if (!p_unkFlag)
    {
        const char *l_relativePath = GetRelativePath(p_fileName);
        if (l_relativePath)
        {
            i32 l_archive = UnkFunc21((i32)l_relativePath);
            if (l_archive)
            {
                class DummyArchive {};
                typedef bool (DummyArchive::*CheckInArchive_t)(const char*);
                DummyArchive *l_subObj = *reinterpret_cast<DummyArchive **>((char *)l_archive + 12);
                if (l_subObj)
                {
                    void **l_vtable = *reinterpret_cast<void ***>(l_subObj);
                    CheckInArchive_t CheckInArchive;
                    *reinterpret_cast<void **>(&CheckInArchive) = l_vtable[4];
                    if ((l_subObj->*CheckInArchive)(l_relativePath))
                    {
                        return true;
                    }
                }

                if (UnkFunc23(l_archive, l_relativePath))
                {
                    return true;
                }
            }
        }
    }

    WIN32_FIND_DATAA l_findData;
    HANDLE l_hFind = FindFirstFileA(p_fileName, &l_findData);
    if (l_hFind != INVALID_HANDLE_VALUE)
    {
        FindClose(l_hFind);
        return true;
    }

    return false;
}

bool ZSysFile::WriteTo(HANDLE p_fileHandle, const void *p_buffer, u32 p_bytesWritten)
{
    DWORD l_bytesWritten = 0;
    if (!WriteFile(p_fileHandle, p_buffer, p_bytesWritten, &l_bytesWritten, NULL))
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 277)
                  ->LogMessage("ZSysFileWintel::WriteTo failed");
        return false;
    }
    return true;
}

void *ZSysFile::ReadFrom(HANDLE p_fileHandle, void *p_buffer, char *p_bytesWritten)
{
    DWORD l_bytesRead = 0;
    if (!ReadFile(p_fileHandle, p_buffer, reinterpret_cast<u32>(p_bytesWritten), &l_bytesRead, NULL))
    {
        char *l_errorMsg = NULL;
        DWORD l_lastError = GetLastError();
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            l_lastError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&l_errorMsg,
            0,
            NULL
        );

        MessageBoxA(NULL, l_errorMsg, "Error", MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);

        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 302)
                  ->LogMessage("ZSysFileWintel::ReadFrom failed %s", l_errorMsg);

        LocalFree(l_errorMsg);
        return p_buffer;
    }

    return reinterpret_cast<void *>(l_bytesRead);
}

void ZSysFile::CD(const char *p_fileName)
{
    if (!SetCurrentDirectoryA(p_fileName))
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 311)->LogMessage("ZSysFileWintel::CD failed");
    }
}

void ZSysFile::MakeDir(const char *p_unkStr)
{
    u32 l_len = strlen(p_unkStr) + 1;
    char *l_dir = (char *)g_pSysMem->New(l_len);
    strcpy(l_dir, p_unkStr);

    u32 l_strLen = strlen(l_dir);
    if (l_strLen > 0 && l_dir[l_strLen - 1] == '\\')
    {
        l_dir[l_strLen - 1] = '\0';
    }

    if (!Exists(l_dir, 0))
    {
        if (!CreateDirectoryA(l_dir, NULL))
        {
            g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 323)->LogMessage("ZSysFileWintel::MakeDir failed");
        }
    }

    g_pSysMem->Delete(l_dir);
}

void ZSysFile::Rename(const char *p_fileName, const char *p_newFileName)
{
    if (!MoveFileA(p_fileName, p_newFileName))
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 331)->LogMessage("ZSysFileWintel::Rename failed");
    }
}

void ZSysFile::Copy(const char *p_oldFileLoc, const char *p_newFileLoc)
{
    if (!CopyFileA(p_oldFileLoc, p_newFileLoc, TRUE))
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysFileWintel.cpp", 337)->LogMessage("ZSysFileWintel::Copy failed");
    }
}

bool ZSysFile::Delete(const char *p_fileName)
{
    return DeleteFileA(p_fileName) != 0;
}

u32 ZSysFile::UnkFunc2(const char *p_unkStr, void *p_buffer, u32 p_elemCount, i32 p_offset, bool p_unkFlag)
{
    return NULL;
}

void ZSysFile::UnkFunc3()
{
}

i32 ZSysFile::UnkFunc4(char *p_unkStr, u32 *p_unkInt)
{
    return NULL;
}

i32 ZSysFile::UnkFunc5(i32 p_unkInt1, i32 p_unkInt2, i32 p_unkInt3, i32 p_unkInt4)
{
    return NULL;
}

i32 ZSysFile::UnkFunc6(i32 p_unkInt1, i32 p_unkInt2, i32 p_unkInt3)
{
    return NULL;
}

void ZSysFile::UnkFunc7(const char *p_unkStr)
{
}

void ZSysFile::UnkFunc8(i32 p_unkInt1, i32 p_unkInt2, i32 *p_unkInt3)
{
}

i32 ZSysFile::UnkFunc9()
{
    return NULL;
}

void ZSysFile::UnkFunc10(char *p_unkStr)
{
}

bool ZSysFile::UnkFunc11(i32 p_unkInt)
{
    return NULL;
}

bool ZSysFile::UnkFunc12(i32 p_unkInt)
{
    return NULL;
}

bool ZSysFile::UnkFunc13(char *p_unkStr)
{
    return NULL;
}

void ZSysFile::UnkFunc14(char *p_unkStr)
{
}

void ZSysFile::UnkFunc15(char *p_unkStr)
{
}

bool ZSysFile::UnkFunc16(char *p_unkStr)
{
    if (m_linkRefTab)
    {
        RefRun l_refRun;
        m_linkRefTab->RunInitNxtRef(&l_refRun);
        for (u32 *l_entry = m_linkRefTab->RunNxtRefPtr(&l_refRun); l_refRun.prev; l_entry = m_linkRefTab->RunNxtRefPtr(&l_refRun))
        {
            u32 *l_objPtr = *reinterpret_cast<u32 **>(l_entry);
            u32 l_flags = *(l_entry + 1);
            if (l_flags & 1)
            {
                char *l_name = *reinterpret_cast<char **>(l_objPtr + 2); // eax + 8
                if (_stricmp(p_unkStr, l_name) == 0)
                {
                    return true;
                }
            }
            else
            {
                char *l_name = *reinterpret_cast<char **>(l_objPtr + 7); // eax + 28 (28 / 4 = 7)
                if (_stricmp(p_unkStr, l_name) == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void ZSysFile::UnkFunc17(bool p_unkFlag)
{
}

HMODULE ZSysFile::LoadLib(char *p_unkStr)
{
    const char *l_fileName = strrchr(p_unkStr, '\\');
    if (l_fileName)
    {
        l_fileName++;
    }
    else
    {
        l_fileName = p_unkStr;
    }

    if (m_linkRefTab2)
    {
        RefRun l_refRun;
        m_linkRefTab2->RunInitNxtRef(&l_refRun);
        for (u32 *l_entry = m_linkRefTab2->RunNxtRefPtr(&l_refRun); l_refRun.prev; l_entry = m_linkRefTab2->RunNxtRefPtr(&l_refRun))
        {
            char *l_entryName = *reinterpret_cast<char **>(l_entry);
            if (_stricmp(l_entryName, l_fileName) == 0)
            {
                HMODULE *l_pHmod = reinterpret_cast<HMODULE *>(l_entry + 1);
                i32 *l_pRefCount = reinterpret_cast<i32 *>(l_entry + 2);
                (*l_pRefCount)++;
                return *l_pHmod;
            }
        }
    }

    HMODULE l_hMod = LoadLibraryA(p_unkStr);
    u32 l_len = strlen(l_fileName) + 1;
    char *l_newKey = static_cast<char *>(g_pSysMem->New(l_len));
    strcpy(l_newKey, l_fileName);

    if (m_linkRefTab2)
    {
        u32 *l_newEntry = m_linkRefTab2->Add(reinterpret_cast<u32>(l_newKey));
        if (l_newEntry)
        {
            *reinterpret_cast<HMODULE *>(l_newEntry + 1) = l_hMod;
            *reinterpret_cast<i32 *>(l_newEntry + 2) = 1;
            *reinterpret_cast<u32 *>(l_newEntry + 3) = 0;
            *reinterpret_cast<u32 *>(l_newEntry + 4) = 0;
        }
    }

    return l_hMod;
}

bool ZSysFile::FreeLib(bool p_freeAll)
{
    if (m_linkRefTab)
    {
        RefRun l_refRun;
        m_linkRefTab->RunInitNxtRef(&l_refRun);
        for (u32 *l_entry = m_linkRefTab->RunNxtRefPtr(&l_refRun); l_refRun.prev; l_entry = m_linkRefTab->RunNxtRefPtr(&l_refRun))
        {
            u32 l_flags = *(l_entry + 1);
            if (p_freeAll || (l_flags & 1))
            {
                if (l_flags & 1)
                {
                    class DummyLoader {};
                    typedef void (DummyLoader::*FreeMem_t)();
                    DummyLoader *l_loader = *reinterpret_cast<DummyLoader **>(l_entry);
                    void **l_vtable = *reinterpret_cast<void ***>(l_loader);
                    FreeMem_t FreeMemFunc;
                    *reinterpret_cast<void **>(&FreeMemFunc) = l_vtable[1];
                    (l_loader->*FreeMemFunc)();
                    
                    if (l_loader)
                    {
                        g_pSysMem->Delete(l_loader);
                    }
                }
                else
                {
                    class DummySub {};
                    DummySub *l_subObj = *reinterpret_cast<DummySub **>(l_entry);
                    if (l_subObj)
                    {
                        void **l_vtable = *reinterpret_cast<void ***>(l_subObj);
                        typedef void (DummySub::*Destroy_t)(char);
                        Destroy_t DestroyFunc;
                        *reinterpret_cast<void **>(&DestroyFunc) = l_vtable[6];
                        (l_subObj->*DestroyFunc)(1);
                    }
                }
                m_linkRefTab->RunDelRef(&l_refRun);
            }
        }
        
        if (m_linkRefTab->GetCount() == 0)
        {
            delete m_linkRefTab;
            m_linkRefTab = NULL;
        }
    }
    return true;
}

i32 ZSysFile::UnkFunc20(char *p_unkStr, i32 *p_unkInt1, i32 *p_unkInt2)
{
    return NULL;
}

i32 ZSysFile::UnkFunc21(i32 p_unkInt)
{
    return NULL;
}

i32 ZSysFile::UnkFunc22(i32 p_unkInt1, i32 p_unkInt2)
{
    return NULL;
}

bool ZSysFile::UnkFunc23(i32 p_unkInt1, const char *p_unkStr)
{
    return NULL;
}

u32 ZSysFile::UnkFunc24(FILE *p_stream, i32 p_offset, u32 p_buffer)
{
    return NULL;
}

void ZSysFile::UnkFunc25(i32 *p_unkInt1, i32 p_unkInt2, char *p_unkStr, i32 p_unkInt3, i32 p_unkInt4)
{
}

HANDLE ZSysFile::AddBig(const char *p_unkStr)
{
    return NULL;
}
