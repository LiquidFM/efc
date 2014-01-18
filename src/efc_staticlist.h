/** @file the EFC library_staticlist.h
 *  @brief TODO: Put description here.
 *
 * TODO: Put description here.
 *
 * @copyright
 * This file is part of the EFC library.
 * @n@n
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * @n@n
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * @n@n
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * @n@n
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFC_STATICLIST_H_
#define EFC_STATICLIST_H_

#include <efc/List>


namespace EFC {

template <typename T, int Count>
class StaticList
{
public:
    struct const_iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t                  difference_type;
        typedef T                               value_type;
        typedef const T *                       pointer;
        typedef const T &                       reference;

        const_iterator() :
            m_pos(0),
            m_field(NULL)
        {}

        const_iterator(int pos, pointer field) :
            m_pos(pos),
            m_field(field)
        {}

        reference operator*() const { return m_field[m_pos % Count]; }
        pointer operator->() const { return m_field + (m_pos % Count); }
        const_iterator &operator++() { ++m_pos; return *this; }
        const_iterator operator++(int) { ++m_pos; return *this; }
        const_iterator &operator--() { --m_pos; return *this; }
        const_iterator operator--(int) { --m_pos; return *this; }
        bool operator==(const const_iterator &x) const { return m_pos == x.m_pos; }
        bool operator!=(const const_iterator &x) const { return m_pos != x.m_pos; }

    private:
        int m_pos;
        pointer m_field;
    };

public:
    StaticList() :
        m_count(0)
    {}

    bool empty() const { return m_count == 0; }
    const_iterator begin() const { return const_iterator(0, m_elements); }
    const_iterator end() const { return const_iterator(m_count, NULL); }
    void push_back(const T &field) { m_elements[m_count % Count] = field; ++m_count; }

private:
    int m_count;
    T m_elements[Count];
};

}

#endif /* EFC_STATICLIST_H_ */
