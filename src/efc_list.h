/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2009, 2010, 2011
// Free Software Foundation, Inc.
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

#ifndef EFC_LIST_H_
#define EFC_LIST_H_

#include <list>
#include <cstddef>
#include <platform/utils.h>
#include <efc/type_traits>


namespace EFC {

/**
 *  @brief A standard container with linear time access to elements,
 *  and fixed time insertion/deletion at any point in the sequence.
 *
 *  @ingroup sequences
 *
 *  Meets the requirements of a <a href="tables.html#65">container</a>, a
 *  <a href="tables.html#66">reversible container</a>, and a
 *  <a href="tables.html#67">sequence</a>, including the
 *  <a href="tables.html#68">optional sequence requirements</a> with the
 *  %exception of @c at and @c operator[].
 *
 *  This is a @e doubly @e linked %List.  Traversal up and down the
 *  %List requires linear time, but adding and removing elements (or
 *  @e nodes) is done in constant time, regardless of where the
 *  change takes place.  Unlike std::vector and std::deque,
 *  random-access iterators are not provided, so subscripting ( @c
 *  [] ) access is not allowed.  For algorithms which only need
 *  sequential access, this lack makes no difference.
 *
 *  Also unlike the other standard containers, EFC::List provides
 *  specialized algorithms %unique to linked lists, such as
 *  splicing, sorting, and in-place reversal.
 *
 *  A couple points on memory allocation for List<T>:
 *
 *  First, we never actually allocate a T, we allocate
 *  List::Node's and trust [20.1.5]/4 to DTRT.  This is to ensure
 *  that after elements from %List<X,Alloc1> are spliced into
 *  %List<X,Alloc2>, destroying the memory of the second %List is a
 *  valid operation, i.e., Alloc1 giveth and Alloc2 taketh away.
 *
 *  Second, a %List conceptually represented as
 *  @code
 *    A <---> B <---> C <---> D
 *  @endcode
 *  is actually circular; a link exists between A and D.  The %List
 *  class holds (as its only data member) a private List::iterator
 *  pointing to @e D, not to @e A!  To get to the head of the %List,
 *  we start at the tail and move forward by one.  When this member
 *  iterator's next/previous pointers refer to itself, the %List is
 *  %empty.
 */
template<typename T, typename Allocator = std::allocator<T> >
class List
{
public:
    typedef T               value_type;
    typedef T *             pointer;
    typedef const T *       const_pointer;
    typedef T &             reference;
    typedef const T &       const_reference;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

private: /* Because of error: redeclared with different access */
    struct NodeBase;

public:
    struct const_iterator;

    /**
     *  @brief A List::iterator.
     *
     *  All the functions are op overloads.
     */
    struct iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t                  difference_type;
        typedef T                               value_type;
        typedef T *                             pointer;
        typedef T &                             reference;

        iterator();
        explicit iterator(NodeBase *x);

        reference operator*() const;
        pointer operator->() const;
        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
        bool operator==(const iterator &x) const;
        bool operator!=(const iterator &x) const;
        bool operator==(const const_iterator &x) const;
        bool operator!=(const const_iterator &x) const;

        NodeBase *node;
    };

    /**
     *  @brief A List::const_iterator.
     *
     *  All the functions are op overloads.
     */
    struct const_iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t                  difference_type;
        typedef T                               value_type;
        typedef const T *                       pointer;
        typedef const T &                       reference;

        const_iterator();
        explicit const_iterator(const NodeBase *x);
        const_iterator(const iterator &x);

        reference operator*() const;
        pointer operator->() const;
        const_iterator &operator++();
        const_iterator operator++(int);
        const_iterator &operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator &x) const;
        bool operator!=(const const_iterator &x) const;

        const NodeBase *node;
    };
    
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef std::reverse_iterator<iterator>        reverse_iterator;

public:
    // [23.2.2.1] construct/copy/destroy
    // (assign() and m_allocator are also listed in this section)
    /**
     *  @brief  Default constructor creates no elements.
     */
    List();

    /**
     *  @brief  Creates a %List with no elements.
     *  @param  a  An allocator object argumant.
     */
    explicit List(const Allocator &a);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief  %List move constructor.
     *  @param  x  A %List of identical element and allocator types.
     *
     *  The newly-created %List contains the exact contents of @a x.
     *  The contents of @a x are a valid, but unspecified %List.
     */
    List(List &&x) NOEXCEPT;

    /**
     *  @brief  Creates a %List with default constructed elements.
     *  @param  n  The number of elements to initially create.
     *
     *  This constructor fills the %List with @a n default
     *  constructed elements.
     */
    explicit List(size_type n);

    /**
     *  @brief  Creates a %List with copies of an exemplar element.
     *  @param  n  The number of elements to initially create.
     *  @param  value  An element to copy.
     *  @param  a  An allocator object.
     *
     *  This constructor fills the %List with @a n copies of @a value.
     */
    List(size_type n, const value_type &value, const Allocator &a = Allocator());

    /**
     *  @brief  Builds a %List from an initializer_list
     *  @param  l  An initializer_list of value_type.
     *  @param  a  An allocator object.
     *
     *  Create a %List consisting of copies of the elements in the
     *  initializer_list @a l.  This is linear in l.size().
     */
    List(std::initializer_list<value_type> l, const Allocator &a = Allocator());
#else
    /**
     *  @brief  Creates a %List with copies of an exemplar element.
     *  @param  n  The number of elements to initially create.
     *  @param  value  An element to copy.
     *  @param  a  An allocator object.
     *
     *  This constructor fills the %List with @a n copies of @a value.
     */
    explicit List(size_type n, const value_type &value = value_type(), const Allocator &a = Allocator());
#endif

    /**
     *  @brief  %List copy constructor.
     *  @param  x  A %List of identical element and allocator types.
     *
     *  The newly-created %List uses a copy of the allocation object used
     *  by @a x.
     */
    List(const List &x);

    /**
     *  @brief  Builds a %List from a range.
     *  @param  first  An input iterator.
     *  @param  last  An input iterator.
     *  @param  a  An allocator object.
     *
     *  Create a %List consisting of copies of the elements from
     *  [@a first,@a last).  This is linear in N (where N is
     *  distance(@a first,@a last)).
     */
    template<typename InputIterator>
    List(InputIterator first, InputIterator last, const Allocator &a = Allocator());

    /**
     *  The dtor only erases the elements, and note
     *  that if the elements themselves are pointers, the pointed-to
     *  memory is not touched in any way.  Managing the pointer is
     *  the user's responsibility.
     */
    ~List() NOEXCEPT;

    /**
     *  @brief  %List assignment operator.
     *  @param  x  A %List of identical element and allocator types.
     *
     *  All the elements of @a x are copied, but unlike the copy
     *  constructor, the allocator object is not copied.
     */
    List &operator=(const List &x);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief  %List move assignment operator.
     *  @param  x  A %List of identical element and allocator types.
     *
     *  The contents of @a x are moved into this %List (without copying).
     *  @a x is a valid, but unspecified %List
     */
    List &operator=(List &&x);

    /**
     *  @brief  %List initializer list assignment operator.
     *  @param  l  An initializer_list of value_type.
     *
     *  Replace the contents of the %List with copies of the elements
     *  in the initializer_list @a l.  This is linear in l.size().
     */
    List &operator=(std::initializer_list<value_type> l);
#endif

    /**
     *  @brief  Assigns a given value to a %List.
     *  @param  n  Number of elements to be assigned.
     *  @param  val  Value to be assigned.
     *
     *  This function fills a %List with @a n copies of the given
     *  value.  Note that the assignment completely changes the %List
     *  and that the resulting %List's size is the same as the number
     *  of elements assigned.  Old data may be lost.
     */
    void assign(size_type n, const value_type &val);

    /**
     *  @brief  Assigns a range to a %List.
     *  @param  first  An input iterator.
     *  @param  last   An input iterator.
     *
     *  This function fills a %List with copies of the elements in the
     *  range [@a first,@a last).
     *
     *  Note that the assignment completely changes the %List and
     *  that the resulting %List's size is the same as the number of
     *  elements assigned.  Old data may be lost.
     */
    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief  Assigns an initializer_list to a %List.
     *  @param  l  An initializer_list of value_type.
     *
     *  Replace the contents of the %List with copies of the elements
     *  in the initializer_list @a l.  This is linear in l.size().
     */
    void assign(std::initializer_list<value_type> l);
#endif

    // iterators
    /**
     *  Returns a read/write iterator that points to the first element in the
     *  %List.  Iteration is done in ordinary element order.
     */
    iterator begin() NOEXCEPT;

    /**
     *  Returns a read-only (constant) iterator that points to the
     *  first element in the %List.  Iteration is done in ordinary
     *  element order.
     */
    const_iterator begin() const NOEXCEPT;

    /**
     *  Returns a read/write iterator that points one past the last
     *  element in the %List.  Iteration is done in ordinary element
     *  order.
     */
    iterator end() NOEXCEPT;

    /**
     *  Returns a read-only (constant) iterator that points one past
     *  the last element in the %List.  Iteration is done in ordinary
     *  element order.
     */
    const_iterator end() const NOEXCEPT;

    /**
     *  Returns a read/write reverse iterator that points to the last
     *  element in the %List.  Iteration is done in reverse element
     *  order.
     */
    reverse_iterator rbegin() NOEXCEPT;

    /**
     *  Returns a read-only (constant) reverse iterator that points to
     *  the last element in the %List.  Iteration is done in reverse
     *  element order.
     */
    const_reverse_iterator rbegin() const NOEXCEPT;

    /**
     *  Returns a read/write reverse iterator that points to one
     *  before the first element in the %List.  Iteration is done in
     *  reverse element order.
     */
    reverse_iterator rend() NOEXCEPT;

    /**
     *  Returns a read-only (constant) reverse iterator that points to one
     *  before the first element in the %List.  Iteration is done in reverse
     *  element order.
     */
    const_reverse_iterator rend() const NOEXCEPT;

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  Returns a read-only (constant) iterator that points to the
     *  first element in the %List.  Iteration is done in ordinary
     *  element order.
     */
    const_iterator cbegin() const NOEXCEPT;

    /**
     *  Returns a read-only (constant) iterator that points one past
     *  the last element in the %List.  Iteration is done in ordinary
     *  element order.
     */
    const_iterator cend() const NOEXCEPT;

    /**
     *  Returns a read-only (constant) reverse iterator that points to
     *  the last element in the %List.  Iteration is done in reverse
     *  element order.
     */
    const_reverse_iterator crbegin() const NOEXCEPT;

    /**
     *  Returns a read-only (constant) reverse iterator that points to one
     *  before the first element in the %List.  Iteration is done in reverse
     *  element order.
     */
    const_reverse_iterator crend() const NOEXCEPT;
#endif

    // [23.2.2.2] capacity
    /**
     *  Returns true if the %List is empty.  (Thus begin() would equal
     *  end().)
     */
    bool empty() const NOEXCEPT;

    /**
     *  Returns the number of elements in the %List.
     */
    size_type size() const NOEXCEPT;

    /**
     *  Returns the size() of the largest possible %List.
     */
    size_type max_size() const NOEXCEPT;

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief Resizes the %List to the specified number of elements.
     *  @param new_size Number of elements the %List should contain.
     *
     *  This function will %resize the %List to the specified number
     *  of elements.  If the number is smaller than the %List's
     *  current size the %List is truncated, otherwise default
     *  constructed elements are appended.
     */
    WARN_UNUSED_RETURN bool resize(size_type new_size);

    /**
     *  @brief Resizes the %List to the specified number of elements.
     *  @param new_size Number of elements the %List should contain.
     *  @param x Data with which new elements should be populated.
     *
     *  This function will %resize the %List to the specified number
     *  of elements.  If the number is smaller than the %List's
     *  current size the %List is truncated, otherwise the %List is
     *  extended and new elements are populated with given data.
     */
    WARN_UNUSED_RETURN bool resize(size_type new_size, const value_type &x);
#else
    /**
     *  @brief Resizes the %List to the specified number of elements.
     *  @param new_size Number of elements the %List should contain.
     *  @param x Data with which new elements should be populated.
     *
     *  This function will %resize the %List to the specified number
     *  of elements.  If the number is smaller than the %List's
     *  current size the %List is truncated, otherwise the %List is
     *  extended and new elements are populated with given data.
     */
    WARN_UNUSED_RETURN bool resize(size_type new_size, value_type x = value_type());
#endif

    // element access
    /**
     *  Returns a read/write reference to the data at the first
     *  element of the %List.
     */
    reference front();

    /**
     *  Returns a read-only (constant) reference to the data at the first
     *  element of the %List.
     */
    const_reference front() const;

    /**
     *  Returns a read/write reference to the data at the last element
     *  of the %List.
     */
    reference back();

    /**
     *  Returns a read-only (constant) reference to the data at the last
     *  element of the %List.
     */
    const_reference back() const;

    // [23.2.2.3] modifiers
    /**
     *  @brief  Add data to the front of the %List.
     *  @param  x  Data to be added.
     *  @return  @c TRUE if Node was pushed into list.
     *
     *  This is a typical stack operation.  The function creates an
     *  element at the front of the %List and assigns the given data
     *  to it.  Due to the nature of a %List this operation can be
     *  done in constant time, and does not invalidate iterators and
     *  references.
     */
    WARN_UNUSED_RETURN bool push_front(const value_type &x);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    WARN_UNUSED_RETURN bool push_front(value_type &&x);

    template<typename... Args>
    WARN_UNUSED_RETURN bool emplace_front(Args &&... args);
#endif

    /**
     *  @brief  Removes first element.
     *
     *  This is a typical stack operation.  It shrinks the %List by
     *  one.  Due to the nature of a %List this operation can be done
     *  in constant time, and only invalidates iterators/references to
     *  the element being removed.
     *
     *  Note that no data is returned, and if the first element's data
     *  is needed, it should be retrieved before pop_front() is
     *  called.
     */
    void pop_front();

    /**
     *  @brief  Add data to the end of the %List.
     *  @param  x  Data to be added.
     *
     *  This is a typical stack operation.  The function creates an
     *  element at the end of the %List and assigns the given data to
     *  it.  Due to the nature of a %List this operation can be done
     *  in constant time, and does not invalidate iterators and
     *  references.
     */
    WARN_UNUSED_RETURN bool push_back(const value_type &x);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    WARN_UNUSED_RETURN bool push_back(value_type &&x);

    template<typename... Args>
    WARN_UNUSED_RETURN bool emplace_back(Args &&... args);
#endif

    /**
     *  @brief  Removes last element.
     *
     *  This is a typical stack operation.  It shrinks the %List by
     *  one.  Due to the nature of a %List this operation can be done
     *  in constant time, and only invalidates iterators/references to
     *  the element being removed.
     *
     *  Note that no data is returned, and if the last element's data
     *  is needed, it should be retrieved before pop_back() is called.
     */
    void pop_back();

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief  Constructs object in %List before specified iterator.
     *  @param  position  A const_iterator into the %List.
     *  @param  args  Arguments.
     *  @return  An iterator that points to the inserted data.
     *
     *  This function will insert an object of type T constructed
     *  with T(std::forward<Args>(args)...) before the specified
     *  location.  Due to the nature of a %List this operation can
     *  be done in constant time, and does not invalidate iterators
     *  and references.
     */
    template<typename... Args>
    iterator emplace(iterator position, Args &&... args);
#endif

    /**
     *  @brief  Inserts given value into %List before specified iterator.
     *  @param  position  An iterator into the %List.
     *  @param  x  Data to be inserted.
     *  @return  An iterator that points to the inserted data.
     *
     *  This function will insert a copy of the given value before
     *  the specified location.  Due to the nature of a %List this
     *  operation can be done in constant time, and does not
     *  invalidate iterators and references.
     */
    iterator insert(iterator position, const value_type &x);

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    /**
     *  @brief  Inserts given rvalue into %List before specified iterator.
     *  @param  position  An iterator into the %List.
     *  @param  x  Data to be inserted.
     *  @return  An iterator that points to the inserted data.
     *
     *  This function will insert a copy of the given rvalue before
     *  the specified location.  Due to the nature of a %List this
     *  operation can be done in constant time, and does not
     *  invalidate iterators and references.
     */
    iterator insert(iterator position, value_type &&x);

    /**
     *  @brief  Inserts the contents of an initializer_list into %List
     *          before specified iterator.
     *  @param  p  An iterator into the %List.
     *  @param  l  An initializer_list of value_type.
     *
     *  This function will insert copies of the data in the
     *  initializer_list @a l into the %List before the location
     *  specified by @a p.
     *
     *  This operation is linear in the number of elements inserted and
     *  does not invalidate iterators and references.
     */
    WARN_UNUSED_RETURN bool insert(iterator p, std::initializer_list<value_type> l);
#endif

    /**
     *  @brief  Inserts a number of copies of given data into the %List.
     *  @param  position  An iterator into the %List.
     *  @param  n  Number of elements to be inserted.
     *  @param  x  Data to be inserted.
     *
     *  This function will insert a specified number of copies of the
     *  given data before the location specified by @a position.
     *
     *  This operation is linear in the number of elements inserted and
     *  does not invalidate iterators and references.
     */
    WARN_UNUSED_RETURN bool insert(iterator position, size_type n, const value_type &x);

    /**
     *  @brief  Inserts a range into the %List.
     *  @param  position  An iterator into the %List.
     *  @param  first  An input iterator.
     *  @param  last   An input iterator.
     *
     *  This function will insert copies of the data in the range [@a
     *  first,@a last) into the %List before the location specified by
     *  @a position.
     *
     *  This operation is linear in the number of elements inserted and
     *  does not invalidate iterators and references.
     */
    template<typename InputIterator>
    WARN_UNUSED_RETURN bool insert(iterator position, InputIterator first, InputIterator last);

    /**
     *  @brief  Remove element at given position.
     *  @param  position  Iterator pointing to element to be erased.
     *  @return  An iterator pointing to the next element (or end()).
     *
     *  This function will erase the element at the given position and thus
     *  shorten the %List by one.
     *
     *  Due to the nature of a %List this operation can be done in
     *  constant time, and only invalidates iterators/references to
     *  the element being removed.  The user is also cautioned that
     *  this function only erases the element, and that if the element
     *  is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(iterator position);

    /**
     *  @brief  Remove a range of elements.
     *  @param  first  Iterator pointing to the first element to be erased.
     *  @param  last  Iterator pointing to one past the last element to be
     *                erased.
     *  @return  An iterator pointing to the element pointed to by @a last
     *           prior to erasing (or end()).
     *
     *  This function will erase the elements in the range @a
     *  [first,last) and shorten the %List accordingly.
     *
     *  This operation is linear time in the size of the range and only
     *  invalidates iterators/references to the element being removed.
     *  The user is also cautioned that this function only erases the
     *  elements, and that if the elements themselves are pointers, the
     *  pointed-to memory is not touched in any way.  Managing the pointer
     *  is the user's responsibility.
     */
    iterator erase(iterator first, iterator last);

    /**
     *  @brief  Swaps data with another %List.
     *  @param  x  A %List of the same element and allocator types.
     *
     *  This exchanges the elements between two lists in constant
     *  time.  Note that the global std::swap() function is
     *  specialized such that std::swap(l1,l2) will feed to this
     *  function.
     */
    void swap(List &x);

    /**
     *  Erases all the elements.  Note that this function only erases
     *  the elements, and that if the elements themselves are
     *  pointers, the pointed-to memory is not touched in any way.
     *  Managing the pointer is the user's responsibility.
     */
    void clear() NOEXCEPT;

    // [23.2.2.4] list operations
    /**
     *  @brief  Insert contents of another %List.
     *  @param  position  Iterator referencing the element to insert before.
     *  @param  x  Source list.
     *
     *  The elements of @a x are inserted in constant time in front of
     *  the element referenced by @a position.  @a x becomes an empty
     *  list.
     *
     *  Requires this != @a x.
     */
    void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    splice(iterator position, List &&x);
#else
    splice(iterator position, List &x);
#endif

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    void splice(iterator position, List &x);
#endif

    /**
     *  @brief  Insert element from another %List.
     *  @param  position  Iterator referencing the element to insert before.
     *  @param  x  Source list.
     *  @param  i  Iterator referencing the element to move.
     *
     *  Removes the element in list @a x referenced by @a i and
     *  inserts it into the current list before @a position.
     */
    void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    splice(iterator position, List &&x, iterator i);
#else
    splice(iterator position, List &x, iterator i);
#endif

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    void splice(iterator position, List &x, iterator i);
#endif

    /**
     *  @brief  Insert range from another %List.
     *  @param  position  Iterator referencing the element to insert before.
     *  @param  x  Source list.
     *  @param  first  Iterator referencing the start of range in x.
     *  @param  last  Iterator referencing the end of range in x.
     *
     *  Removes elements in the range [first,last) and inserts them
     *  before @a position in constant time.
     *
     *  Undefined if @a position is in [first,last).
     */
    void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    splice(iterator position, List &&x, iterator first, iterator last);
#else
    splice(iterator position, List &x, iterator first, iterator last);
#endif

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    void splice(iterator position, List &x, iterator first, iterator last);
#endif

    /**
     *  @brief  Remove all elements equal to value.
     *  @param  value  The value to remove.
     *
     *  Removes every element in the list equal to @a value.
     *  Remaining elements stay in list order.  Note that this
     *  function only erases the elements, and that if the elements
     *  themselves are pointers, the pointed-to memory is not
     *  touched in any way.  Managing the pointer is the user's
     *  responsibility.
     */
    void remove(const T &value);

    /**
     *  @brief  Remove all elements satisfying a predicate.
     *  @tparam  Predicate  Unary predicate function or object.
     *
     *  Removes every element in the list for which the predicate
     *  returns true.  Remaining elements stay in list order.  Note
     *  that this function only erases the elements, and that if the
     *  elements themselves are pointers, the pointed-to memory is
     *  not touched in any way.  Managing the pointer is the user's
     *  responsibility.
     */
    template<typename Predicate>
    void remove_if(Predicate);

    /**
     *  @brief  Remove consecutive duplicate elements.
     *
     *  For each consecutive set of elements with the same value,
     *  remove all but the first one.  Remaining elements stay in
     *  list order.  Note that this function only erases the
     *  elements, and that if the elements themselves are pointers,
     *  the pointed-to memory is not touched in any way.  Managing
     *  the pointer is the user's responsibility.
     */
    void unique();

    /**
     *  @brief  Remove consecutive elements satisfying a predicate.
     *  @tparam BinaryPredicate  Binary predicate function or object.
     *
     *  For each consecutive set of elements [first,last) that
     *  satisfy predicate(first,i) where i is an iterator in
     *  [first,last), remove all but the first one.  Remaining
     *  elements stay in list order.  Note that this function only
     *  erases the elements, and that if the elements themselves are
     *  pointers, the pointed-to memory is not touched in any way.
     *  Managing the pointer is the user's responsibility.
     */
    template<typename BinaryPredicate>
    void unique(BinaryPredicate);

    /**
     *  @brief  Merge sorted lists.
     *  @param  x  Sorted list to merge.
     *
     *  Assumes that both @a x and this list are sorted according to
     *  operator<().  Merges elements of @a x into this list in
     *  sorted order, leaving @a x empty when complete.  Elements in
     *  this list precede elements in @a x that are equal.
     */
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    void merge(List &&x);
    void merge(List &x);
#else
    void merge(List &x);
#endif

    /**
     *  @brief  Merge sorted lists according to comparison function.
     *  @tparam StrictWeakOrdering Comparison function defining
     *  sort order.
     *  @param  x  Sorted list to merge.
     *  @param  comp  Comparison functor.
     *
     *  Assumes that both @a x and this list are sorted according to
     *  StrictWeakOrdering.  Merges elements of @a x into this list
     *  in sorted order, leaving @a x empty when complete.  Elements
     *  in this list precede elements in @a x that are equivalent
     *  according to StrictWeakOrdering().
     */
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    template<typename StrictWeakOrdering>
    void merge(List &&x, StrictWeakOrdering comp);

    template<typename StrictWeakOrdering>
    void merge(List &x, StrictWeakOrdering comp);
#else
    template<typename StrictWeakOrdering>
    void merge(List &x, StrictWeakOrdering comp);
#endif

    /**
     *  @brief  Reverse the elements in list.
     *
     *  Reverse the order of elements in the list in linear time.
     */
    void reverse() NOEXCEPT;

    /**
     *  @brief  Sort the elements.
     *
     *  Sorts the elements of this list in NlogN time.  Equivalent
     *  elements remain in list order.
     */
    void sort();

    /**
     *  @brief  Sort the elements according to comparison function.
     *
     *  Sorts the elements of this list in NlogN time.  Equivalent
     *  elements remain in list order.
     */
    template<typename StrictWeakOrdering>
    void sort(StrictWeakOrdering);

protected:
    void init();
    void internal_clear();

private:
#if PLATFORM_COMPILER(GCC)
    // concept requirements
    typedef typename Allocator::value_type AllocValueType;
    __glibcxx_class_requires(T, _SGIAssignableConcept)
    __glibcxx_class_requires2(T, AllocValueType, _SameTypeConcept)
#endif

    struct NodeBase
    {
        static void swap(NodeBase &x, NodeBase &y) _GLIBCXX_USE_NOEXCEPT;
        void transfer(NodeBase * const first, NodeBase * const last) _GLIBCXX_USE_NOEXCEPT;
        void reverse() _GLIBCXX_USE_NOEXCEPT;
        void hook(NodeBase * const position) _GLIBCXX_USE_NOEXCEPT;
        void unhook() _GLIBCXX_USE_NOEXCEPT;

        NodeBase *next;
        NodeBase *prev;
    };

    /// An actual node in the %List.
    struct Node : public NodeBase
    {
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
        template<typename ... Args>
        Node(Args &&... args) :
            NodeBase(),
            data(std::forward<Args>(args)...)
        {}
#endif

        ///< User's data.
        T data;
    };

    typedef typename Allocator::template rebind<Node>::other Node_alloc_type;
    typedef typename Allocator::template rebind<T>::other    T_alloc_type;

protected:
    /**
     *  @param  args  An instance of user data.
     *
     *  Allocates space for a new node and constructs a copy of
     *  @a args in it.
     */
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    template<typename ... Args>
    WARN_UNUSED_RETURN Node *create_node(Args &&... args)
    {
        Node *p = m_allocator.allocate(1);

        PLATFORM_TRY
        {
            m_allocator.construct(p, std::forward<Args>(args)...);
        }
        PLATFORM_CATCH(...)
        {
            m_allocator.deallocate(p, 1);
            PLATFORM_RETHROW;
        }

        return p;
    }
#else
    WARN_UNUSED_RETURN Node *create_node(const value_type &x)
    {
        Node *p = m_allocator.allocate(1);

        PLATFORM_TRY
        {
            m_allocator.construct(&p->data, x);
        }
        PLATFORM_CATCH(...)
        {
            m_allocator.deallocate(p, 1);
            PLATFORM_RETHROW;
        }

        return p;
    }
#endif

protected:
    // Internal constructor functions follow.

    // Called by the range constructor to implement [23.1.1]/9

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 438. Ambiguity in the "do the right thing" clause
    template<typename Integer>
    void initialize_dispatch(Integer n, Integer x, bool_value<true>)
    {
        fill_initialize(static_cast<size_type>(n), x);
    }

    // Called by the range constructor to implement [23.1.1]/9
    template<typename InputIterator>
    void initialize_dispatch(InputIterator first, InputIterator last, bool_value<false>)
    {
        for (; first != last; ++first)
            if (UNLIKELY(push_back(*first) == false))
            {
                clear();
                return;
            }
    }

    // Called by list(n,v,a), and the range constructor when it turns out
    // to be the same thing.
    void fill_initialize(size_type n, const value_type &x)
    {
        for (; n; --n)
            if (UNLIKELY(push_back(x) == false))
            {
                clear();
                return;
            }
    }

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    // Called by list(n).
    void default_initialize(size_type n)
    {
        for (; n; --n)
            if (UNLIKELY(emplace_back() == false))
            {
                clear();
                return;
            }
    }

    // Called by resize(sz).
    WARN_UNUSED_RETURN bool default_append(size_type n);
#endif

    // Internal assign functions follow.

    // Called by the range assign to implement [23.1.1]/9

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 438. Ambiguity in the "do the right thing" clause
    template<typename Integer>
    void assign_dispatch(Integer n, Integer val, bool_value<true>)
    {
        fill_assign(n, val);
    }

    // Called by the range assign to implement [23.1.1]/9
    template<typename InputIterator>
    void assign_dispatch(InputIterator first, InputIterator last, bool_value<false>);

    // Called by assign(n,t), and the range assign when it turns out
    // to be the same thing.
    WARN_UNUSED_RETURN bool fill_assign(size_type n, const value_type &val);

    // Moves the elements from [first,last) before position.
    void transfer(iterator position, iterator first, iterator last)
    {
        position.node->transfer(first.node, last.node);
    }

    // Inserts new element at position given and with value given.
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
    template<typename... _Args>
    WARN_UNUSED_RETURN bool internal_insert(iterator position, _Args &&... args)
    {
        Node *tmp = create_node(std::forward<_Args>(args)...);

        if (LIKELY(tmp != NULL))
            tmp->hook(position.node);
        else
            return false;

        return true;
    }
#else
    WARN_UNUSED_RETURN bool internal_insert(iterator position, const value_type &x)
    {
        Node *tmp = create_node(x);

        if (LIKELY(tmp != NULL))
            tmp->hook(position.node);
        else
            return false;

        return true;
    }
#endif

    // Erases element at position given.
    void internal_erase(iterator position)
    {
        position.node->unhook();
        Node *n = static_cast<Node *>(position.node);

        m_allocator.destroy(n);
        m_allocator.deallocate(n, 1);
    }

    // To implement the splice (and merge) bits of N1599.
    void _M_check_equal_allocators(List &x)
    {
//        if (std::alloc_neq<typename _Base::_Node_alloc_type>::_S_do_it(_M_get_Node_allocator(), x._M_get_Node_allocator()))
//            throw_runtime_error(__N("List::_M_check_equal_allocators"));
    }

private:
    NodeBase m_node;
    Node_alloc_type m_allocator;
};


template<typename T, typename Allocator>
List<T, Allocator>::List()
{
    init();
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const Allocator &a) :
    m_allocator(Node_alloc_type(a))
{
    init();
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
List<T, Allocator>::List(List &&x) :
    m_allocator(std::move(x.m_allocator))
{
    init();
    NodeBase::swap(m_node, x.m_node);
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_type n)
{
    init();
    default_initialize(n);
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_type n, const value_type &value, const Allocator &a) :
    m_allocator(Node_alloc_type(a))
{
    init();
    fill_initialize(n, value);
}

template<typename T, typename Allocator>
List<T, Allocator>::List(std::initializer_list<value_type> l, const Allocator &a) :
    m_allocator(Node_alloc_type(a))
{
    init();
    initialize_dispatch(l.begin(), l.end(), bool_value<false>());
}
#else
template<typename T, typename Allocator>
List<T, Allocator>::List(size_type n, const value_type &value = value_type(), const Allocator &a) :
    m_allocator(Node_alloc_type(a))
{
    init();
    fill_initialize(n, value);
}
#endif

template<typename T, typename Allocator>
List<T, Allocator>::List(const List &x) :
    m_allocator(x.m_allocator)
{
    init();
    initialize_dispatch(x.begin(), x.end(), bool_value<false>());
}

template<typename T, typename Allocator>
template<typename InputIterator>
List<T, Allocator>::List(InputIterator first, InputIterator last, const Allocator &a) :
    m_allocator(Node_alloc_type(a))
{
    init();

    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename is_integer<InputIterator>::type Integral;
    initialize_dispatch(first, last, Integral());
}

template<typename T, typename Allocator>
List<T, Allocator>::~List()
{
    internal_clear();
}

template<typename T, typename Allocator>
List<T, Allocator> &List<T, Allocator>::operator=(const List &x)
{
    if (this != &x)
    {
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = x.begin();
        const_iterator last2 = x.end();

        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            *first1 = *first2;

        if (first2 == last2)
            erase(first1, last1);
        else
        {
            WARN_UNUSED_RETURN_OFF
            insert(last1, first2, last2);
            WARN_UNUSED_RETURN_ON
        }
    }

    return *this;
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
List<T, Allocator> &List<T, Allocator>::operator=(List &&x)
{
    clear();
    swap(x);
    return *this;
}

template<typename T, typename Allocator>
List<T, Allocator> &List<T, Allocator>::operator=(std::initializer_list<value_type> l)
{
    assign(l.begin(), l.end());
    return *this;
}
#endif

template<typename T, typename Allocator>
void List<T, Allocator>::assign(size_type n, const value_type &val)
{
    fill_assign(n, val);
}

template<typename T, typename Allocator>
template<typename InputIterator>
void List<T, Allocator>::assign(InputIterator first, InputIterator last)
{
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename is_integer<InputIterator>::type Integral;
    assign_dispatch(first, last, Integral());
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
void List<T, Allocator>::assign(std::initializer_list<value_type> l)
{
    assign(l.begin(), l.end());
}
#endif

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::begin()
{
    return iterator(m_node.next);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::begin() const
{
    return const_iterator(m_node.next);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::end()
{
    return iterator(&m_node);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::end() const
{
    return const_iterator(&m_node);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rbegin()
{
    return reverse_iterator(end());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rend()
{
    return reverse_iterator(begin());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rend() const
{
    return const_reverse_iterator(begin());
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::cbegin() const
{
    return const_iterator(m_node.next);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::cend() const
{
    return const_iterator(&m_node);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crbegin() const
{
    return const_reverse_iterator(end());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crend() const
{
    return const_reverse_iterator(begin());
}
#endif

template<typename T, typename Allocator>
bool List<T, Allocator>::empty() const
{
    return m_node.next == &m_node;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::size_type List<T, Allocator>::size() const
{
    return std::distance(begin(), end());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::size_type List<T, Allocator>::max_size() const
{
    return m_allocator.max_size();
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
bool List<T, Allocator>::resize(size_type new_size)
{
    iterator i = begin();
    size_type len = 0;

    for (; i != end() && len < new_size; ++i, ++len);

    if (len == new_size)
        erase(i, end());
    else // i == end()
        return default_append(new_size - len);

    return true;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::resize(size_type new_size, const value_type &x)
{
    iterator i = begin();
    size_type len = 0;

    for (; i != end() && len < new_size; ++i, ++len);

    if (len == new_size)
        erase(i, end());
    else // i == end()
        return insert(end(), new_size - len, x);

    return true;
}
#else
template<typename T, typename Allocator>
bool List<T, Allocator>::resize(size_type new_size, value_type x)
{
    iterator i = begin();
    size_type len = 0;

    for (; i != end() && len < new_size; ++i, ++len);

    if (len == new_size)
        erase(i, end());
    else // i == end()
        return insert(end(), new_size - len, x);

    return true;
}
#endif

template<typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::front()
{
    return *begin();
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::front() const
{
    return *begin();
}

template<typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::back()
{
    return *(--end());
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::back() const
{
    return *(--end());
}

template<typename T, typename Allocator>
bool List<T, Allocator>::push_front(const value_type &x)
{
    return internal_insert(begin(), x);
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
bool List<T, Allocator>::push_front(value_type &&x)
{
    return internal_insert(begin(), std::move(x));
}

template<typename T, typename Allocator>
template<typename... Args>
bool List<T, Allocator>::emplace_front(Args &&... args)
{
    return internal_insert(begin(), std::forward<Args>(args)...);
}
#endif

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front()
{
    internal_erase(begin());
}

template<typename T, typename Allocator>
bool List<T, Allocator>::push_back(const value_type &x)
{
    return internal_insert(end(), x);
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
bool List<T, Allocator>::push_back(value_type &&x)
{
    return internal_insert(end(), std::move(x));
}

template<typename T, typename Allocator>
template<typename... Args>
bool List<T, Allocator>::emplace_back(Args &&... args)
{
    return internal_insert(end(), std::forward<Args>(args)...);
}
#endif

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back()
{
    internal_erase(iterator(m_node.prev));
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
template<typename... Args>
typename List<T, Allocator>::iterator List<T, Allocator>::emplace(iterator position, Args &&... args)
{
    Node *tmp = create_node(std::forward<Args>(args)...);

    if (LIKELY(tmp != NULL))
    {
        tmp->hook(position.node);
        return iterator(tmp);
    }

    return end();
}
#endif

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::insert(iterator position, const value_type &x)
{
    Node *tmp = create_node(x);

    if (LIKELY(tmp != NULL))
    {
        tmp->hook(position.node);
        return iterator(tmp);
    }

    return end();
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::insert(iterator position, value_type &&x)
{
    return emplace(position, std::move(x));
}

template<typename T, typename Allocator>
bool List<T, Allocator>::insert(iterator p, std::initializer_list<value_type> l)
{
    return insert(p, l.begin(), l.end());
}
#endif

template<typename T, typename Allocator>
bool List<T, Allocator>::insert(iterator position, size_type n, const value_type &x)
{
    List<T, Allocator> tmp(n, x, m_allocator);

    if (UNLIKELY(tmp.empty() == true))
        return false;
    else
        splice(position, tmp);

    return true;
}

template<typename T, typename Allocator>
template<typename InputIterator>
bool List<T, Allocator>::insert(iterator position, InputIterator first, InputIterator last)
{
    List<T, Allocator> tmp(first, last, m_allocator);

    if (UNLIKELY(tmp.empty() == true))
        return false;
    else
        splice(position, tmp);

    return true;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::erase(iterator position)
{
    iterator ret = iterator(position.node->next);
    internal_erase(position);
    return ret;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::erase(iterator first, iterator last)
{
    while (first != last)
        first = erase(first);
    return last;
}

template<typename T, typename Allocator>
void List<T, Allocator>::swap(List &x)
{
    NodeBase::swap(m_node, x.m_node);

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 431. Swapping containers with unequal allocators.
    //std::alloc_swap<typename Base::_Node_alloc_type>::
    //_S_do_it(_M_get_Node_allocator(), x._M_get_Node_allocator());
}

template<typename T, typename Allocator>
void List<T, Allocator>::clear()
{
    internal_clear();
    init();
}

template<typename T, typename Allocator>
void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
List<T, Allocator>::splice(iterator position, List &&x)
#else
List<T, Allocator>::splice(iterator position, List &x)
#endif
{
    if (!x.empty())
    {
        _M_check_equal_allocators(x);

        transfer(position, x.begin(), x.end());
    }
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
void List<T, Allocator>::splice(iterator position, List &x)
{
    splice(position, std::move(x));
}
#endif

template<typename T, typename Allocator>
void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
List<T, Allocator>::splice(iterator position, List &&x, iterator i)
#else
List<T, Allocator>::splice(iterator position, List &x, iterator i)
#endif
{
    iterator j = i;
    ++j;

    if (position == i || position == j)
        return;

    if (this != &x)
        _M_check_equal_allocators(x);

    transfer(position, i, j);
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
void List<T, Allocator>::splice(iterator position, List &x, iterator i)
{
    splice(position, std::move(x), i);
}
#endif

template<typename T, typename Allocator>
void
#if PLATFORM_COMPILER_SUPPORTS(CXX11)
List<T, Allocator>::splice(iterator position, List &&x, iterator first, iterator last)
#else
List<T, Allocator>::splice(iterator position, List &x, iterator first, iterator last)
#endif
{
    if (first != last)
    {
        if (this != &x)
            _M_check_equal_allocators(x);

        transfer(position, first, last);
    }
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
void List<T, Allocator>::splice(iterator position, List &x, iterator first, iterator last)
{
    splice(position, std::move(x), first, last);
}
#endif

template<typename T, typename Allocator>
void List<T, Allocator>::remove(const T &value)
{
    iterator first = begin();
    iterator last = end();
    iterator extra = last;

    while (first != last)
    {
        iterator next = first;
        ++next;

        if (*first == value)
        {
            // _GLIBCXX_RESOLVE_LIB_DEFECTS
            // 526. Is it undefined if a function in the standard changes
            // in parameters?
            if (&(*first) != &(value))
                internal_erase(first);
            else
                extra = first;
        }

        first = next;
    }

    if (extra != last)
        internal_erase(extra);
}

template<typename T, typename Allocator>
template<typename Predicate>
void List<T, Allocator>::remove_if(Predicate pred)
{
    iterator first = begin();
    iterator last = end();

    while (first != last)
    {
        iterator next = first;
        ++next;

        if (pred(*first))
            internal_erase(first);

        first = next;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::unique()
{
    iterator first = begin();
    iterator last = end();

    if (first == last)
        return;

    iterator next = first;

    while (++next != last)
    {
        if (*first == *next)
            internal_erase(next);
        else
            first = next;

        next = first;
    }
}

template<typename T, typename Allocator>
template<typename BinaryPredicate>
void List<T, Allocator>::unique(BinaryPredicate binary_pred)
{
    iterator first = begin();
    iterator last = end();

    if (first == last)
        return;

    iterator next = first;

    while (++next != last)
    {
        if (binary_pred(*first, *next))
            internal_erase(next);
        else
            first = next;

        next = first;
    }
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
void List<T, Allocator>::merge(List &x)
{
    merge(std::move(x));
}

template<typename T, typename Allocator>
void List<T, Allocator>::merge(List &&x)
#else
template<typename T, typename Allocator>
void List<T, Allocator>::merge(List &x)
#endif
{
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 300. List::merge() specification incomplete
    if (this != &x)
    {
        _M_check_equal_allocators(x);

        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while (first1 != last1 && first2 != last2)
            if (*first2 < *first1)
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;

        if (first2 != last2)
            transfer(last1, first2, last2);
    }
}

#if PLATFORM_COMPILER_SUPPORTS(CXX11)
template<typename T, typename Allocator>
template<typename StrictWeakOrdering>
void List<T, Allocator>::merge(List &x, StrictWeakOrdering comp)
{
    merge(std::move(x), comp);
}

template<typename T, typename Allocator>
template<typename StrictWeakOrdering>
void List<T, Allocator>::merge(List &&x, StrictWeakOrdering comp)
#else
template<typename T, typename Allocator>
template<typename StrictWeakOrdering>
void List<T, Allocator>::merge(List &x, StrictWeakOrdering comp)
#endif
{
    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 300. List::merge() specification incomplete
    if (this != &x)
    {
        _M_check_equal_allocators(x);

        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while (first1 != last1 && first2 != last2)
            if (comp(*first2, *first1))
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;

        if (first2 != last2)
            transfer(last1, first2, last2);
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::reverse()
{
    m_node.reverse();
}

template<typename T, typename Allocator>
void List<T, Allocator>::sort()
{
    // Do nothing if the list has length 0 or 1.
    if (m_node.next != &m_node && m_node.next->next != &m_node)
    {
        List carry;
        List tmp[64];
        List * fill = &tmp[0];
        List * counter;

        do
        {
            carry.splice(carry.begin(), *this, begin());

            for(counter = &tmp[0]; counter != fill && !counter->empty(); ++counter)
            {
            counter->merge(carry);
            carry.swap(*counter);
            }

            carry.swap(*counter);

            if (counter == fill)
                ++fill;
        }
        while (!empty());

        for (counter = &tmp[1]; counter != fill; ++counter)
            counter->merge(*(counter - 1));

        swap(*(fill - 1));
    }
}

template<typename T, typename Allocator>
template<typename StrictWeakOrdering>
void List<T, Allocator>::sort(StrictWeakOrdering comp)
{
    // Do nothing if the list has length 0 or 1.
    if (m_node.next != &m_node && m_node.next->next != &m_node)
    {
        List carry;
        List tmp[64];
        List * fill = &tmp[0];
        List * counter;

        do
        {
            carry.splice(carry.begin(), *this, begin());

            for(counter = &tmp[0]; counter != fill && !counter->empty(); ++counter)
            {
                counter->merge(carry, comp);
                carry.swap(*counter);
            }

            carry.swap(*counter);

            if (counter == fill)
                ++fill;
        }
        while (!empty());

        for (counter = &tmp[1]; counter != fill; ++counter)
            counter->merge(*(counter - 1), comp);

        swap(*(fill - 1));
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::init()
{
    m_node.next = &m_node;
    m_node.prev = &m_node;
}

template<typename T, typename Allocator>
void List<T, Allocator>::internal_clear()
{
    Node *cur = static_cast<Node *>(m_node.next);

    while (cur != &m_node)
    {
        Node *tmp = cur;
        cur = static_cast<Node *>(cur->next);

        m_allocator.destroy(tmp);
        m_allocator.deallocate(tmp, 1);
    }
}

template<typename T, typename Allocator>
bool List<T, Allocator>::default_append(size_type n)
{
    size_type i = 0;

    PLATFORM_TRY
    {
        for (; i < n; ++i)
            if (UNLIKELY(emplace_back() == false))
            {
                clear();
                return false;
            }
    }
    PLATFORM_CATCH(...)
    {
        for (; i; --i)
            pop_back();

        PLATFORM_RETHROW;
        return false;
    }

    return true;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::fill_assign(size_type n, const value_type &val)
{
    iterator i = begin();

    for (; i != end() && n > 0; ++i, --n)
        *i = val;

    if (n > 0)
        return insert(end(), n, val);
    else
        erase(i, end());

    return true;
}

template<typename T, typename Allocator>
void List<T, Allocator>::NodeBase::swap(NodeBase &x, NodeBase &y)
{
    if (x.next != &x)
    {
        if (y.next != &y)
        {
            // Both x and y are not empty.
            std::swap(x.next, y.next);
            std::swap(x.prev, y.prev);
            x.next->prev = x.prev->next = &x;
            y.next->prev = y.prev->next = &y;
        }
        else
        {
            // x is not empty, y is empty.
            y.next = x.next;
            y.prev = x.prev;
            y.next->prev = y.prev->next = &y;
            x.next = x.prev = &x;
        }
    }
    else if (y.next != &y)
    {
        // x is empty, y is not empty.
        x.next = y.next;
        x.prev = y.prev;
        x.next->prev = x.prev->next = &x;
        y.next = y.prev = &y;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::NodeBase::transfer(NodeBase * const first, NodeBase * const last)
{
    if (this != last)
    {
        // Remove [first, last) from its old position.
        last->prev->next  = this;
        first->prev->next = last;
        prev->next        = first;

        // Splice [first, last) into its new position.
        NodeBase * const tmp = prev;
        prev                 = last->prev;
        last->prev           = first->prev;
        first->prev          = tmp;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::NodeBase::reverse()
{
    NodeBase *tmp = this;
    do
    {
        std::swap(tmp->next, tmp->prev);

        // Old next node is now prev.
        tmp = tmp->prev;
    }
    while (tmp != this);
}

template<typename T, typename Allocator>
void List<T, Allocator>::NodeBase::hook(NodeBase * const position)
{
    next = position;
    prev = position->prev;
    position->prev->next = this;
    position->prev = this;
}

template<typename T, typename Allocator>
void List<T, Allocator>::NodeBase::unhook()
{
    NodeBase * const next_node = next;
    NodeBase * const prev_node = prev;
    prev_node->next = next_node;
    next_node->prev = prev_node;
}


template<typename T, typename Allocator>
List<T, Allocator>::iterator::iterator() :
    node()
{}

template<typename T, typename Allocator>
List<T, Allocator>::iterator::iterator(NodeBase *x) :
    node(x)
{}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator::reference List<T, Allocator>::iterator::operator*() const
{
    return static_cast<Node *>(node)->data;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator::pointer List<T, Allocator>::iterator::operator->() const
{
    return &static_cast<Node *>(node)->data;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator &List<T, Allocator>::iterator::operator++()
{
    node = node->next;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::iterator::operator++(int)
{
    iterator tmp = *this;
    node = node->next;
    return tmp;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator &List<T, Allocator>::iterator::operator--()
{
    node = node->prev;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::iterator::operator--(int)
{
    iterator tmp = *this;
    node = node->prev;
    return tmp;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::iterator::operator==(const iterator &x) const
{
    return node == x.node;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::iterator::operator!=(const iterator &x) const
{
    return node != x.node;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::iterator::operator==(const const_iterator &x) const
{
    return node == x.node;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::iterator::operator!=(const const_iterator &x) const
{
    return node != x.node;
}


template<typename T, typename Allocator>
List<T, Allocator>::const_iterator::const_iterator() :
    node()
{}

template<typename T, typename Allocator>
List<T, Allocator>::const_iterator::const_iterator(const NodeBase *x) :
    node(x)
{}

template<typename T, typename Allocator>
List<T, Allocator>::const_iterator::const_iterator(const iterator &x) :
    node(x.node)
{}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator::reference List<T, Allocator>::const_iterator::operator*() const
{
    return static_cast<const Node *>(node)->data;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator::pointer List<T, Allocator>::const_iterator::operator->() const
{
    return &static_cast<const Node *>(node)->data;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator &List<T, Allocator>::const_iterator::operator++()
{
    node = node->next;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::const_iterator::operator++(int)
{
    const_iterator tmp = *this;
    node = node->next;
    return tmp;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator &List<T, Allocator>::const_iterator::operator--()
{
    node = node->prev;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::const_iterator::operator--(int)
{
    const_iterator tmp = *this;
    node = node->prev;
    return tmp;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::const_iterator::operator==(const const_iterator &x) const
{
    return node == x.node;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::const_iterator::operator!=(const const_iterator &x) const
{
    return node != x.node;
}

}


/**
 *  @brief  List equality comparison.
 *  @param  x  A %List.
 *  @param  y  A %List of the same type as @a x.
 *  @return  True iff the size and elements of the lists are equal.
 *
 *  This is an equivalence relation.  It is linear in the size of
 *  the lists.  Lists are considered equivalent if their sizes are
 *  equal, and if corresponding elements compare equal.
 */
template<typename T, typename Allocator>
inline bool operator==(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    typedef typename EFC::List<T, Allocator>::const_iterator const_iterator;
    const_iterator end1 = x.end();
    const_iterator end2 = y.end();

    const_iterator i1 = x.begin();
    const_iterator i2 = y.begin();

    while (i1 != end1 && i2 != end2 && *i1 == *i2)
    {
        ++i1;
        ++i2;
    }

    return i1 == end1 && i2 == end2;
}


/**
 *  @brief  List ordering relation.
 *  @param  x  A %List.
 *  @param  y  A %List of the same type as @a x.
 *  @return  True if @a x is lexicographically less than @a y.
 *
 *  This is a total ordering relation.  It is linear in the size of the
 *  lists.  The elements must be comparable with @c <.
 *
 *  See std::lexicographical_compare() for how the determination is made.
 */
template<typename T, typename Allocator>
inline bool operator<(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

/// Based on operator==
template<typename T, typename Allocator>
inline bool operator!=(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    return !(x == y);
}

/// Based on operator<
template<typename T, typename Allocator>
inline bool operator>(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    return y < x;
}

/// Based on operator<
template<typename T, typename Allocator>
inline bool operator<=(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    return !(y < x);
}

/// Based on operator<
template<typename T, typename Allocator>
inline bool operator>=(const EFC::List<T, Allocator> &x, const EFC::List<T, Allocator> &y)
{
    return !(x < y);
}

/// See EFC::List::swap().
template<typename T, typename Allocator>
inline void swap(EFC::List<T, Allocator> &x, EFC::List<T, Allocator> &y)
{
    x.swap(y);
}

#endif /* EFC_LIST_H_ */
