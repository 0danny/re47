#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "common.h"

template <class T>
class DynamicArray
{
public:
    i32 m_maxSize;
    i32 m_currentSize;
    T *m_data;

    inline DynamicArray()
    {
        m_maxSize = 0;
        m_currentSize = 0;
        m_data = 0;
    }

    inline ~DynamicArray()
    {
        Cleanup();
    }

    inline i32 Init(i32 size)
    {
        Cleanup();

        m_data = new T[size];

        if (!m_data)
            printf("ERROR: Cannot allocate array structure");

        m_maxSize = size;
        m_currentSize = 0;
        return 1;
    }

    inline T *Add() // T &data
    {
        if (m_currentSize >= m_maxSize)
            ExpandArray(); // if we don't have enough space, expand the array

        m_currentSize++;

        return &m_data[m_currentSize - 1];
    }

    inline void ExpandArray()
    {
        i32 newSize = m_maxSize * 2;

        T *newData = new T[newSize];

        if (!newData)
            printf("ERROR: Cannot allocate array structure");

        memcpy(newData, m_data, m_maxSize * sizeof(T));

        m_maxSize = newSize;

        delete[] m_data;

        m_data = newData;
    }

    inline void Cleanup()
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = 0;
            m_maxSize = 0;
            m_currentSize = 0;
        }
    }

    inline T &operator[](i32 index)
    {
        return m_data[index];
    }
};

#endif