/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
// 2011, 2012 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef EFC_MAP_H_
#define EFC_MAP_H_

#include <map>
#include <cstddef>
#include <platform/utils.h>
#include <efc/type_traits>


namespace EFC {

template <typename Key, typename Value>
using Map = ::std::map<Key, Value>;

///**
//*  @brief A standard container made up of (key,value) pairs, which can be
//*  retrieved based on a key, in logarithmic time.
//*
//*  @ingroup associative_containers
//*
//*  Meets the requirements of a <a href="tables.html#65">container</a>, a
//*  <a href="tables.html#66">reversible container</a>, and an
//*  <a href="tables.html#69">associative container</a> (using unique keys).
//*  For a @c map<Key,T> the key_type is Key, the mapped_type is T, and the
//*  value_type is std::pair<const Key,T>.
//*
//*  Maps support bidirectional iterators.
//*
//*  The private tree data is declared exactly the same way for map and
//*  multimap; the distinction is made entirely in how the tree functions are
//*  called (*_unique versus *_equal, same as the standard).
//*/
//template <typename Key, typename T, typename Compare = std::less<Key>,
//typename Allocator = std::allocator<std::pair<const Key, T> > >
//class Map
//{
//public:
//    typedef Key                     key_type;
//    typedef T                       mapped_type;
//    typedef std::pair<const Key, T> value_type;
//    typedef Compare                 key_compare;
//    typedef Allocator               allocator_type;
//    typedef std::size_t             size_type;
//    typedef std::ptrdiff_t          difference_type;
//    typedef T *                     pointer;
//    typedef const T *               const_pointer;
//    typedef T &                     reference;
//    typedef const T &               const_reference;
//
//private: /* Because of error: redeclared with different access */
//    struct NodeBase;
//
//public:
//    struct const_iterator;
//
//    /**
//     *  @brief A Map::iterator.
//     *
//     *  All the functions are op overloads.
//     */
//    struct iterator
//    {
//        typedef std::bidirectional_iterator_tag iterator_category;
//        typedef std::ptrdiff_t                  difference_type;
//        typedef T                               value_type;
//        typedef T *                             pointer;
//        typedef T &                             reference;
//
//        iterator();
//        explicit iterator(NodeBase *x);
//
//        reference operator*() const;
//        pointer operator->() const;
//        iterator &operator++();
//        iterator operator++(int);
//        iterator &operator--();
//        iterator operator--(int);
//        bool operator==(const iterator &x) const;
//        bool operator!=(const iterator &x) const;
//        bool operator==(const const_iterator &x) const;
//        bool operator!=(const const_iterator &x) const;
//
//        NodeBase *node;
//    };
//
//    /**
//     *  @brief A Map::const_iterator.
//     *
//     *  All the functions are op overloads.
//     */
//    struct const_iterator
//    {
//        typedef std::bidirectional_iterator_tag iterator_category;
//        typedef std::ptrdiff_t                  difference_type;
//        typedef T                               value_type;
//        typedef const T *                       pointer;
//        typedef const T &                       reference;
//
//        const_iterator();
//        explicit const_iterator(const NodeBase *x);
//        const_iterator(const iterator &x);
//
//        reference operator*() const;
//        pointer operator->() const;
//        const_iterator &operator++();
//        const_iterator operator++(int);
//        const_iterator &operator--();
//        const_iterator operator--(int);
//        bool operator==(const const_iterator &x) const;
//        bool operator!=(const const_iterator &x) const;
//
//        const NodeBase *node;
//    };
//
//    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
//    typedef std::reverse_iterator<iterator>        reverse_iterator;
//
//    class value_compare : public std::binary_function<value_type, value_type, bool>
//    {
//        friend class Map<Key, T, Compare, Allocator>;
//
//    public:
//        bool operator()(const value_type &x, const value_type &y) const
//        {
//            return comp(x.first, y.first);
//        }
//
//    protected:
//        Compare comp;
//
//        value_compare(Compare c) :
//            comp(c)
//        {}
//    };
//
//public:
//    // [23.3.1.1] construct/copy/destroy
//    // (get_allocator() is normally listed in this section, but seems to have
//    // been accidentally omitted in the printed standard)
//    /**
//    *  @brief  Default constructor creates no elements.
//    */
//    Map();
//
//    /**
//    *  @brief  Creates a %map with no elements.
//    *  @param  comp  A comparison object.
//    *  @param  a  An allocator object.
//    */
//    explicit Map(const Compare &comp, const allocator_type &a = allocator_type());
//
//    /**
//    *  @brief  %Map copy constructor.
//    *  @param  x  A %map of identical element and allocator types.
//    *
//    *  The newly-created %map uses a copy of the allocation object
//    *  used by @a x.
//    */
//    Map(const Map &x);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    /**
//    *  @brief  %Map move constructor.
//    *  @param  x  A %map of identical element and allocator types.
//    *
//    *  The newly-created %map contains the exact contents of @a x.
//    *  The contents of @a x are a valid, but unspecified %map.
//    */
//    Map(Map &&x) NOEXCEPT;
//
//    /**
//    *  @brief  Builds a %map from an std::initializer_list.
//    *  @param  l  An std::initializer_list.
//    *  @param  comp  A comparison object.
//    *  @param  a  An allocator object.
//    *
//    *  Create a %map consisting of copies of the elements in the
//    *  std::initializer_list @a l.
//    *  This is linear in N if the range is already sorted, and NlogN
//    *  otherwise (where N is @a l.size()).
//    */
//    Map(std::initializer_list<value_type> l, const Compare &comp = Compare(), const allocator_type &a = allocator_type());
//#endif
//
//    /**
//    *  @brief  Builds a %map from a range.
//    *  @param  first  An input iterator.
//    *  @param  last  An input iterator.
//    *
//    *  Create a %map consisting of copies of the elements from
//    *  [first,last).  This is linear in N if the range is
//    *  already sorted, and NlogN otherwise (where N is
//    *  distance(first,last)).
//    */
//    template<typename InputIterator>
//    Map(InputIterator first, InputIterator last);
//
//    /**
//    *  @brief  Builds a %map from a range.
//    *  @param  first  An input iterator.
//    *  @param  last  An input iterator.
//    *  @param  comp  A comparison functor.
//    *  @param  a  An allocator object.
//    *
//    *  Create a %map consisting of copies of the elements from
//    *  [first,last).  This is linear in N if the range is
//    *  already sorted, and NlogN otherwise (where N is
//    *  distance(first,last)).
//    */
//    template<typename InputIterator>
//    Map(InputIterator first, InputIterator last, const Compare &comp, const allocator_type &a = allocator_type());
//
//    /**
//    *  The dtor only erases the elements, and note that if the elements
//    *  themselves are pointers, the pointed-to memory is not touched in any
//    *  way.  Managing the pointer is the user's responsibility.
//    */
//    ~Map();
//
//    /**
//    *  @brief  %Map assignment operator.
//    *  @param  x  A %map of identical element and allocator types.
//    *
//    *  All the elements of @a x are copied, but unlike the copy
//    *  constructor, the allocator object is not copied.
//    */
//    Map &operator=(const Map &x);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    /**
//    *  @brief  %Map move assignment operator.
//    *  @param  x  A %map of identical element and allocator types.
//    *
//    *  The contents of @a x are moved into this map (without copying).
//    *  @a x is a valid, but unspecified %map.
//    */
//    Map &operator=(Map &&x);
//
//    /**
//    *  @brief  %Map list assignment operator.
//    *  @param  l  An std::initializer_list.
//    *
//    *  This function fills a %map with copies of the elements in the
//    *  initializer list @a l.
//    *
//    *  Note that the assignment completely changes the %map and
//    *  that the resulting %map's size is the same as the number
//    *  of elements assigned.  Old data may be lost.
//    */
//    Map &operator=(std::initializer_list<value_type> l);
//#endif
//
//    // iterators
//    /**
//    *  Returns a read/write iterator that points to the first pair in the
//    *  %map.
//    *  Iteration is done in ascending order according to the keys.
//    */
//    iterator begin() NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) iterator that points to the first pair
//    *  in the %map.  Iteration is done in ascending order according to the
//    *  keys.
//    */
//    const_iterator begin() const NOEXCEPT;
//
//    /**
//    *  Returns a read/write iterator that points one past the last
//    *  pair in the %map.  Iteration is done in ascending order
//    *  according to the keys.
//    */
//    iterator end() NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) iterator that points one past the last
//    *  pair in the %map.  Iteration is done in ascending order according to
//    *  the keys.
//    */
//    const_iterator end() const NOEXCEPT;
//
//    /**
//    *  Returns a read/write reverse iterator that points to the last pair in
//    *  the %map.  Iteration is done in descending order according to the
//    *  keys.
//    */
//    reverse_iterator rbegin() NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) reverse iterator that points to the
//    *  last pair in the %map.  Iteration is done in descending order
//    *  according to the keys.
//    */
//    const_reverse_iterator
//    rbegin() const NOEXCEPT;
//
//    /**
//    *  Returns a read/write reverse iterator that points to one before the
//    *  first pair in the %map.  Iteration is done in descending order
//    *  according to the keys.
//    */
//    reverse_iterator rend() NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) reverse iterator that points to one
//    *  before the first pair in the %map.  Iteration is done in descending
//    *  order according to the keys.
//    */
//    const_reverse_iterator rend() const NOEXCEPT;
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    /**
//    *  Returns a read-only (constant) iterator that points to the first pair
//    *  in the %map.  Iteration is done in ascending order according to the
//    *  keys.
//    */
//    const_iterator cbegin() const NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) iterator that points one past the last
//    *  pair in the %map.  Iteration is done in ascending order according to
//    *  the keys.
//    */
//    const_iterator cend() const NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) reverse iterator that points to the
//    *  last pair in the %map.  Iteration is done in descending order
//    *  according to the keys.
//    */
//    const_reverse_iterator crbegin() const NOEXCEPT;
//
//    /**
//    *  Returns a read-only (constant) reverse iterator that points to one
//    *  before the first pair in the %map.  Iteration is done in descending
//    *  order according to the keys.
//    */
//    const_reverse_iterator crend() const NOEXCEPT;
//#endif
//
//    // capacity
//    /** Returns true if the %map is empty.  (Thus begin() would equal
//    *  end().)
//    */
//    bool empty() const NOEXCEPT;
//
//    /** Returns the size of the %map.  */
//    size_type size() const NOEXCEPT;
//
//    /** Returns the maximum size of the %map.  */
//    size_type max_size() const NOEXCEPT;
//
//    // [23.3.1.2] element access
//    /**
//    *  @brief  Subscript ( @c [] ) access to %map data.
//    *  @param  k  The key for which data should be retrieved.
//    *  @return  A reference to the data of the (key,data) %pair.
//    *
//    *  Allows for easy lookup with the subscript ( @c [] )
//    *  operator.  Returns data associated with the key specified in
//    *  subscript.  If the key does not exist, a pair with that key
//    *  is created using default values, which is then returned.
//    *
//    *  Lookup requires logarithmic time.
//    */
//    mapped_type &operator[](const key_type &k);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    mapped_type &operator[](key_type &&k);
//#endif
//
//    // GLIBCXX_RESOLVE_LIB_DEFECTS
//    // DR 464. Suggestion for new member functions in standard containers.
//    /**
//    *  @brief  Access to %map data.
//    *  @param  k  The key for which data should be retrieved.
//    *  @return  A reference to the data whose key is equivalent to @a k, if
//    *           such a data is present in the %map.
//    *  @throw  std::out_of_range  If no such data is present.
//    */
//    mapped_type &at(const key_type &k);
//    const mapped_type &at(const key_type &k) const;
//
//    // modifiers
//    /**
//    *  @brief Attempts to insert a std::pair into the %map.
//    *
//    *  @param x Pair to be inserted (see std::make_pair for easy
//    *         creation of pairs).
//    *
//    *  @return  A pair, of which the first element is an iterator that
//    *           points to the possibly inserted pair, and the second is
//    *           a bool that is true if the pair was actually inserted.
//    *
//    *  This function attempts to insert a (key, value) %pair into the %map.
//    *  A %map relies on unique keys and thus a %pair is only inserted if its
//    *  first element (the key) is not already present in the %map.
//    *
//    *  Insertion requires logarithmic time.
//    */
//    std::pair<iterator, bool> insert(const value_type &x);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    template<typename Pair, typename = typename std::enable_if<std::is_constructible<value_type, Pair&&>::value>::type>
//    std::pair<iterator, bool> insert(_Pair &&x);
//
//    /**
//    *  @brief Attempts to insert a list of std::pairs into the %map.
//    *  @param  list  A std::std::initializer_list<value_type> of pairs to be
//    *                  inserted.
//    *
//    *  Complexity similar to that of the range constructor.
//    */
//    void insert(std::initializer_list<value_type> list);
//#endif
//
//    /**
//    *  @brief Attempts to insert a std::pair into the %map.
//    *  @param  position  An iterator that serves as a hint as to where the
//    *                    pair should be inserted.
//    *  @param  x  Pair to be inserted (see std::make_pair for easy creation
//    *               of pairs).
//    *  @return An iterator that points to the element with key of
//    *           @a x (may or may not be the %pair passed in).
//    *
//
//    *  This function is not concerned about whether the insertion
//    *  took place, and thus does not return a boolean like the
//    *  single-argument insert() does.  Note that the first
//    *  parameter is only a hint and can potentially improve the
//    *  performance of the insertion process.  A bad hint would
//    *  cause no gains in efficiency.
//    *
//    *  See
//    *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt07ch17.html
//    *  for more on @a hinting.
//    *
//    *  Insertion requires logarithmic time (if the hint is not taken).
//    */
//    iterator
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    insert(const_iterator position, const value_type &x);
//#else
//    insert(iterator position, const value_type &x);
//#endif
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    template<typename Pair, typename = typename std::enable_if<std::is_constructible<value_type, Pair&&>::value>::type>
//    iterator insert(const_iterator position, Pair &&x);
//#endif
//
//    /**
//    *  @brief Template function that attempts to insert a range of elements.
//    *  @param  first  Iterator pointing to the start of the range to be
//    *                   inserted.
//    *  @param  last  Iterator pointing to the end of the range.
//    *
//    *  Complexity similar to that of the range constructor.
//    */
//    template<typename InputIterator>
//    void insert(_InputIterator first, InputIterator last);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    // GLIBCXX_RESOLVE_LIB_DEFECTS
//    // DR 130. Associative erase should return an iterator.
//    /**
//    *  @brief Erases an element from a %map.
//    *  @param  position  An iterator pointing to the element to be erased.
//    *  @return An iterator pointing to the element immediately following
//    *          @a position prior to the element being erased. If no such
//    *          element exists, end() is returned.
//    *
//    *  This function erases an element, pointed to by the given
//    *  iterator, from a %map.  Note that this function only erases
//    *  the element, and that if the element is itself a pointer,
//    *  the pointed-to memory is not touched in any way.  Managing
//    *  the pointer is the user's responsibility.
//    */
//    iterator erase(const_iterator position);
//
//    // LWG 2059.
//    iterator erase(iterator position);
//#else
//    /**
//    *  @brief Erases an element from a %map.
//    *  @param  position  An iterator pointing to the element to be erased.
//    *
//    *  This function erases an element, pointed to by the given
//    *  iterator, from a %map.  Note that this function only erases
//    *  the element, and that if the element is itself a pointer,
//    *  the pointed-to memory is not touched in any way.  Managing
//    *  the pointer is the user's responsibility.
//    */
//    void erase(iterator position);
//#endif
//
//    /**
//    *  @brief Erases elements according to the provided key.
//    *  @param  x  Key of element to be erased.
//    *  @return  The number of elements erased.
//    *
//    *  This function erases all the elements located by the given key from
//    *  a %map.
//    *  Note that this function only erases the element, and that if
//    *  the element is itself a pointer, the pointed-to memory is not touched
//    *  in any way.  Managing the pointer is the user's responsibility.
//    */
//    size_type erase(const key_type &x);
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//    // GLIBCXX_RESOLVE_LIB_DEFECTS
//    // DR 130. Associative erase should return an iterator.
//    /**
//    *  @brief Erases a [first,last) range of elements from a %map.
//    *  @param  first  Iterator pointing to the start of the range to be
//    *                   erased.
//    *  @param last Iterator pointing to the end of the range to
//    *                be erased.
//    *  @return The iterator @a last.
//    *
//    *  This function erases a sequence of elements from a %map.
//    *  Note that this function only erases the element, and that if
//    *  the element is itself a pointer, the pointed-to memory is not touched
//    *  in any way.  Managing the pointer is the user's responsibility.
//    */
//    iterator erase(const_iterator first, const_iterator last);
//#else
//    /**
//    *  @brief Erases a [first,last) range of elements from a %map.
//    *  @param  first  Iterator pointing to the start of the range to be
//    *                   erased.
//    *  @param last Iterator pointing to the end of the range to
//    *                be erased.
//    *
//    *  This function erases a sequence of elements from a %map.
//    *  Note that this function only erases the element, and that if
//    *  the element is itself a pointer, the pointed-to memory is not touched
//    *  in any way.  Managing the pointer is the user's responsibility.
//    */
//    void erase(iterator first, iterator last);
//#endif
//
//    /**
//    *  @brief  Swaps data with another %map.
//    *  @param  x  A %map of the same element and allocator types.
//    *
//    *  This exchanges the elements between two maps in constant
//    *  time.  (It is only swapping a pointer, an integer, and an
//    *  instance of the @c Compare type (which itself is often
//    *  stateless and empty), so it should be quite fast.)  Note
//    *  that the global std::swap() function is specialized such
//    *  that std::swap(m1,m2) will feed to this function.
//    */
//    void swap(Map &x);
//
//    /**
//    *  Erases all elements in a %map.  Note that this function only
//    *  erases the elements, and that if the elements themselves are
//    *  pointers, the pointed-to memory is not touched in any way.
//    *  Managing the pointer is the user's responsibility.
//    */
//    void clear() NOEXCEPT;
//
//    // observers
//    /**
//    *  Returns the key comparison object out of which the %map was
//    *  constructed.
//    */
//    key_compare key_comp() const;
//
//    /**
//    *  Returns a value comparison object, built from the key comparison
//    *  object out of which the %map was constructed.
//    */
//    value_compare value_comp() const;
//
//    // [23.3.1.3] map operations
//    /**
//    *  @brief Tries to locate an element in a %map.
//    *  @param  x  Key of (key, value) %pair to be located.
//    *  @return  Iterator pointing to sought-after element, or end() if not
//    *           found.
//    *
//    *  This function takes a key and tries to locate the element with which
//    *  the key matches.  If successful the function returns an iterator
//    *  pointing to the sought after %pair.  If unsuccessful it returns the
//    *  past-the-end ( @c end() ) iterator.
//    */
//    iterator find(const key_type &x);
//
//    /**
//    *  @brief Tries to locate an element in a %map.
//    *  @param  x  Key of (key, value) %pair to be located.
//    *  @return  Read-only (constant) iterator pointing to sought-after
//    *           element, or end() if not found.
//    *
//    *  This function takes a key and tries to locate the element with which
//    *  the key matches.  If successful the function returns a constant
//    *  iterator pointing to the sought after %pair. If unsuccessful it
//    *  returns the past-the-end ( @c end() ) iterator.
//    */
//    const_iterator find(const key_type &x) const;
//
//    /**
//    *  @brief  Finds the number of elements with given key.
//    *  @param  x  Key of (key, value) pairs to be located.
//    *  @return  Number of elements with specified key.
//    *
//    *  This function only makes sense for multimaps; for map the result will
//    *  either be 0 (not present) or 1 (present).
//    */
//    size_type count(const key_type &x) const;
//
//    /**
//    *  @brief Finds the beginning of a subsequence matching given key.
//    *  @param  x  Key of (key, value) pair to be located.
//    *  @return  Iterator pointing to first element equal to or greater
//    *           than key, or end().
//    *
//    *  This function returns the first element of a subsequence of elements
//    *  that matches the given key.  If unsuccessful it returns an iterator
//    *  pointing to the first element that has a greater value than given key
//    *  or end() if no such element exists.
//    */
//    iterator lower_bound(const key_type &x);
//
//    /**
//    *  @brief Finds the beginning of a subsequence matching given key.
//    *  @param  x  Key of (key, value) pair to be located.
//    *  @return  Read-only (constant) iterator pointing to first element
//    *           equal to or greater than key, or end().
//    *
//    *  This function returns the first element of a subsequence of elements
//    *  that matches the given key.  If unsuccessful it returns an iterator
//    *  pointing to the first element that has a greater value than given key
//    *  or end() if no such element exists.
//    */
//    const_iterator lower_bound(const key_type &x) const;
//
//    /**
//    *  @brief Finds the end of a subsequence matching given key.
//    *  @param  x  Key of (key, value) pair to be located.
//    *  @return Iterator pointing to the first element
//    *          greater than key, or end().
//    */
//    iterator upper_bound(const key_type &x);
//
//    /**
//    *  @brief Finds the end of a subsequence matching given key.
//    *  @param  x  Key of (key, value) pair to be located.
//    *  @return  Read-only (constant) iterator pointing to first iterator
//    *           greater than key, or end().
//    */
//    const_iterator upper_bound(const key_type &x) const;
//
//    /**
//    *  @brief Finds a subsequence matching given key.
//    *  @param  x  Key of (key, value) pairs to be located.
//    *  @return  Pair of iterators that possibly points to the subsequence
//    *           matching given key.
//    *
//    *  This function is equivalent to
//    *  @code
//    *    std::make_pair(c.lower_bound(val),
//    *                   c.upper_bound(val))
//    *  @endcode
//    *  (but is faster than making the calls separately).
//    *
//    *  This function probably only makes sense for multimaps.
//    */
//    std::pair<iterator, iterator> equal_range(const key_type &x);
//
//    /**
//    *  @brief Finds a subsequence matching given key.
//    *  @param  x  Key of (key, value) pairs to be located.
//    *  @return  Pair of read-only (constant) iterators that possibly points
//    *           to the subsequence matching given key.
//    *
//    *  This function is equivalent to
//    *  @code
//    *    std::make_pair(c.lower_bound(val),
//    *                   c.upper_bound(val))
//    *  @endcode
//    *  (but is faster than making the calls separately).
//    *
//    *  This function probably only makes sense for multimaps.
//    */
//    std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
//
//    template<typename K1, typename T1, typename C1, typename A1>
//    friend bool operator==(const Map<_K1, T1, C1, A1>&, const Map<_K1, T1, C1, A1>&);
//
//    template<typename K1, typename T1, typename C1, typename A1>
//    friend bool operator<(const Map<_K1, T1, C1, A1>&, const Map<_K1, T1, C1, A1>&);
//
//private:
//    enum Balance
//    {
//        BALANCED = 0,
//        LEFT_IS_HEAVY = -1,
//        RIGHT_IS_HEAVY = 1
//    };
//
//    enum Visit
//    {
//        NONE_IS_VISITED = 0,
//        LEFT_IS_VISITED = 1,
//        RIGHT_IS_VISITED = 2,
//        ALL_IS_VISITED = 3
//    };
//
//    struct NodeBase;
//    struct Links
//    {
//        NodeBase *left;
//        NodeBase *right;
//        NodeBase *parent;
//    };
//
//    struct NodeBase
//    {
//        NodeBase(NodeBase *parent = NULL)
//        {
//            links.left = NULL;
//            links.right = NULL;
//            links.parent = parent;
//        }
//
//        NodeBase(NodeBase *left, NodeBase *right, NodeBase *parent)
//        {
//            links.left = left;
//            links.right = right;
//            links.parent = parent;
//        }
//
//        Links links;
//    };
//
//    struct Node : public NodeBase
//    {
//        Node() :
//            NodeBase(),
//            visit(NONE_IS_VISITED),
//            balance(BALANCED),
//            key(key),
//            value()
//        {}
//
//        Node(const key_type &key, const value_type &value, NodeBase *parent) :
//            NodeBase(parent),
//            visit(NONE_IS_VISITED),
//            balance(BALANCED),
//            key(key),
//            value(value)
//        {}
//
//        Node(const key_type &key, NodeBase *parent) :
//            NodeBase(parent),
//            visit(NONE_IS_VISITED),
//            balance(BALANCED),
//            key(key),
//            value()
//        {}
//
//        int visit;
//        Balance balance;
//        key_type key;
//        mapped_type value;
//    };
//
//private:
//    static void left_left_rotation(Node *parent, Node *child, Node **root);
//    static void right_left_rotation(Node *parent, Node *child, Node *grandchild, Node **root);
//    static void right_right_rotation(Node *parent, Node *child, Node **root);
//    static void left_right_rotation(Node *parent, Node *child, Node *grandchild, Node **root);
//    static void rebalance_grew(Node *this_node, Node **root);
//    static void rebalance_shrunk(Node *this_node, Node **root);
//
//private:
//    void destroy_subtree(Node **subtree_root);
//    Node **search_routine(const key_type &key) const;
//    Node **search_routine(const key_type &key, Node **out_parent_node) const;
//    value_type &search_or_create_node(const key_type &key);
//    void search_or_create_node(const key_type &key, const value_type &value);
//    void delete_node(const key_type &key);
//
//private:
//    typedef typename Allocator::template rebind<Node>::other Node_alloc_type;
//
//private:
//    Node *m_root;
//    NodeBase m_end;
//    Compare m_compare;
//    Node_alloc_type m_allocator;
//
//#if PLATFORM_COMPILER(GCC)
//private:
//    // concept requirements
//    typedef typename Allocator::value_type Alloc_value_type;
//    glibcxx_class_requires(T, SGIAssignableConcept)
//    glibcxx_class_requires4(Compare, bool, Key, Key, BinaryFunctionConcept)
//    glibcxx_class_requires2(value_type, Alloc_value_type, SameTypeConcept)
//#endif
//};
//
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::Map() :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(),
//    m_allocator()
//{}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::Map(const Compare &comp, const allocator_type &a) :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(comp),
//    m_allocator(Node_alloc_type(a))
//{}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::Map(const Map &x) :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(x.m_compare),
//    m_allocator(x.m_allocator)
//{
//// FIXME: 987987
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::Map(Map &&x) NOEXCEPT :
//    m_root(x.m_root),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(std::move(x.m_compare)),
//    m_allocator(std::move(x.m_allocator))
//{
//    x.m_root = NULL;
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::Map(std::initializer_list<value_type> l, const Compare &comp, const allocator_type &a) :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(comp),
//    m_allocator(Node_alloc_type(a))
//{
////    FIXME: M_t._M_insert_unique(l.begin(), l.end());
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//template<typename InputIterator>
//Map<Key, T, Compare, Allocator>::Map(InputIterator first, InputIterator last) :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(),
//    m_allocator()
//{
////    FIXME: M_t._M_insert_unique(first, last);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//template<typename InputIterator>
//Map<Key, T, Compare, Allocator>::Map(InputIterator first, InputIterator last, const Compare &comp, const allocator_type &a) :
//    m_root(),
//    m_end(&m_end, &m_end, &m_end),
//    m_compare(comp),
//    m_allocator(Node_alloc_type(a))
//{
////    FIXME: M_t._M_insert_unique(first, last);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::~Map()
//{
//
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator> &Map<Key, T, Compare, Allocator>::operator=(const Map &x)
//{
////    FIXME: M_t = x._M_t;
//    return *this;
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator> &Map<Key, T, Compare, Allocator>::operator=(Map &&x)
//{
//    // NB: DR 1204.
//    // NB: DR 675.
//    clear();
//    swap(x);
//    return *this;
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator> &Map<Key, T, Compare, Allocator>::operator=(std::initializer_list<value_type> l)
//{
//    clear();
//    insert(l.begin(), l.end());
//    return *this;
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::begin() NOEXCEPT
//{
//    return iterator(m_root);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::begin() const NOEXCEPT
//{
//    return const_iterator(m_root);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::end() NOEXCEPT
//{
//    return iterator(&m_end);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::end() const NOEXCEPT
//{
//    return const_iterator(&m_end);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::reverse_iterator Map<Key, T, Compare, Allocator>::rbegin() NOEXCEPT
//{
//    return M_t.rbegin();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::rbegin() const NOEXCEPT
//{
//    return M_t.rbegin();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::reverse_iterator Map<Key, T, Compare, Allocator>::rend() NOEXCEPT
//{
//    return M_t.rend();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::rend() const NOEXCEPT
//{
//    return M_t.rend();
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::cbegin() const NOEXCEPT
//{
//    return M_t.begin();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::cend() const NOEXCEPT
//{
//    return M_t.end();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::crbegin() const NOEXCEPT
//{
//    return M_t.rbegin();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_reverse_iterator Map<Key, T, Compare, Allocator>::crend() const NOEXCEPT
//{
//    return M_t.rend();
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//bool Map<Key, T, Compare, Allocator>::empty() const NOEXCEPT
//{
//    return M_t.empty();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::size() const NOEXCEPT
//{
//    return M_t.size();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::max_size() const NOEXCEPT
//{
//    return M_t.max_size();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::mapped_type &Map<Key, T, Compare, Allocator>::operator[](const key_type &k)
//{
//    // concept requirements
//    glibcxx_function_requires(_DefaultConstructibleConcept<mapped_type>)
//
//    iterator i = lower_bound(k);
//    // i->first is greater than or equivalent to k.
//    if (i == end() || key_comp()(k, (*i).first))
//    i = insert(i, value_type(k, mapped_type()));
//    return (*i).second;
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::mapped_type &Map<Key, T, Compare, Allocator>::operator[](key_type &&k)
//{
//    // concept requirements
//    glibcxx_function_requires(_DefaultConstructibleConcept<mapped_type>)
//
//    iterator i = lower_bound(k);
//    // i->first is greater than or equivalent to k.
//    if (i == end() || key_comp()(k, (*i).first))
//    i = insert(i, std::make_pair(std::move(k), mapped_type()));
//    return (*i).second;
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::mapped_type &Map<Key, T, Compare, Allocator>::at(const key_type &k)
//{
//    iterator i = lower_bound(k);
//    if (i == end() || key_comp()(k, (*i).first))
//        throw_out_of_range(N("map::at"));
//    return (*i).second;
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//const Map<Key, T, Compare, Allocator>::mapped_type &Map<Key, T, Compare, Allocator>::at(const key_type &k) const
//{
//    const_iterator i = lower_bound(k);
//    if (i == end() || key_comp()(k, (*i).first))
//        throw_out_of_range(N("map::at"));
//    return (*i).second;
//}
//
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//std::pair<Map<Key, T, Compare, Allocator>::iterator, bool> Map<Key, T, Compare, Allocator>::insert(const value_type &x)
//{
//    return M_t._M_insert_unique(x);
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//template<typename Pair, typename = typename std::enable_if<std::is_constructible<value_type, Pair&&>::value>::type>
//std::pair<Map<Key, T, Compare, Allocator>::iterator, bool> Map<Key, T, Compare, Allocator>::insert(_Pair &&x)
//{
//    return M_t._M_insert_unique(std::forward<_Pair>(x));
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//void Map<Key, T, Compare, Allocator>::insert(std::initializer_list<value_type> list)
//{
//    insert(list.begin(), list.end());
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//Map<Key, T, Compare, Allocator>::insert(const_iterator position, const value_type &x)
//#else
//Map<Key, T, Compare, Allocator>::insert(iterator position, const value_type &x)
//#endif
//{
//    return M_t._M_insert_unique_(position, x);
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//template<typename Pair, typename = typename std::enable_if<std::is_constructible<value_type, Pair&&>::value>::type>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::insert(const_iterator position, Pair &&x)
//{
//    return M_t._M_insert_unique_(position, std::forward<_Pair>(x));
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//template<typename InputIterator>
//void Map<Key, T, Compare, Allocator>::insert(_InputIterator first, InputIterator last)
//{
//    M_t._M_insert_unique(first, last);
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::erase(const_iterator position)
//{
//    return M_t.erase(position);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::erase(iterator position)
//{
//    return M_t.erase(position);
//}
//#else
//template <typename Key, typename T, typename Compare, typename Allocator>
//void Map<Key, T, Compare, Allocator>::erase(iterator position)
//{
//    M_t.erase(position);
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::erase(const key_type &x)
//{
//    return M_t.erase(x);
//}
//
//#if PLATFORM_COMPILER_SUPPORTS(CXX11)
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::erase(const_iterator first, const_iterator last)
//{
//    return M_t.erase(first, last);
//}
//#else
//template <typename Key, typename T, typename Compare, typename Allocator>
//void Map<Key, T, Compare, Allocator>::erase(iterator first, iterator last)
//{
//    M_t.erase(first, last);
//}
//#endif
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//void Map<Key, T, Compare, Allocator>::swap(Map &x)
//{
//    M_t.swap(x._M_t);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//void Map<Key, T, Compare, Allocator>::clear() NOEXCEPT
//{
//    M_t.clear();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::key_compare Map<Key, T, Compare, Allocator>::key_comp() const
//{
//    return M_t.key_comp();
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::value_compare Map<Key, T, Compare, Allocator>::value_comp() const
//{
//    return value_compare(_M_t.key_comp());
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::find(const key_type &x)
//{
//    return M_t.find(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::find(const key_type &x) const
//{
//    return M_t.find(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::size_type Map<Key, T, Compare, Allocator>::count(const key_type &x) const
//{
//    return M_t.find(x) == M_t.end() ? 0 : 1;
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::lower_bound(const key_type &x)
//{
//    return M_t.lower_bound(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::lower_bound(const key_type &x) const
//{
//    return M_t.lower_bound(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::iterator Map<Key, T, Compare, Allocator>::upper_bound(const key_type &x)
//{
//    return M_t.upper_bound(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//Map<Key, T, Compare, Allocator>::const_iterator Map<Key, T, Compare, Allocator>::upper_bound(const key_type &x) const
//{
//    return M_t.upper_bound(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//std::pair<Map<Key, T, Compare, Allocator>::iterator, Map<Key, T, Compare, Allocator>::iterator> Map<Key, T, Compare, Allocator>::equal_range(const key_type &x)
//{
//    return M_t.equal_range(x);
//}
//
//template <typename Key, typename T, typename Compare, typename Allocator>
//std::pair<Map<Key, T, Compare, Allocator>::const_iterator, Map<Key, T, Compare, Allocator>::const_iterator> Map<Key, T, Compare, Allocator>::equal_range(const key_type &x) const
//{
//    return M_t.equal_range(x);
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
///**
//*  @brief  Map equality comparison.
//*  @param  x  A %map.
//*  @param  y  A %map of the same type as @a x.
//*  @return  True iff the size and elements of the maps are equal.
//*
//*  This is an equivalence relation.  It is linear in the size of the
//*  maps.  Maps are considered equivalent if their sizes are equal,
//*  and if corresponding elements compare equal.
//*/
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator==(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return x._M_t == y._M_t;
//}
//
///**
//*  @brief  Map ordering relation.
//*  @param  x  A %map.
//*  @param  y  A %map of the same type as @a x.
//*  @return  True iff @a x is lexicographically less than @a y.
//*
//*  This is a total ordering relation.  It is linear in the size of the
//*  maps.  The elements must be comparable with @c <.
//*
//*  See std::lexicographical_compare() for how the determination is made.
//*/
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator<(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return x._M_t < y._M_t;
//}
//
///// Based on operator==
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator!=(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return !(x == y);
//}
//
///// Based on operator<
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator>(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return y < x;
//}
//
///// Based on operator<
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator<=(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return !(y < x);
//}
//
///// Based on operator<
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline bool operator>=(const Map<Key, T, Compare, Allocator> &x, const Map<Key, T, Compare, Allocator> &y)
//{
//    return !(x < y);
//}
//
///// See std::map::swap().
//template<typename Key, typename T, typename Compare, typename Allocator>
//inline void swap(Map<Key, T, Compare, Allocator> &x, Map<Key, T, Compare, Allocator> &y)
//{
//    x.swap(y);
//}

}

#endif /* EFC_MAP_H_ */
