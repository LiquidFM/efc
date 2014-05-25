/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * The EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef EFC_ORDEREDMAP_H_
#define EFC_ORDEREDMAP_H_

#include <efc/Map>
#include <efc/List>
#include <efc/type_traits>


namespace EFC {

template <typename Key, typename T>
class OrderedMap
{
public:
    typedef List<T>                       ValueList;
    typedef List<Key>                     KeyList;
    typedef Map<Key, size_type>           ValueMap;

    typedef T                                   value_type;
    typedef T *                                 pointer;
    typedef const T *                           const_pointer;
    typedef T &                                 reference;
    typedef const T &                           const_reference;
	typedef typename ValueList::size_type       size_type;
    typedef typename ValueList::difference_type difference_type;

    struct const_iterator;
    struct iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t                  difference_type;
        typedef T                               value_type;
        typedef T *                             pointer;
        typedef T &                             reference;

        iterator();
        explicit iterator(typename ValueList::iterator iterator);

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

        typename ValueList::iterator m_iterator;
    };

    struct const_iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t                  difference_type;
        typedef T                               value_type;
        typedef const T *                       pointer;
        typedef const T &                       reference;

        const_iterator();
        explicit const_iterator(typename ValueList::const_iterator iterator);
        const_iterator(const iterator &x);

        reference operator*() const;
        pointer operator->() const;
        const_iterator &operator++();
        const_iterator operator++(int);
        const_iterator &operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator &x) const;
        bool operator!=(const const_iterator &x) const;

        typename ValueList::const_iterator m_iterator;
    };

    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef std::reverse_iterator<iterator>        reverse_iterator;

public:
	OrderedMap()
	{}

	~OrderedMap()
	{}

	T &operator[](size_type index) { return m_list[index]; }
	const T &operator[](size_type index) const { return m_list[index]; }
	const T &at(size_type index) const { return m_list.at(index); }
	const T &last() const { return m_list.last(); }
	size_type lastIndex() const { return m_list.size() - 1; }
	T &last() { return m_list.last(); }

	bool isEmpty() const { return m_list.isEmpty(); }
	size_type size() const { return m_list.size(); }
	size_type indexOf(const Key &hash) const { return m_map.value(hash, InvalidIndex); }
	size_type indexOf(const T &value) const { return m_list.indexOf(value); }
	bool contains(const Key &hash) const { return m_map.contains(hash); }

	KeyList keys() const { return m_map.keys(); }
	const ValueList &values() const { return m_list; }

	QSet<T> toSet() const { return m_list.toSet(); }

	void add(const Key &hash, const T &value)
	{
		m_list.push_back(value);
		m_map[hash] = m_list.size() - 1;
	}
	void remove(const Key &hash)
	{
		size_type index = indexOf(hash);

		if (index != InvalidIndex)
			remove(index);
	}
	void remove(size_type index)
	{
		for (typename ValueMap::iterator it = m_map.begin(), end = m_map.end(); it != end; ++it)
			if (*it == index)
			{
				for (it = m_map.erase(it); it != end; ++it)
					if (*it > index)
						--*it;

				break;
			}
			else
				if (*it > index)
					--*it;

		m_list.removeAt(index);
	}
	T take(const Key &hash)
	{
		size_type index = indexOf(hash);

		if (index != InvalidIndex)
			return take(index);

		return T();
	}
	void replace(size_type index, const Key &oldKey, const Key &newKey, const T &value)
	{
		m_list[index] = value;
		m_map[newKey] = index;
		m_map.remove(oldKey);
	}
	void replace(size_type index, const Key &oldKey, const Key &newKey)
	{
		m_map[newKey] = index;
		m_map.remove(oldKey);
	}
	void replace(const Key &oldKey, const Key &newKey)
	{
		m_map[newKey] = m_map.take(oldKey);
	}
	T take(size_type index)
	{
		T res(m_list.at(index));
		remove(index);
		return res;
	}
	void clear()
	{
		m_list.clear();
		m_map.clear();
	}
	void deleteAll()
	{
		qDeleteAll(m_list);
	}

private:
	ValueList m_list;
	ValueMap m_map;
};

}

#endif /* EFC_ORDEREDMAP_H_ */
