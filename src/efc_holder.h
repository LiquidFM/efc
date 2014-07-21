/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFC_HOLDER_H_
#define EFC_HOLDER_H_

#include <cstddef>
#include <platform/utils.h>
#include <efc/Atomics>
#include <brolly/assert.h>


namespace EFC {

template <typename T>
class Holder
{
public:
    typedef Holder<T> This;

public:
    inline Holder();
    inline explicit Holder(T *data);

    inline Holder(const This &other);
    inline This &operator=(const This &other);

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
    inline Holder(This &&other);
    inline This &operator=(This &&other);
#endif

    ~Holder();

    inline static Holder<T> fromRawData(T *data);

    inline bool operator<(const This &other) const;
    inline bool operator==(const This &other) const;
    inline bool operator!=(const This &other) const;
    inline bool operator==(const T *data) const;
    inline bool operator!=(const T *data) const;

    inline const T *operator->() const;
    inline T *operator->();

    void reset(T *data = NULL);
    inline This &operator=(T *data);

    bool isValid() const;
    bool isUnique() const;

    template <typename R> inline R *as() const;
    template <typename R> inline const R *as_const() const;

public:
    class Data
    {
        PLATFORM_MAKE_NONCOPYABLE(Data)
        PLATFORM_MAKE_NONMOVEABLE(Data)

    protected:
        inline Data()
        {
            ref = EFC_ATOMIC_INIT(0);
        }

        virtual ~Data()
        {}

        static inline void destroy(T *data) { data->~T(); }
        virtual void deallocate() { delete this; }

    private:
        friend class Holder<T>;
        atomic_t ref;
    };

private:
    inline explicit Holder(Data *data);

private:
    Data *m_data;
};


template <typename T>
Holder<T>::Holder() :
    m_data(NULL)
{}

template <typename T>
Holder<T>::Holder(T *data) :
    m_data(data)
{
    if (m_data)
        m_data->ref = EFC_ATOMIC_INIT(1);
}

template <typename T>
Holder<T>::Holder(const This &other) :
    m_data(other.m_data)
{
    if (m_data)
        atomic_inc(&m_data->ref);
}

template <typename T>
typename Holder<T>::This &Holder<T>::operator=(const This &other)
{
    if (&m_data == &other.m_data)
        return *this;

    if (m_data && atomic_dec_and_test(&m_data->ref))
        m_data->deallocate();

    if (m_data = other.m_data)
        atomic_inc(&m_data->ref);

    return *this;
}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
template <typename T>
Holder<T>::Holder(This &&other) :
    m_data(other.m_data)
{
    other.m_data = NULL;
}

template <typename T>
typename Holder<T>::This &Holder<T>::operator=(This &&other)
{
    if (&m_data == &other.m_data)
        return *this;

    if (m_data && atomic_dec_and_test(&m_data->ref))
        m_data->deallocate();

    m_data = other.m_data;
    other.m_data = NULL;

    return *this;
}
#endif

template <typename T>
Holder<T>::~Holder()
{
    if (m_data && atomic_dec_and_test(&m_data->ref))
        m_data->deallocate();
}

template <typename T>
Holder<T> Holder<T>::fromRawData(T *data)
{
    return Holder<T>(static_cast<Data *>(data));
}

template <typename T>
bool Holder<T>::operator<(const This &other) const
{
    return m_data < other.m_data;
}

template <typename T>
bool Holder<T>::operator==(const This &other) const
{
    return m_data == other.m_data;
}

template <typename T>
bool Holder<T>::operator!=(const This &other) const
{
    return !operator==(other);
}

template <typename T>
bool Holder<T>::operator==(const T *data) const
{
    return m_data == static_cast<const Data *>(data);
}

template <typename T>
bool Holder<T>::operator!=(const T *data) const
{
    return !operator==(data);
}

template <typename T>
inline const T *Holder<T>::operator->() const
{
    return as_const<T>();
}

template <typename T>
inline T *Holder<T>::operator->()
{
    return as<T>();
}

template <typename T>
void Holder<T>::reset(T *data)
{
    if (m_data && atomic_dec_and_test(&m_data->ref))
        m_data->deallocate();

    if (m_data = data)
        m_data->ref = EFC_ATOMIC_INIT(1);
}

template <typename T>
typename Holder<T>::This &Holder<T>::operator=(T *data)
{
    reset(data);
    return *this;
}

template <typename T>
bool Holder<T>::isValid() const
{
    return m_data;
}

template <typename T>
bool Holder<T>::isUnique() const
{
    return !m_data || atomic_read(&m_data->ref) == 1;
}

template <typename T> template <typename R> inline
R *Holder<T>::as() const
{
    return static_cast<R *>(m_data);
}

template <typename T> template <typename R> inline
const R *Holder<T>::as_const() const
{
    return static_cast<const R *>(m_data);
}

template <typename T>
Holder<T>::Holder(Data *data) :
    m_data(data)
{
    if (m_data)
    {
        ASSERT(m_data->ref.counter > 0);
        atomic_inc(&m_data->ref);
    }
}

}

#endif /* EFC_HOLDER_H_ */
