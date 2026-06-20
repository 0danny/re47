#include "zsysmem.h"
#include "zsyscom.h"
#include <cstdlib>
#include <cstring>
#include <windows.h>

ZSysMem::ZSysMem()
{
    unkByte1 = 1; // m_isInitialized
    unkByte3 = 1; // m_useZMalloc
    
    // Construct ZMalloc inline or set its properties
    m_zMalloc.m_unkByte2 = 1;
    
    unkInt7 = 0; // m_head
    unkInt8 = 0; // m_tail
    unkInt9 = 0;
    m_nrAllocs = 0;
    m_nrAllocsCheck = 0;
    m_memAllocated = 0;
    unkInt13 = 0; // m_peakMemAllocated
    unkInt14 = 0;
    
    m_equTab2 = new EquRefTab(256, 0);
    m_equTab = new EquRefTab(256, 0);
    m_allocTab = new AllocRefTab();
    
    u32 l_maxRefIndex = m_allocTab->m_maxRefIndex;
    m_refArray = new u32[l_maxRefIndex + 1];
    memset(m_refArray, 0, (l_maxRefIndex + 1) * sizeof(u32));
    m_allocArray = m_allocTab->m_array;
}

ZSysMem::~ZSysMem()
{
    if (m_equTab2) delete m_equTab2;
    if (m_equTab) delete m_equTab;
    if (m_allocTab) delete m_allocTab;
    if (m_refArray) delete[] m_refArray;
}

void ZSysMem::UnkFunc0(i32 param1, i32 param2)
{
}

char *ZSysMem::UnkFunc1()
{
    return 0;
}

void ZSysMem::AllocCheck()
{
    SMallocBlockHeader *l_node = reinterpret_cast<SMallocBlockHeader *>(unkInt7);
    int l_count = 0;
    while (l_node)
    {
        AllocCheck(l_node);
        l_count++;
        if (l_count > m_nrAllocsCheck * 3)
        {
            g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 198);
            g_pSysCom->LogMessage("AllocCheck chain is probably corrupted, aborting Check");
            break;
        }
        l_node = reinterpret_cast<SMallocBlockHeader *>(l_node->checksumComponents[0]); // prev
    }
    
    if (l_count != m_nrAllocsCheck)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 203);
        g_pSysCom->LogMessage("NrAllocsCheck is incorrect (is %d, should be %d).", l_count, m_nrAllocsCheck);
    }
    
    if (m_zMalloc.m_unkInt0 != 0)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 205);
        g_pSysCom->LogMessage("WARNING: At Exit %d bytes was not freed", m_zMalloc.m_unkInt0);
    }
}

void ZSysMem::AllocCheck(SMallocBlockHeader *p_header)
{
    if (p_header->flag & 0x80000000)
    {
        u32 l_checksum = p_header->checksumComponents[0] ^
                         p_header->checksumComponents[1] ^
                         p_header->checksumComponents[2] ^
                         p_header->checksumComponents[3] ^
                         p_header->unkInt ^
                         p_header->flag ^
                         p_header->referenceNumber;
        if (l_checksum == 0)
            l_checksum = 1;
            
        if (l_checksum != p_header->checksum)
        {
            g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 225);
            g_pSysCom->LogMessage("Memory underrun for"); // Fatal error dispatch
            DebugBreak();
        }
    }
    
    u32 l_size = p_header->flag & 0x1FFFFFFF;
    u32 *l_sentinel = reinterpret_cast<u32 *>(reinterpret_cast<char *>(p_header) + 32 + l_size);
    if (*l_sentinel != 0x87654321)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 232);
        g_pSysCom->LogMessage("Memory overrun for");
        DebugBreak();
    }
}

char ZSysMem::UnkFunc4(u32 *param)
{
    return 0;
}

i32 ZSysMem::UnkFunc5()
{
    return 0;
}

void ZSysMem::NullSub1()
{
}

void ZSysMem::NullSub2()
{
}

void *ZSysMem::NewRef(void *p_link)
{
    u32 l_newRef = m_allocTab->NewRef();
    m_refArray[l_newRef & m_allocTab->m_maxRefIndex] = reinterpret_cast<u32>(p_link);
    
    SMallocBlockHeader *header = reinterpret_cast<SMallocBlockHeader *>(reinterpret_cast<char *>(p_link) - 32);
    header->referenceNumber = l_newRef;
    
    if (header->flag & 0x80000000)
    {
        u32 l_checksum = header->checksumComponents[0] ^
                         header->checksumComponents[1] ^
                         header->checksumComponents[2] ^
                         header->checksumComponents[3] ^
                         header->unkInt ^
                         header->flag ^
                         header->referenceNumber;
        if (l_checksum == 0)
            l_checksum = 1;
        header->checksum = l_checksum;
    }
    return p_link;
}

void *ZSysMem::NewNoChecksum(u32 allocSize)
{
    if (!unkByte1)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234);
        g_pSysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 234);
        DebugBreak();
    }
    
    SMallocBlockHeader *l_header = NULL;
    u32 l_flag = allocSize;
    if (!unkByte3)
    {
        l_header = reinterpret_cast<SMallocBlockHeader *>(malloc(allocSize + 12));
    }
    else
    {
        l_header = reinterpret_cast<SMallocBlockHeader *>(m_zMalloc.Alloc(allocSize + 12));
        l_flag |= 0x40000000;
    }
    
    if (!l_header)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 244);
        g_pSysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 244);
        DebugBreak();
    }
    
    l_header->checksumComponents[0] = 1; // refCount
    l_header->checksumComponents[1] = l_flag;
    l_header->checksumComponents[2] = 0; // referenceNumber
    
    m_nrAllocs++;
    m_memAllocated += allocSize + 12;
    if (m_memAllocated > unkInt13)
        unkInt13 = m_memAllocated;
        
    return reinterpret_cast<void *>(reinterpret_cast<char *>(l_header) + 12);
}

void *ZSysMem::New(u32 allocSize)
{
    if (!unkByte1)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 254);
        g_pSysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 254);
        DebugBreak();
    }
    
    SMallocBlockHeader *l_header = NULL;
    u32 l_flag = allocSize;
    if (!unkByte3)
    {
        l_header = reinterpret_cast<SMallocBlockHeader *>(malloc(allocSize + 36));
        l_flag |= 0x80000000;
    }
    else
    {
        l_header = reinterpret_cast<SMallocBlockHeader *>(m_zMalloc.Alloc(allocSize + 36));
        l_flag |= 0xC0000000;
    }
    
    if (!l_header)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 264);
        g_pSysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 264);
        DebugBreak();
    }
    
    // Link to doubly-linked list
    l_header->checksumComponents[1] = unkInt8; // next = current tail
    l_header->checksumComponents[0] = 0;       // prev = NULL
    l_header->checksumComponents[2] = 0;
    l_header->checksumComponents[3] = 0;
    
    if (unkInt8)
    {
        SMallocBlockHeader *l_tail = reinterpret_cast<SMallocBlockHeader *>(unkInt8);
        l_tail->checksumComponents[0] = reinterpret_cast<u32>(l_header); // tail->prev = newBlock
        
        // Recalculate tail checksum since its prev pointer changed
        if (l_tail->flag & 0x80000000)
        {
            u32 l_chk = l_tail->checksumComponents[0] ^ l_tail->checksumComponents[1] ^
                        l_tail->checksumComponents[2] ^ l_tail->checksumComponents[3] ^
                        l_tail->unkInt ^ l_tail->flag ^ l_tail->referenceNumber;
            l_tail->checksum = l_chk ? l_chk : 1;
        }
    }
    else
    {
        unkInt7 = reinterpret_cast<u32>(l_header); // head = newBlock
    }
    
    unkInt8 = reinterpret_cast<u32>(l_header); // tail = newBlock
    
    // Fill user memory with 0xED
    char *l_userData = reinterpret_cast<char *>(l_header) + 32;
    memset(l_userData, 0xED, allocSize);
    
    // Write sentinel
    *reinterpret_cast<u32 *>(l_userData + allocSize) = 0x87654321;
    
    l_header->unkInt = 1; // refCount
    l_header->flag = l_flag;
    l_header->referenceNumber = 0;
    
    m_nrAllocsCheck++;
    m_memAllocated += allocSize + 32;
    if (m_memAllocated > unkInt13)
        unkInt13 = m_memAllocated;
        
    // Calculate new block checksum
    if (l_flag & 0x80000000)
    {
        u32 l_chk = l_header->checksumComponents[0] ^ l_header->checksumComponents[1] ^
                    l_header->checksumComponents[2] ^ l_header->checksumComponents[3] ^
                    l_header->unkInt ^ l_header->flag ^ l_header->referenceNumber;
        l_header->checksum = l_chk ? l_chk : 1;
    }
    
    return reinterpret_cast<void *>(l_userData);
}

void *ZSysMem::NewWithRef(u32 allocSize)
{
    void *l_mem = New(allocSize);
    if (l_mem)
    {
        NewRef(l_mem);
    }
    return l_mem;
}

bool ZSysMem::Delete(void *p_mem)
{
    if (!p_mem)
    {
        g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysMemWintel.cpp", 307);
        g_pSysCom->LogMessage("Tried to Delete 0 Pointer");
        return false;
    }
    
    // Retrieve flag first to check checksum status
    // Flag is at p_mem - 8
    u32 l_flag = *reinterpret_cast<u32 *>(reinterpret_cast<char *>(p_mem) - 8);
    
    if (l_flag & 0x80000000)
    {
        SMallocBlockHeader *l_header = reinterpret_cast<SMallocBlockHeader *>(reinterpret_cast<char *>(p_mem) - 32);
        AllocCheck(l_header);
        
        l_header->unkInt--; // decrement refcount
        if (l_header->unkInt <= 0)
        {
            // Unlink block
            SMallocBlockHeader *l_prev = reinterpret_cast<SMallocBlockHeader *>(l_header->checksumComponents[0]);
            SMallocBlockHeader *l_next = reinterpret_cast<SMallocBlockHeader *>(l_header->checksumComponents[1]);
            
            if (l_next)
            {
                l_next->checksumComponents[0] = reinterpret_cast<u32>(l_prev);
                if (l_next->flag & 0x80000000)
                {
                    u32 l_chk = l_next->checksumComponents[0] ^ l_next->checksumComponents[1] ^
                                l_next->checksumComponents[2] ^ l_next->checksumComponents[3] ^
                                l_next->unkInt ^ l_next->flag ^ l_next->referenceNumber;
                    l_next->checksum = l_chk ? l_chk : 1;
                }
            }
            else
            {
                unkInt8 = reinterpret_cast<u32>(l_prev); // tail = prev
            }
            
            if (l_prev)
            {
                l_prev->checksumComponents[1] = reinterpret_cast<u32>(l_next);
                if (l_prev->flag & 0x80000000)
                {
                    u32 l_chk = l_prev->checksumComponents[0] ^ l_prev->checksumComponents[1] ^
                                l_prev->checksumComponents[2] ^ l_prev->checksumComponents[3] ^
                                l_prev->unkInt ^ l_prev->flag ^ l_prev->referenceNumber;
                    l_prev->checksum = l_chk ? l_chk : 1;
                }
            }
            else
            {
                unkInt7 = reinterpret_cast<u32>(l_next); // head = next
            }
            
            if (l_header->referenceNumber)
            {
                m_allocTab->FreeRef(l_header->referenceNumber);
                m_refArray[l_header->referenceNumber & m_allocTab->m_maxRefIndex] = 0;
            }
            
            u32 l_size = l_flag & 0x1FFFFFFF;
            // Fill user memory with 0xD5
            memset(p_mem, 0xD5, l_size);
            
            m_nrAllocsCheck--;
            m_memAllocated -= (l_size + 32);
            
            if (l_flag & 0x40000000)
            {
                m_zMalloc.Free(l_header);
            }
            else
            {
                free(l_header);
            }
        }
        else
        {
            // Recalculate checksum since refcount changed
            u32 l_chk = l_header->checksumComponents[0] ^ l_header->checksumComponents[1] ^
                        l_header->checksumComponents[2] ^ l_header->checksumComponents[3] ^
                        l_header->unkInt ^ l_header->flag ^ l_header->referenceNumber;
            l_header->checksum = l_chk ? l_chk : 1;
        }
    }
    else
    {
        // Non-checksummed block (12-byte header)
        // Header structure is at p_mem - 12
        u32 *l_pRefCount = reinterpret_cast<u32 *>(reinterpret_cast<char *>(p_mem) - 12);
        u32 *l_pFlag = reinterpret_cast<u32 *>(reinterpret_cast<char *>(p_mem) - 8);
        u32 *l_pRefNum = reinterpret_cast<u32 *>(reinterpret_cast<char *>(p_mem) - 4);
        
        (*l_pRefCount)--;
        if (*l_pRefCount <= 0)
        {
            if (*l_pRefNum)
            {
                m_allocTab->FreeRef(*l_pRefNum);
                m_refArray[*l_pRefNum & m_allocTab->m_maxRefIndex] = 0;
            }
            
            u32 l_size = *l_pFlag & 0x1FFFFFFF;
            m_nrAllocs--;
            m_memAllocated -= (l_size + 12);
            
            void *l_header = reinterpret_cast<void *>(l_pRefCount);
            if (*l_pFlag & 0x40000000)
            {
                m_zMalloc.Free(l_header);
            }
            else
            {
                free(l_header);
            }
        }
    }
    return true;
}

u32 ZSysMem::UnkFunc10(const void *param1, i32 param2, i32 param3)
{
    return 0;
}

u32 ZSysMem::UnkFunc11(const void **param)
{
    return 0;
}

u32 ZSysMem::UnkFunc12(u32 param)
{
    return 0;
}

char *ZSysMem::UnkFunc13(u32 param)
{
    return 0;
}

i32 ZSysMem::UnkFunc14(i32 param1, i32 param2)
{
    return 0;
}

i32 ZSysMem::UnkFunc15(i32 param)
{
    return 0;
}

i32 ZSysMem::UnkFunc16(i32 param)
{
    return 0;
}

i32 ZSysMem::UnkFunc17()
{
    return 0;
}

i32 ZSysMem::UnkFunc18(char *param)
{
    return 0;
}

i32 ZSysMem::UnkFunc19(u32 *param1, i32 param2, i32 param3, i32 param4)
{
    return 0;
}

void ZSysMem::UnkFunc20(i32 param)
{
}

u32 ZSysMem::FindEqu(i32 value, bool flag)
{
    return 0;
}

void ZSysMem::AddEqu(u32 refNum, i32 refValue, bool flag)
{
}

i32 ZSysMem::UnkFunc23(u32 param)
{
    return 0;
}

u32 *ZSysMem::UnkFunc24(u32 *param1, u32 *param2)
{
    return 0;
}