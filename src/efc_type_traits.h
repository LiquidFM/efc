/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2010-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#ifndef EFC_TYPE_TRAITS_H_
#define EFC_TYPE_TRAITS_H_

#include <platform/platform.h>


namespace EFC {

class null_type {};
struct empty_type {};

template <int T>
struct int_to_type
{
    enum { value = T };
};

template <typename T>
struct type_to_type
{
    typedef T type;
};

template <bool B>
struct bool_value
{
    enum { value = B };
};


/***************************************************************************************************/
template<typename T>
struct is_void
{
    enum { value = 0 };
    typedef bool_value<false> type;
};

template<>
struct is_void<void>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};


/***************************************************************************************************/
template<typename T>
struct is_integer
{
    enum { value = 0 };
    typedef bool_value<false> type;
};

template<>
struct is_integer<bool>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<char>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<signed char>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<unsigned char>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

#if PLATFORM_USE(WCHAR_T)
template<>
struct is_integer<wchar_t>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};
# endif

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<>
struct is_integer<char16_t>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<char32_t>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};
#endif

template<>
struct is_integer<short>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<unsigned short>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<int>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<unsigned int>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<long>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<unsigned long>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<long long>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_integer<unsigned long long>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};


/***************************************************************************************************/
template<typename T>
struct is_floating
{
    enum { value = 0 };
    typedef bool_value<false> type;
};

template<>
struct is_floating<float>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_floating<double>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};

template<>
struct is_floating<long double>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};


/***************************************************************************************************/
template<typename T>
struct is_pointer
{
enum { value = 0 };
typedef bool_value<false> type;
};

template<typename T>
struct is_pointer<T*>
{
    enum { value = 1 };
    typedef bool_value<true> type;
};


/***************************************************************************************************/
template <typename T, typename U>
class conversion
{
    typedef char one_t;
    typedef struct { char array[2]; } two_t;
    static one_t test(U);
    static two_t test(...);
    static T makeT();

public:
    enum { exists = sizeof(test(makeT())) == sizeof(one_t) };
    enum { is_same_types = 0 };
};

template <typename T>
class conversion<T, T>
{
public:
    enum { exists = 1 };
    enum { is_same_types = 1 };
};


template <typename T>
class has_value_type
{
    typedef char one_t;
    typedef struct { char array[2]; } two_t;
    template <typename U> static one_t check_function(...);
    template <typename U> static two_t check_function(typename U::value_type const volatile *);

public:
    enum { value = sizeof(check_function<T>(0)) == sizeof(two_t) };
};

template <typename T>
class has_size_type
{
    typedef char one_t;
    typedef struct { char array[2]; } two_t;
    template <typename U> static one_t check_function(...);
    template <typename U> static two_t check_function(typename U::size_type const volatile *);

public:
    enum { value = sizeof(check_function<T>(0)) == sizeof(two_t) };
};


/***************************************************************************************************/
template <typename T>
struct remove_qualifiers
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T*>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T*&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T* const &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T* volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<T* const volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T*>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T*&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* const &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* const volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T*>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T*&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* const &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* const volatile &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T*>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T*&>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T* const &>
{
    typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T* const volatile &>
{
    typedef T type;
};


/***************************************************************************************************/
template <typename Derived, typename Base>
struct must_have_base
{
    typedef typename remove_qualifiers<Derived>::type first_t;
    typedef typename remove_qualifiers<Base>::type second_t;

public:
    ~must_have_base() throw()
    {
        void(*p)(first_t*, second_t*) = constraints;
        (void)p;
    }

private:
    static void constraints(first_t *pd, second_t *pb)
    {
        pb = pd;
        (void)pb;
    }
};


/***************************************************************************************************/
template <typename T1, typename T2, bool B>
struct select_first_type_if
{
    typedef T2 type;
};

template <typename T1, typename T2>
struct select_first_type_if<T1, T2, true>
{
    typedef T1 type;
};

}

#endif /* EFC_TYPE_TRAITS_H_ */
