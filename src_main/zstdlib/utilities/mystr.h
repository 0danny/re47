#ifndef MYSTR_H
#define MYSTR_H

#include "common.h"

class MyStr
{
public:
    char *m_pointer;
    char m_buffer[124];

    inline MyStr()
    {
        m_pointer = m_buffer;
        m_buffer[0] = '\0';
    }

    inline MyStr(const char *p_str)
    {
        if (p_str)
            SetString(p_str);
        else
            SetString(g_emptyArray);
    }

    inline MyStr(const MyStr &p_instance)
    {
        if (&p_instance)
            SetString(*p_instance);
        else
            SetString(g_emptyArray);
    }

    inline ~MyStr()
    {
        DeleteString();
    }

    inline void SetString(const char *p_targetStr)
    {
        char *l_string;

        if (strlen(p_targetStr) + 1 <= sizeof(m_buffer))
        {
            // If we have enough space, use the buffer.
            l_string = m_buffer;
        }
        else
        {
            // If we don't, allocate some.
            u32 l_strLen = strlen(p_targetStr) + 1;
            l_string = new char[l_strLen];
        }

        m_pointer = l_string;

        strcpy(m_pointer, p_targetStr);
    }

    inline void DeleteString()
    {
        // If the pointer is not pointing to the buffer, then we allocated extra space, clear it.
        if (m_pointer != m_buffer)
        {
            delete m_pointer;
        }
    }

    inline void ToLower()
    {
        char *l_pointer = m_pointer;

        for (i32 l_index = 0; l_index < strlen(l_pointer); ++l_index)
        {
            char l_curChar = l_pointer[l_index];

            if (l_curChar >= 'A' && l_curChar <= 'Z')
            {
                l_pointer[l_index] = l_curChar + ' '; // Convert to lowercase.
            }
        }
    }

    inline MyStr &operator=(const MyStr &p_instance)
    {
        if (this != &p_instance)
        {
            DeleteString();

            if (&p_instance)
                SetString(*p_instance);
            else
                SetString(g_emptyArray);
        }

        return *this;
    }

    inline bool operator==(const MyStr &p_instance) const
    {
        return strcmp(m_pointer, *p_instance) == 0;
    }

    inline MyStr &operator+=(const MyStr &p_instance)
    {
        if (this != &p_instance && &p_instance)
        {
            if (m_pointer == m_buffer && (strlen(m_pointer) + strlen(*p_instance) + 1) < sizeof(m_buffer))
            {
                strcat(m_pointer, *p_instance);
            }
            else
            {
                char *l_newStr = new char[strlen(m_pointer) + strlen(*p_instance) + 1];

                strcpy(l_newStr, m_pointer);
                strcat(l_newStr, *p_instance);

                DeleteString();
                m_pointer = l_newStr;
            }
        }

        return *this;
    }

    inline friend MyStr operator+(const MyStr &p_instance1, const MyStr &p_instance2)
    {
        if (p_instance1 == p_instance2)
            return MyStr(p_instance1);

        MyStr l_temp(p_instance1);

        l_temp += p_instance2;

        MyStr l_result(l_temp);

        return l_result;
    }

    inline const char *operator*() const
    {
        return m_pointer;
    }
};

#endif