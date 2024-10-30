#include "common.h"
#include "system/zsysmem.h"

// Global mem alloc overloads
inline void *operator new(size_t size)
{
    if (g_pSysMem)
        return g_pSysMem->New(size);
    else
        return malloc(size);
}

inline void operator delete(void *ptr)
{
    if (g_pSysMem)
    {
        g_pSysMem->Delete(ptr);
        return;
    }

    free(ptr);
}