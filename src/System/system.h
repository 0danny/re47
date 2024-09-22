#include "zsysmem.h"

inline static void *operator new(size_t size)
{
    // TODO: Add method
    if (g_pSysMem)
        return nullptr; //(*(*g_pSysMem + 44))(g_pSysMem, 39);
    else
        return malloc(size);
}

inline static void operator delete(void *ptr)
{
    // TODO: Add method
    if (g_pSysMem)
    {
        //(*(g_pSysMem->vtable + 13))(g_pSysMem, this);
        return;
    }

    delete ptr;
}