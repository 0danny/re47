#include "fszip.h"
#include "cfastlookupfilecache.h"
#include <time.h>

/* FSZIP */

FSZip::FSZip() : m_ioZip()
{
    m_fileName = 0;
    m_ioZip.m_FSInit = 0;

    m_status = 0;

    m_ioZip.m_compressionLevel = -1;
    m_ioZip.m_archiveType = 0;

    m_ioZip.m_longList.Init(256);
}

FSZip::~FSZip()
{
    UnloadFS();

    m_ioZip.m_longList.Cleanup();
    m_ioZip.m_dirList2.Cleanup();
    m_ioZip.m_dirList1.Cleanup();
}

bool FSZip::InitFS(char *p_fileName, i32 p_mode)
{
    if (m_fileName)
        UnloadFS();

    char *l_mode = "wb+";

    if (p_mode != 1)
        l_mode = "rb";

    m_ioZip.m_fileStream = fopen(p_fileName, l_mode);

    if (m_ioZip.m_fileStream)
    {
        m_fileName = new char[strlen(p_fileName) + 1];
        strcpy(m_fileName, p_fileName);

        m_ioZip.m_FSInit = 1;
        m_status = p_mode;

        m_ioZip.m_dirList1.Init(256);
        m_ioZip.m_dirList2.Init(8);

        m_ioZip.m_ecod1.diskNumber = 0;
        m_ioZip.m_ecod1.numEntriesOnDisk = 0;
        m_ioZip.m_ecod1.centralDirSize = 0;
        m_ioZip.m_ecod1.centralDirOffset = 0;
        m_ioZip.m_ecod1.commentLength = 0;

        m_ioZip.m_ecod2.diskNumber = 0;
        m_ioZip.m_ecod2.numEntriesOnDisk = 0;
        m_ioZip.m_ecod2.centralDirSize = 0;
        m_ioZip.m_ecod2.centralDirOffset = 0;
        m_ioZip.m_ecod2.commentLength = 0;

        FindEOCDOffset();

        m_ioZip.m_fastLookupCache = new CFastLookupFileCache();

        if (p_mode != 1)
            InitalizeFileCache();

        return true;
    }
    else
    {
        printf("ZIPFS: Cannot initialize archive\n");
        m_status = 0;

        return false;
    }
}

IOFSHandleZip *FSZip::Open(char *p_fileName, i32 p_ioFSAccess)
{
    IOFSHandleZip *l_IOFSHandle = new IOFSHandleZip;

    i32 l_compSize;
    i32 l_offset;

    if (!l_IOFSHandle)
        return 0;

    l_IOFSHandle->currentOffset = 0;
    l_IOFSHandle->access = p_ioFSAccess;

    if (p_ioFSAccess != 3)
        goto DELETE_HANDLE;

    if (!m_ioZip.FindFile(p_fileName, &l_IOFSHandle->fileHeader, 0))
        goto DELETE_HANDLE;

    l_offset = ftell(m_ioZip.m_fileStream);

    l_compSize = l_IOFSHandle->fileHeader.compressedSize;

    l_IOFSHandle->currentOffset = l_offset;
    l_IOFSHandle->initialOffset = l_offset;
    l_IOFSHandle->size = l_compSize;
    l_IOFSHandle->zlibStream.zalloc = 0;
    l_IOFSHandle->zlibStream.zfree = 0;
    l_IOFSHandle->zlibStream.next_in = l_IOFSHandle->buffer;
    l_IOFSHandle->zlibStream.avail_in = 0;

    l_IOFSHandle->status = inflateInit2_(&l_IOFSHandle->zlibStream, -15, "1.1.3", 56);

    if (l_IOFSHandle->status)
    {
    DELETE_HANDLE:
        delete l_IOFSHandle;
        return 0;
    }

    return l_IOFSHandle;
}

u32 FSZip::Read(IOFSHandleZip *p_fsHandle, u8 *p_buffer, u32 p_bytesToRead)
{
    FSZip *l_this = this;
    fseek(m_ioZip.m_fileStream, p_fsHandle->currentOffset, 0);

    if (p_fsHandle->fileHeader.compressionMethod)
    {
        if (p_fsHandle->fileHeader.compressionMethod == 8)
        {
            if (p_fsHandle->status)
            {
                if (p_fsHandle->status == 1)
                    return 0;
                else
                    return -1;
            }
            else
            {
                p_fsHandle->zlibStream.next_out = p_buffer;

                for (p_fsHandle->zlibStream.avail_out = p_bytesToRead; p_fsHandle->zlibStream.avail_out; l_this = this)
                {
                    if (p_fsHandle->status)
                        break;

                    i32 l_index = 1024 - p_fsHandle->zlibStream.avail_in;

                    memcpy(p_fsHandle->buffer, &p_fsHandle->buffer[l_index], p_fsHandle->zlibStream.avail_in);

                    if (p_fsHandle->fileHeader.compressedSize - p_fsHandle->currentOffset < l_index)
                        l_index = p_fsHandle->fileHeader.compressedSize - p_fsHandle->currentOffset;

                    if (l_index)
                        p_fsHandle->zlibStream.avail_in = fread(p_fsHandle->buffer, 1u, l_index, m_ioZip.m_fileStream);

                    p_fsHandle->zlibStream.next_in = p_fsHandle->buffer;

                    p_fsHandle->status = inflate(&p_fsHandle->zlibStream, 3);

                    switch (p_fsHandle->status)
                    {
                    case 0:
                    case 1:
                        continue;
                    default:
                        printf("dut");
                        break;
                    }
                }

                u32 l_availOut = p_fsHandle->zlibStream.avail_out;
                p_fsHandle->currentOffset = ftell(l_this->m_ioZip.m_fileStream);

                return p_bytesToRead - l_availOut;
            }
        }
        else
        {
            printf("ZIPFS: unsupported compression method\n");
            return p_bytesToRead - p_fsHandle->zlibStream.avail_out;
        }
    }
    else
    {
        fseek(l_this->m_ioZip.m_fileStream, p_fsHandle->currentOffset, 0);
        return fread(p_buffer, 1u, p_bytesToRead, l_this->m_ioZip.m_fileStream);
    }
}

i32 FSZip::Write(IOFSHandleZip *p_fsHandle, u8 *p_buffer, u32 p_bytesToWrite)
{
    printf("ZIPFS: Stream write not implemented yet\n");
    return -1;
}

void FSZip::Close(IOFSHandleZip *p_fsHandle)
{
    inflateEnd(&p_fsHandle->zlibStream);
    delete p_fsHandle;
}

bool FSZip::EndOfFile(IOFSHandleZip *p_fsHandle)
{
    return p_fsHandle->status == 1;
}

bool FSZip::Compare(char *p_fileName1, char *p_fileName2)
{
    IOZip_LocalFileHeader l_header1;
    IOZip_LocalFileHeader l_header2;

    return m_ioZip.FindFile(p_fileName1, &l_header1, 0) &&
           m_ioZip.FindFile(p_fileName2, &l_header2, 0) &&
           l_header1.compressedSize == l_header2.compressedSize &&
           l_header1.crc32 == l_header2.crc32;
}

bool FSZip::InvalidateFile(char *p_fileName)
{
    IOZip_LocalFileHeader l_header;
    char l_buffer[65540];
    i32 l_result;

    bool l_foundFile = m_ioZip.FindFile(p_fileName, &l_header, &l_result);

    if (l_foundFile)
    {
        *m_ioZip.m_longList.Add() = l_result;

        if (m_ioZip.m_fastLookupCache)
        {
            FSZip::NormalizePath(p_fileName, l_buffer);
            m_ioZip.m_fastLookupCache->Invalidate(l_buffer);
        }
    }

    return l_foundFile;
}

void FSZip::AddFileWithTimestamp(char *p_data, FILETIME *p_fileTime, u8 *p_nextIn, u32 p_len)
{
    m_ioZip.AddCompressedFile(p_data, p_fileTime, p_nextIn, p_len);
}

bool FSZip::GetFileTime(char *p_fileName, FILETIME *p_fileTime)
{
    IOZip_LocalFileHeader l_fileHeader;

    if (!m_ioZip.FindFile(p_fileName, &l_fileHeader, 0))
        return false;

    DosDateTimeToFileTime(l_fileHeader.lastModDate, l_fileHeader.lastModTime, p_fileTime);

    return true;
}

void FSZip::SetCompressionLevel(i32 p_compressionLevel)
{
    m_ioZip.m_compressionLevel = p_compressionLevel;
}

void FSZip::FreeMemory()
{
    m_ioZip.m_dirList1.Cleanup();
    m_ioZip.m_dirList2.Cleanup();
    m_ioZip.m_longList.Cleanup();

    fclose(m_ioZip.m_fileStream);

    m_ioZip.m_FSInit = 0;

    if (m_ioZip.m_fastLookupCache)
        delete m_ioZip.m_fastLookupCache;
}

bool FSZip::InitFSCurrent()
{
    char l_buffer[264];

    strncpy(l_buffer, m_fileName, 260u);
    l_buffer[260] = '\0';

    delete[] m_fileName;

    m_fileName = 0;

    return InitFS(l_buffer, m_status);
}

void FSZip::UsePrimaryArchive()
{
    m_ioZip.m_archiveType = 0;
}

void FSZip::UseSecondaryArchive()
{
    m_ioZip.m_archiveType = 1;
}

void FSZip::InvalidateRedundantFiles(FSZip *p_zip)
{
    IOZip_CentralDir l_centralDir;
    IOZip_LocalFileHeader l_fileHeader;

    char l_buffer1[65540];
    char l_buffer2[65540];

    bool l_flag = false;
    fseek(p_zip->m_ioZip.m_fileStream, p_zip->m_ioZip.m_ecod1.centralDirOffset, 0);

    for (FILE *l_item = p_zip->m_ioZip.m_fileStream; feof(l_item) == 0; l_item = p_zip->m_ioZip.m_fileStream)
    {
        if (l_flag)
            break;

        ftell(l_item);

        i32 l_signature;
        fread(&l_signature, 1u, 4u, p_zip->m_ioZip.m_fileStream);
        ftell(p_zip->m_ioZip.m_fileStream);

        if (l_signature == CDIR_SIGNATURE)
        {
            fread(&l_centralDir, sizeof(IOZip_CentralDir), 1u, p_zip->m_ioZip.m_fileStream);

            if (l_centralDir.filenameLength)
            {
                fread(l_buffer1, 1u, l_centralDir.filenameLength, p_zip->m_ioZip.m_fileStream);
                l_buffer1[l_centralDir.filenameLength] = 0;
                i32 l_offset = ftell(p_zip->m_ioZip.m_fileStream);

                if (m_ioZip.FindFile(l_buffer1, &l_fileHeader, 0))
                {
                    // my formatter won't let me format this properly, sorry ):
                    if ((l_fileHeader.lastModDate & 0xFE00) >= (l_centralDir.lastModDate & 0xFE00) && ((l_fileHeader.lastModDate & 0xFE00) > (l_centralDir.lastModDate & 0xFE00) || (l_fileHeader.lastModDate & 0x1E0) >= (l_centralDir.lastModDate & 0x1E0) && ((l_fileHeader.lastModDate & 0x1E0) > (l_centralDir.lastModDate & 0x1E0) || (l_fileHeader.lastModDate & 0x1F) >= (l_centralDir.lastModDate & 0x1F) && ((l_fileHeader.lastModDate & 0x1F) > (l_centralDir.lastModDate & 0x1F) || (l_fileHeader.lastModTime & 0xF800) >= (l_centralDir.lastModTime & 0xF800) && ((l_fileHeader.lastModTime & 0xF800) > (l_centralDir.lastModTime & 0xF800) || (l_fileHeader.lastModTime & 0x7E0) >= (l_centralDir.lastModTime & 0x7E0) && ((l_fileHeader.lastModTime & 0x7E0) > (l_centralDir.lastModTime & 0x7E0) || (l_fileHeader.lastModTime & 0x1F) >= (l_centralDir.lastModTime & 0x1F)))))))
                        p_zip->InvalidateFile(l_buffer1);
                    else
                        InvalidateFile(l_buffer1);
                }

                fseek(p_zip->m_ioZip.m_fileStream, l_offset, 0);
            }

            if (l_centralDir.extraFieldLength)
                fread(l_buffer2, 1u, l_centralDir.extraFieldLength, p_zip->m_ioZip.m_fileStream);

            if (l_centralDir.commentLength)
                fread(l_buffer2, 1u, l_centralDir.commentLength, p_zip->m_ioZip.m_fileStream);
        }
        else if (l_signature == EOCD_SIGNATURE || l_signature == EOCD2_SIGNATURE)
            l_flag = 1;
        else
            printf("ZIPFS: Broken .zip archive\n");
    }
}

bool FSZip::ImportFromArchive(char *p_fileName)
{
    FILETIME l_fileTime;
    IOZip_CentralDir l_centralDir;
    IOZip_LocalFileHeader l_fileHeader;

    char l_fileNameBuffer[65540];
    char l_extraBuffer[65540];

    bool l_flag = 0;
    FSZip l_zipFS = FSZip();

    if (l_zipFS.InitFS(p_fileName, 3))
    {
        fseek(l_zipFS.m_ioZip.m_fileStream, l_zipFS.m_ioZip.m_ecod1.centralDirOffset, SEEK_SET);

        for (FILE *l_item = l_zipFS.m_ioZip.m_fileStream; feof(l_zipFS.m_ioZip.m_fileStream) == 0; l_item = l_zipFS.m_ioZip.m_fileStream)
        {
            if (l_flag)
                break;

            ftell(l_item);

            i32 l_signature;
            fread(&l_signature, 1u, 4u, l_zipFS.m_ioZip.m_fileStream);

            ftell(l_zipFS.m_ioZip.m_fileStream);

            if (l_signature == CDIR_SIGNATURE)
            {
                fread(&l_centralDir, sizeof(IOZip_CentralDir), 1u, l_zipFS.m_ioZip.m_fileStream);

                if (l_centralDir.filenameLength)
                {
                    fread(l_fileNameBuffer, 1u, l_centralDir.filenameLength, l_zipFS.m_ioZip.m_fileStream);
                    i32 l_offset = ftell(l_zipFS.m_ioZip.m_fileStream);

                    i32 l_uncompressedSize = l_centralDir.uncompressedSize;
                    l_fileNameBuffer[l_centralDir.filenameLength] = '\0';
                    i32 l_newUncompSize = l_uncompressedSize + 1;

                    i32 l_uncompSize = l_centralDir.uncompressedSize;
                    u8 *l_alloc = new u8[l_newUncompSize];

                    if (l_zipFS.m_ioZip.FindFile(l_fileNameBuffer, &l_fileHeader, 0))
                    {
                        if (!l_uncompSize)
                            l_uncompSize = l_fileHeader.uncompressedSize;

                        l_zipFS.m_ioZip.ExtractData(&l_fileHeader, l_alloc, 0, l_uncompSize);
                    }

                    fseek(l_zipFS.m_ioZip.m_fileStream, l_offset, SEEK_SET);
                    DosDateTimeToFileTime(l_centralDir.lastModDate, l_centralDir.lastModTime, &l_fileTime);
                    m_ioZip.AddCompressedFile(l_fileNameBuffer, &l_fileTime, l_alloc, l_centralDir.uncompressedSize);

                    delete[] l_alloc;
                }

                if (l_centralDir.extraFieldLength)
                    fread(l_extraBuffer, 1u, l_centralDir.extraFieldLength, l_zipFS.m_ioZip.m_fileStream);

                if (l_centralDir.commentLength)
                    fread(l_extraBuffer, 1u, l_centralDir.commentLength, l_zipFS.m_ioZip.m_fileStream);
            }
            else if (l_signature == EOCD_SIGNATURE || l_signature == EOCD2_SIGNATURE)
                l_flag = true;
            else
                printf("ZIPFS: Broken .zip archive\n");
        }

        return true;
    }
    else
        return false;
}

i32 FSZip::FindEOCDOffset()
{
    fseek(m_ioZip.m_fileStream, 0, 2);

    i32 l_startOfEOCD = ftell(m_ioZip.m_fileStream) - 22;

    fseek(m_ioZip.m_fileStream, l_startOfEOCD, 0);

    if (l_startOfEOCD < 0)
        return -1;

    i32 l_signature;

    while (true)
    {
        fread(&l_signature, 1u, 4u, m_ioZip.m_fileStream);

        if (l_signature != EOCD2_SIGNATURE)
            break;

        fread(&m_ioZip.m_ecod2, 1u, 18u, m_ioZip.m_fileStream);

        fseek(m_ioZip.m_fileStream, -18, 1);

    GO_LOOP:
        --l_startOfEOCD;

        fseek(m_ioZip.m_fileStream, -5, 1);

        if (l_startOfEOCD < 0)
            return -1;
    }

    if (l_signature != EOCD_SIGNATURE)
        goto GO_LOOP;

    m_ioZip.m_eocdOffset = ftell(m_ioZip.m_fileStream);

    fread(&m_ioZip.m_ecod1, 1u, 18u, m_ioZip.m_fileStream);

    return m_ioZip.m_eocdOffset;
}

void FSZip::InitalizeFileCache()
{
    u16 l_filenameLen;
    i32 l_signature;
    IOZip_CentralDir l_centralDir;
    IOZip_LocalFileHeader l_header;

    char l_buffer[65540];
    char l_extraField[65540];

    if (m_ioZip.m_ecod2.centralDirSize)
        fseek(m_ioZip.m_fileStream, m_ioZip.m_ecod2.centralDirOffset, 0);
    else
        fseek(m_ioZip.m_fileStream, m_ioZip.m_ecod1.centralDirOffset, 0);

    for (FILE *l_item = m_ioZip.m_fileStream; feof(l_item) == 0; l_item = m_ioZip.m_fileStream)
    {
        bool l_flag = 0;
        fread(&l_signature, 1u, 4u, l_item);

        if (l_signature == CDIR_SIGNATURE)
        {
            i32 l_offset = ftell(m_ioZip.m_fileStream);
            fread(&l_centralDir, 1u, sizeof(IOZip_CentralDir), m_ioZip.m_fileStream);

            l_filenameLen = l_centralDir.filenameLength;

            if (l_centralDir.filenameLength)
            {
                fread(l_buffer, 1u, l_centralDir.filenameLength, m_ioZip.m_fileStream);
                l_filenameLen = l_centralDir.filenameLength;
                l_buffer[l_centralDir.filenameLength] = '\0';

                i32 l_count = 0;

                if (m_ioZip.m_longList.m_currentSize)
                {
                    i32 *l_array3 = m_ioZip.m_longList.m_data;

                    while (l_offset != *l_array3)
                    {
                        ++l_count;
                        ++l_array3;

                        if (l_count >= m_ioZip.m_longList.m_currentSize)
                            goto EXTRA_LENGTH;
                    }
                }
                else
                {
                EXTRA_LENGTH:
                    memcpy(&l_header, &l_centralDir.versionNeeded, 24u);
                    l_header.extraFieldLength = l_centralDir.extraFieldLength;
                    l_flag = 1;
                }
            }

            if (l_centralDir.extraFieldLength)
            {
                fread(l_extraField, 1u, l_centralDir.extraFieldLength, m_ioZip.m_fileStream);
                l_filenameLen = l_centralDir.filenameLength;
            }

            if (l_centralDir.commentLength)
            {
                fread(l_extraField, 1u, l_centralDir.commentLength, m_ioZip.m_fileStream);
                l_filenameLen = l_centralDir.filenameLength;
            }

            if (l_flag)
                m_ioZip.m_fastLookupCache->Add(l_buffer, &l_header, l_filenameLen + l_centralDir.relativeOffset + 30);
        }
        else
        {
            if (l_signature == EOCD_SIGNATURE || l_signature == EOCD2_SIGNATURE)
                return;

            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\IOZip.cpp", 491);
            l_sysCom->LogMessage("ZIPFS: Broken .zip archive\n");
        }
    }
}

void FSZip::WriteCDirs()
{
    char l_buffer[65540];
    i32 l_signature = CDIR_SIGNATURE;

    m_ioZip.m_ecod2.centralDirOffset = ftell(m_ioZip.m_fileStream);

    i32 l_count = 0;
    l_signature = CDIR_SIGNATURE;

    if (m_ioZip.m_dirList2.m_currentSize)
    {
        i32 l_index = 0;

        do
        {
            IOZip_CentralDir *l_data = &m_ioZip.m_dirList2.m_data[l_index];
            fwrite(&l_signature, 1u, 4u, m_ioZip.m_fileStream);
            fwrite(l_data, 1u, sizeof(IOZip_CentralDir), m_ioZip.m_fileStream);

            i32 l_offset1 = ftell(m_ioZip.m_fileStream);

            fseek(m_ioZip.m_fileStream, l_data->relativeOffset + 30, 0);
            fread(l_buffer, 1u, l_data->filenameLength, m_ioZip.m_fileStream);
            fseek(m_ioZip.m_fileStream, l_offset1, 0);

            fwrite(l_buffer, 1u, l_data->filenameLength, m_ioZip.m_fileStream);

            ++m_ioZip.m_ecod2.numEntriesOnDisk;

            ++l_count;
            ++l_index;

        } while (l_count < m_ioZip.m_dirList2.m_currentSize);
    }

    m_ioZip.m_ecod2.numEntriesTotal = m_ioZip.m_ecod2.numEntriesOnDisk;
    i32 l_offset2 = ftell(m_ioZip.m_fileStream);

    m_ioZip.m_ecod2.centralDirSize = l_offset2 - m_ioZip.m_ecod2.centralDirOffset;
    m_ioZip.m_ecod1.centralDirOffset = ftell(m_ioZip.m_fileStream);
    i32 l_count2 = 0;

    if (m_ioZip.m_dirList1.m_currentSize)
    {
        i32 l_index2 = 0;

        do
        {
            IOZip_CentralDir *l_data2 = &m_ioZip.m_dirList1.m_data[l_index2];
            fwrite(&l_signature, 1u, 4u, m_ioZip.m_fileStream);
            fwrite(l_data2, 1u, sizeof(IOZip_CentralDir), m_ioZip.m_fileStream);

            i32 l_offset3 = ftell(m_ioZip.m_fileStream);

            fseek(m_ioZip.m_fileStream, l_data2->relativeOffset + 30, 0);
            fread(l_buffer, 1u, l_data2->filenameLength, m_ioZip.m_fileStream);
            fseek(m_ioZip.m_fileStream, l_offset3, 0);

            fwrite(l_buffer, 1u, l_data2->filenameLength, m_ioZip.m_fileStream);

            ++m_ioZip.m_ecod1.numEntriesOnDisk;

            ++l_count2;
            ++l_index2;

        } while (l_count2 < m_ioZip.m_dirList1.m_currentSize);
    }

    m_ioZip.m_ecod1.numEntriesTotal = m_ioZip.m_ecod1.numEntriesOnDisk;
    m_ioZip.m_ecod1.centralDirSize = ftell(m_ioZip.m_fileStream) - m_ioZip.m_ecod1.centralDirOffset;
}

/* IOZIP */

void IOZip::PrintStatus()
{
    if (m_FSInit)
    {
        i32 l_offset = ftell(m_fileStream);

        // i don't see another way to implement this?
        char *l_fileName = (char *)(this - 4);

        printf("fs: '%s' Offset = %i, status = %i\n", l_fileName, l_offset, m_FSStatus);
    }
    else
        printf("fs not initialized\n");
}

bool IOZip::Add(char *p_fileName, char *p_data)
{
    // i don't see another way to implement this, part2 ?
    if (((i32)(this - 8)) != 1)
    {
        printf("IOZIP: Cannot add file - filesystem not writable\n");
        return false;
    }

    FILE *l_stream = fopen(p_fileName, "rb");
    FILE *l_streamPtr = l_stream;

    if (!l_stream)
        return false;

    fseek(l_stream, 0, SEEK_END);
    i32 l_offset = ftell(l_streamPtr);

    if (l_offset)
    {
        u8 *l_bufferSpace = new u8[l_offset];
        u8 *l_bufferPtr = l_bufferSpace;

        if (!l_bufferSpace)
        {
            printf("ZIPFS: Cannot allocate buffer space for file '%s'\n", p_fileName);
            return false;
        }

        fseek(l_streamPtr, 0, SEEK_SET);
        fread(l_bufferPtr, 1u, l_offset, l_streamPtr);

        AddCompressedFile(p_data, 0, l_bufferPtr, l_offset);

        if (g_pSysMem)
        {
            delete l_bufferPtr;
            fclose(l_streamPtr);
            return true;
        }

        free(l_bufferPtr);
    }

    fclose(l_streamPtr);

    return true;
}

void IOZip::Save(char *p_unused)
{
    printf("ZIPFS: Save: This method is unsupported by this filesystem\n");
}

i32 IOZip::GetSize(char *p_fileName)
{
    IOZip_LocalFileHeader p_localFileHeader;

    if (FindFile(p_fileName, &p_localFileHeader, 0))
        return p_localFileHeader.uncompressedSize;
    else
        return -1;
}

bool IOZip::Exists(char *p_fileName)
{
    IOZip_LocalFileHeader p_localFileHeader;

    return FindFile(p_fileName, &p_localFileHeader, 0);
}

i32 IOZip::Load(char *p_fileName, u8 *p_nextOut, i32 p_size, i32 p_unused)
{
    IOZip_LocalFileHeader p_localFileHeader;

    if (!FindFile(p_fileName, &p_localFileHeader, 0))
        return -1;

    i32 l_size = p_size;

    if (!p_size)
        l_size = p_localFileHeader.uncompressedSize;

    if (ExtractData(&p_localFileHeader, p_nextOut, 0, l_size))
        return l_size;
    else
        return -1;
}

void IOZip::GetDirectory(StrRefTab *p_strTab)
{
    IOZip_CentralDir l_centralDir;
    char l_buffer[65540];

    if (m_ecod2.centralDirSize)
        fseek(m_fileStream, m_ecod2.centralDirOffset, SEEK_SET);
    else
        fseek(m_fileStream, m_ecod1.centralDirOffset, SEEK_SET);

    for (FILE *l_item = m_fileStream; feof(l_item) == 0; l_item = m_fileStream)
    {
        i32 l_signature;
        fread(&l_signature, 1u, 4u, l_item);

        if (l_signature == CDIR_SIGNATURE)
        {
            i32 l_offset = ftell(m_fileStream);

            fread(&l_centralDir, 1u, sizeof(IOZip_CentralDir), m_fileStream);

            if (l_centralDir.filenameLength)
            {
                fread(l_buffer, 1u, l_centralDir.filenameLength, m_fileStream);
                l_buffer[l_centralDir.filenameLength] = 0;

                i32 l_count = 0;
                if (m_longList.m_currentSize)
                {
                    i32 *l_list = m_longList.m_data;

                    while (l_offset != *l_list)
                    {
                        ++l_count;
                        ++l_list;

                        if (l_count >= m_longList.m_currentSize)
                            goto ADD_REF_LBL;
                    }
                }
                else
                {
                ADD_REF_LBL:
                    p_strTab->AddAlways(l_buffer);
                }
            }
        }
        else
        {
            if (l_signature == EOCD_SIGNATURE || l_signature == EOCD2_SIGNATURE)
                return;

            printf("ZIPFS: Broken .zip archive\n");
        }
    }
}

void IOZip::AddCompressedFile(char *p_data, FILETIME *p_fileTime, u8 *p_nextIn, u32 p_len)
{
    IOZip_CentralDir *l_centralDir;
    time_t *l_timeStruct;
    char l_buffer[65540];

    FSZip::NormalizePath(p_data, l_buffer);

    if (m_archiveType)
        l_centralDir = m_dirList2.Add();
    else
        l_centralDir = m_dirList1.Add();

    l_centralDir->uncompressedSize = p_len;
    l_centralDir->generalPurpose = 0;
    l_centralDir->compressedSize = 0;
    l_centralDir->crc32 = 0;
    l_centralDir->versionNeeded = 20;
    l_centralDir->compressionMethod = 8;
    l_centralDir->extraFieldLength = 0;
    l_centralDir->filenameLength = strlen(l_buffer);

    if (p_fileTime)
    {
        u16 *l_lastModTime = &l_centralDir->lastModTime;
        u16 *l_lastModDate = &l_centralDir->lastModDate;

        if (!FileTimeToDosDateTime(p_fileTime, &l_centralDir->lastModDate, &l_centralDir->lastModTime))
        {
            time(l_timeStruct);
            tm *l_localTime = localtime(l_timeStruct);

            if (*l_lastModDate)
            {
                u16 l_mDay = l_localTime->tm_mday;
                *l_lastModDate = l_mDay;
                u16 l_dayCalc = l_mDay | (32 * (l_localTime->tm_mon + 1));

                *l_lastModDate = l_dayCalc;
                *l_lastModDate = l_dayCalc | ((l_localTime->tm_year + 48) << 9);
            }

            if (*l_lastModTime)
            {
                u16 l_secCalc = l_localTime->tm_sec / 2;
                *l_lastModTime = l_secCalc;
                u16 l_hourCalc = l_secCalc | (32 * l_localTime->tm_min);

                *l_lastModTime = l_hourCalc;
                *l_lastModTime = l_hourCalc | (l_localTime->tm_hour << 11);
            }
        }
    }
    else
    {
        u16 *l_lastModDate2 = &l_centralDir->lastModDate;

        time(l_timeStruct);
        tm *l_localTime2 = localtime(l_timeStruct);

        if (l_centralDir->lastModDate)
        {
            u16 l_mDay2 = l_localTime2->tm_mday;
            *l_lastModDate2 = l_mDay2;
            u16 l_dayCalc2 = l_mDay2 | (32 * (l_localTime2->tm_mon + 1));

            *l_lastModDate2 = l_dayCalc2;
            *l_lastModDate2 = l_dayCalc2 | ((l_localTime2->tm_year + 48) << 9);
        }

        if (l_centralDir->lastModTime)
        {
            u16 l_secCalc2 = l_localTime2->tm_sec / 2;
            l_centralDir->lastModTime = l_secCalc2;
            u16 l_hourCalc2 = l_secCalc2 | (32 * l_localTime2->tm_min);

            l_centralDir->lastModTime = l_hourCalc2;
            l_centralDir->lastModTime = l_hourCalc2 | (l_localTime2->tm_hour << 11);
        }
    }

    i32 l_crc1 = crc32(0, 0, 0);
    l_centralDir->crc32 = l_crc1;
    l_centralDir->crc32 = crc32(l_crc1, p_nextIn, p_len);

    i32 l_offset2 = ftell(m_fileStream);
    i32 l_signature = LFHEADER_SIGNATURE;

    fwrite(&l_signature, 1u, 4u, m_fileStream);
    fwrite(&l_centralDir->versionNeeded, 1u, 26u, m_fileStream);
    fwrite(l_buffer, 1u, l_centralDir->filenameLength, m_fileStream);

    Zip(l_centralDir, p_nextIn);

    if (l_centralDir->compressedSize >= l_centralDir->uncompressedSize)
    {
        l_centralDir->compressedSize = 0;
        l_centralDir->compressionMethod = 0;

        fseek(m_fileStream, l_offset2 + 4, SEEK_SET);
        fwrite(&l_centralDir->versionNeeded, 1u, 26u, m_fileStream);
        fwrite(l_buffer, 1u, l_centralDir->filenameLength, m_fileStream);
        Zip(l_centralDir, p_nextIn);
    }

    i32 l_offset3 = ftell(m_fileStream);
    fseek(m_fileStream, l_offset2 + 4, SEEK_SET);
    fwrite(&l_centralDir->versionNeeded, 1u, 26u, m_fileStream);
    fseek(m_fileStream, l_offset3, SEEK_SET);

    l_centralDir->relativeOffset = l_offset2;
    l_centralDir->versionMade = 1558;
    l_centralDir->commentLength = 0;
    l_centralDir->diskNumberStart = 0;
    l_centralDir->internalAttrs = 0;
    l_centralDir->externalAttrs = 0;
}

bool IOZip::FindFile(char *p_fileName, IOZip_LocalFileHeader *p_fileHeader, i32 *p_centralDirOffset)
{
    i32 l_signature;
    IOZip_CentralDir l_centralDir;
    char l_buffer1[65540];
    char l_buffer2[65540];
    char l_buffer3[65540];

    bool l_flag = false;

    FSZip::NormalizePath(p_fileName, l_buffer2);

    char *l_starLoc = strchr(p_fileName, '*');
    i32 l_starLocPtr = (i32)l_starLoc;

    if (!l_starLoc)
    {
        if (m_fastLookupCache)
        {
            if (!p_centralDirOffset)
            {
                l_starLocPtr = -1;

                if (m_fastLookupCache->Lookup(l_buffer2, p_fileHeader, &l_starLocPtr))
                {
                    fseek(m_fileStream, l_starLocPtr, SEEK_SET);
                    return true;
                }

                return false;
            }
        }
    }

    if (m_ecod2.centralDirSize)
        fseek(m_fileStream, m_ecod2.centralDirOffset, SEEK_SET);
    else
        fseek(m_fileStream, m_ecod1.centralDirOffset, SEEK_SET);

    if (feof(m_fileStream) != FALSE)
        return false;

    while (true)
    {
        fread(&l_signature, 1u, 4u, m_fileStream);

        if (l_signature == CDIR_SIGNATURE)
            break;

        if (l_signature == EOCD_SIGNATURE || l_signature == EOCD2_SIGNATURE)
            return false;

        printf("ZIPFS: Broken .zip archive\n");
    CHECK_EOF_LBL:

        if (feof(m_fileStream) != FALSE)
            return false;
    }

    i32 l_offset = ftell(m_fileStream);
    fread(&l_centralDir, 1u, sizeof(IOZip_CentralDir), m_fileStream);

    if (l_centralDir.filenameLength)
    {
        fread(l_buffer1, 1u, l_centralDir.filenameLength, m_fileStream);

        i32 l_curSize = m_longList.m_currentSize;
        i32 l_count2 = 0;

        l_buffer1[l_centralDir.filenameLength] = '\0';

        if (!l_curSize)
        {
        CHECK_EXTRA_LBL:
            if (l_starLoc)
            {
                if (RegexMatch(l_buffer1, l_buffer2))
                    goto END_CHECKS_LBL;
            }
            else if (strlen(l_buffer2) != l_centralDir.filenameLength || _strcmpi(l_buffer1, l_buffer2))
            {
                goto END_CHECKS_LBL;
            }

            memcpy(p_fileHeader, &l_centralDir.versionNeeded, 24u);
            p_fileHeader->extraFieldLength = l_centralDir.extraFieldLength;

            l_flag = true;
            goto END_CHECKS_LBL;
        }

        i32 *l_data = m_longList.m_data;

        while (l_offset != *l_data)
        {
            ++l_count2;
            ++l_data;

            if (l_count2 >= l_curSize)
                goto CHECK_EXTRA_LBL;
        }
    }
END_CHECKS_LBL:

    if (l_centralDir.extraFieldLength)
        fread(l_buffer3, 1u, l_centralDir.extraFieldLength, m_fileStream);

    if (l_centralDir.commentLength)
        fread(l_buffer3, 1u, l_centralDir.commentLength, m_fileStream);

    if (!l_flag)
    {
        l_starLoc = (char *)l_starLocPtr;
        goto CHECK_EOF_LBL;
    }

    if (p_centralDirOffset)
        *p_centralDirOffset = l_offset;

    fseek(m_fileStream, l_centralDir.filenameLength + l_centralDir.relativeOffset + 30, SEEK_SET);

    if (m_fastLookupCache)
    {
        i32 l_offset3 = ftell(m_fileStream);
        m_fastLookupCache->Add(l_buffer2, p_fileHeader, l_offset3);
    }

    return true;
}

i32 IOZip::RegexMatch(char *p_str1, char *p_str2)
{
    i32 l_diff;
    char *l_loc;
    char *l_locPtr;
    char *l_str2Ptr = p_str2;

    if (!*p_str2)
        return 0;

    while (*l_str2Ptr != '*')
    {
        l_loc = (char *)strchr(l_str2Ptr, '*');
        l_locPtr = l_loc;

        if (!l_loc)
            return _strcmpi(l_str2Ptr, p_str1);

        l_diff = l_loc - l_str2Ptr;

        if (_memicmp(l_str2Ptr, p_str1, l_loc - l_str2Ptr))
            return -1;

        p_str1 += l_diff;
        l_str2Ptr = l_locPtr;

    RETURN_ZERO_LBL:

        if (!*l_str2Ptr)
            return 0;
    }

    char *l_bufferPtr = l_str2Ptr + 1;
    char *l_loc2 = strchr(l_bufferPtr, '*');
    char *l_loc2Ptr = l_loc2;

    if (l_loc2)
    {
        i32 l_diff2 = l_loc2 - l_bufferPtr;

        if (l_loc2 == l_bufferPtr)
            return 0;

        i32 l_len = strlen(p_str1) + 1;

        if ((l_len - 1 - l_diff2) < 0)
            return -1;

        i32 l_count = 0;

        while (_memicmp(l_bufferPtr, p_str1, l_diff2))
        {
            ++p_str1;

            if (++l_count == l_len - l_diff2)
                return -1;
        }

        l_str2Ptr = l_loc2Ptr;

        goto RETURN_ZERO_LBL;
    }

    if (strlen(p_str1) >= strlen(l_bufferPtr))
        return _strcmpi(&p_str1[strlen(p_str1) - strlen(l_bufferPtr)], l_bufferPtr);
    else
        return 1;
}

u8 *IOZip::ExtractData(IOZip_LocalFileHeader *p_header, u8 *p_nextOut, bool p_unused, i32 p_elemCount)
{
    i32 l_zErrorCode2;
    u8 *l_nextOut;
    z_stream l_zstream;
    u8 l_buffer[1024];

    if (p_header->compressionMethod)
    {
        if (p_header->compressionMethod != 8)
        {
            printf("ZIPFS: unsupported compression method\n");
            return 0;
        }

        u32 l_compSize = p_header->compressedSize;
        u8 *l_nextOut = p_nextOut;
        l_zstream.next_out = p_nextOut;
        l_zstream.avail_out = p_elemCount;
        l_zstream.zalloc = 0;
        l_zstream.zfree = 0;

        i32 l_zErrorCode = inflateInit2_(&l_zstream, -15, "1.1.3", 56);
        m_FSStatus = l_zErrorCode;

        if (l_zErrorCode)
            return 0;

        for (; l_compSize; m_FSStatus = l_zErrorCode2)
        {
            l_zstream.next_in = l_buffer;
            i32 l_sizeLeft = 1024;

            if (l_compSize <= 1024)
                l_sizeLeft = l_compSize;

            l_zstream.avail_in = fread(l_buffer, 1u, l_sizeLeft, m_fileStream);
            l_zErrorCode2 = inflate(&l_zstream, l_sizeLeft < l_compSize ? 1 : 4);

            l_compSize -= l_sizeLeft;
        }

        inflateEnd(&l_zstream);

        if (m_FSStatus != 1 && (m_FSStatus != -5 || l_zstream.avail_out))
            return 0;
    }
    else
    {
        l_nextOut = p_nextOut;
        fread(p_nextOut, 1u, p_elemCount, m_fileStream);
    }

    return l_nextOut;
}

void IOZip::Zip(IOZip_CentralDir *p_centralDir, u8 *p_nextIn)
{
    i32 l_errCode;
    z_stream l_strm;
    u8 l_next_out[0x4000];

    i32 l_index = 2;

    if (p_centralDir->compressionMethod)
    {
        if (p_centralDir->compressionMethod == 8)
        {
            l_strm.next_in = p_nextIn;

            l_strm.zalloc = 0;
            l_strm.zfree = 0;
            l_strm.avail_in = p_centralDir->uncompressedSize;

            if (deflateInit_(&l_strm, m_compressionLevel, "1.1.3", 56))
            {
                printf("ZIPFS: Cannot initialize compression library\n");
            }
            else
            {
                do
                {
                    l_strm.next_out = l_next_out;
                    l_strm.avail_out = 0x4000;

                    l_errCode = deflate(&l_strm, 4);

                    i32 l_elemCount = 0x4000 - l_strm.avail_out - l_index;

                    p_centralDir->compressedSize += l_elemCount;

                    if (fwrite(&l_next_out[l_index], 1u, l_elemCount, m_fileStream) != l_elemCount)
                    {
                        deflateEnd(&l_strm);
                        printf("ZIPFS: Write error: Cannot write to archive\n");
                    }

                    l_index = 0;

                } while (l_errCode != 1);

                deflateEnd(&l_strm);
            }
        }
        else
        {
            printf("ZIPFS: Unrecognized compression method selected");
        }
    }
    else
    {
        if (fwrite(p_nextIn, 1u, p_centralDir->uncompressedSize, m_fileStream) < p_centralDir->uncompressedSize)
            printf("ZIPFS: Write error while trying to store file data\n");

        p_centralDir->compressedSize = p_centralDir->uncompressedSize;
    }
}
