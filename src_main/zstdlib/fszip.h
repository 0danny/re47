#ifndef FSZIP_H
#define FSZIP_H

#include "common.h"
#include "utilities/dynamicarray.h"
#include "reftab/strreftab.h"
#include "system/zsysmem.h"
#include "zstdlib/cfastlookupfilecache.h"

#include <zlib/zlib.h>

#pragma pack(push, 1)

class CFastLookupFileCache;

#define EOCD_SIGNATURE 0x6054B50     // End of Central Directory signature
#define CDIR_SIGNATURE 0x2014B50     // Central Directory Header signature
#define LFHEADER_SIGNATURE 0x4034B50 // Local File Header signature
#define EOCD2_SIGNATURE 0x52756E65   // Secondary End of Central Directory signature

struct IOZip_CentralDir
{
    u16 versionMade;       // 0
    u16 versionNeeded;     // 2
    u16 generalPurpose;    // 4
    u16 compressionMethod; // 6
    u16 lastModTime;       // 8
    u16 lastModDate;       // 10
    u32 crc32;             // 12
    u32 compressedSize;    // 16
    u32 uncompressedSize;  // 20
    u16 filenameLength;    // 24
    u16 extraFieldLength;  // 26
    u16 commentLength;     // 28
    u16 diskNumberStart;   // 30
    u16 internalAttrs;     // 32
    u32 externalAttrs;     // 34
    u32 relativeOffset;    // 38
}; // 42 in size.

struct IOZip_EOCD
{
    u16 diskNumber;       // 0
    u16 centralDirDisk;   // 2
    u16 numEntriesOnDisk; // 4
    u16 numEntriesTotal;  // 6
    u32 centralDirSize;   // 8
    u32 centralDirOffset; // 12
    u16 commentLength;    // 16
}; // 18 in size.

struct IOFSHandleZip
{
    i32 access;                       // 0
    IOZip_LocalFileHeader fileHeader; // 4
    i32 currentOffset;                // 30
    i32 initialOffset;                // 34
    z_stream zlibStream;              // 38
    i32 size;                         // 94
    u8 buffer[1024];                  // 98
    i32 status;                       // 1122
}; //  1126 in size.

class IOZip
{
public:
    i32 m_FSStatus;                            // 12
    u8 m_FSInit;                               // 16
    FILE *m_fileStream;                        // 17
    i32 m_eocdOffset;                          // 21
    i32 m_compressionLevel;                    // 25
    u8 m_archiveType;                          // 29
    IOZip_EOCD m_ecod1;                        // 30
    IOZip_EOCD m_ecod2;                        // 48
    DynamicArray<IOZip_CentralDir> m_dirList1; // 66
    DynamicArray<IOZip_CentralDir> m_dirList2; // 78
    DynamicArray<i32> m_longList;              // 90
    CFastLookupFileCache *m_fastLookupCache;   // 102

    inline IOZip() : m_dirList1(), m_dirList2(), m_longList()
    {
        m_fastLookupCache = 0;
    }

    virtual void PrintStatus();
    virtual bool Add(char *p_fileName, char *p_data);
    virtual void Save(char *p_unused);
    virtual i32 GetSize(char *p_fileName);
    virtual bool Exists(char *p_fileName);
    virtual i32 Load(char *p_fileName, u8 *p_nextOut, i32 p_size, i32 p_unused);
    virtual void GetDirectory(StrRefTab *p_strTab);

    void AddCompressedFile(char *p_data, FILETIME *p_fileTime, u8 *p_nextIn, u32 p_len);
    void Zip(IOZip_CentralDir *p_centralDir, u8 *p_nextIn);

    bool FindFile(char *p_fileName, IOZip_LocalFileHeader *p_fileHeader, i32 *p_centralDirOffset);

    u8 *ExtractData(IOZip_LocalFileHeader *p_header, u8 *p_nextOut, bool p_unused, i32 p_elemCount);
    i32 RegexMatch(char *p_str1, char *p_str2);

}; // 98 in size.

class FSZip
{
public:
    i32 m_status;     // 4
    char *m_fileName; // 8
    IOZip m_ioZip;    // 12

    FSZip();
    ~FSZip();

    virtual bool InitFS(char *p_fileName, i32 p_mode);

    virtual inline void UnloadFS()
    {
        if (m_fileName)
        {
            WriteCDirs();
            WriteEOCD();

            m_ioZip.m_dirList1.Cleanup();
            m_ioZip.m_dirList2.Cleanup();

            delete[] m_fileName;

            m_fileName = 0;

            fclose(m_ioZip.m_fileStream);

            m_ioZip.m_FSInit = 0;
            m_status = 0;

            if (m_ioZip.m_fastLookupCache)
                delete m_ioZip.m_fastLookupCache;
        }
    }

    virtual IOFSHandleZip *Open(char *p_fileName, i32 p_ioFSAccess);
    virtual u32 Read(IOFSHandleZip *p_fsHandle, u8 *p_buffer, u32 p_bytesToRead);
    virtual i32 Write(IOFSHandleZip *p_fsHandle, u8 *p_buffer, u32 p_bytesToWrite);
    virtual void Close(IOFSHandleZip *p_fsHandle);

    virtual bool EndOfFile(IOFSHandleZip *p_fsHandle);
    virtual bool Compare(char *p_fileName1, char *p_fileName2);
    virtual bool InvalidateFile(char *p_fileName);

    virtual void AddFileWithTimestamp(char *p_data, FILETIME *p_fileTime, u8 *p_nextIn, u32 p_len);
    virtual bool GetFileTime(char *p_fileName, FILETIME *p_fileTime);
    virtual void SetCompressionLevel(i32 p_compressionLevel);
    virtual void FreeMemory();
    virtual bool InitFSCurrent();

    virtual void UsePrimaryArchive();
    virtual void UseSecondaryArchive();

    virtual void InvalidateRedundantFiles(FSZip *p_zip);
    virtual bool ImportFromArchive(char *p_fileName);

    void InitalizeFileCache();
    void WriteCDirs();

    i32 FindEOCDOffset();

    inline void WriteEOCD()
    {
        i32 l_signature = EOCD_SIGNATURE;

        fwrite(&l_signature, 1u, 4u, m_ioZip.m_fileStream);
        fwrite(&m_ioZip.m_ecod1, 1u, 18u, m_ioZip.m_fileStream);

        l_signature = EOCD2_SIGNATURE;

        fwrite(&l_signature, 1u, 4u, m_ioZip.m_fileStream);
        fwrite(&m_ioZip.m_ecod2, 1u, 18u, m_ioZip.m_fileStream);
    }

    static inline void NormalizePath(char *p_sourcePath, char *p_targetPath)
    {
        i32 l_index;
        char *l_sourcePtr;
        char *l_targetPtr;

        l_index = 0;

        if (*p_sourcePath)
        {
            l_sourcePtr = p_sourcePath;
            l_targetPtr = p_targetPath;

            do
            {
                if (*l_sourcePtr == '\\')
                    *l_targetPtr = '/';
                else
                    *l_targetPtr = *l_sourcePtr;

                l_sourcePtr = &p_sourcePath[++l_index];
                ++l_targetPtr;

            } while (p_sourcePath[l_index]);
        }

        p_targetPath[l_index] = 0;
    }

}; // 110 in size.

#pragma pack(pop)

#endif