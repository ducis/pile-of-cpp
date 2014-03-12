/** @file */
#ifndef __HASHSET_H
#define __HASHSET_H

#include "Utility.h"
#include "HashMap.h"

/**
 * A HashSet holds elements using a hash table, providing facilities
 * to insert, remove an element into the container and search an
 * element within the container efficiently.
 *
 * We don't require an order in the iteration, but you should
 * guarantee all elements will be iterated.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashcode'',
 * which takes a parameter of type T and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashcode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashSet<int, Hashint> hash;
 * @endcode
 */
template <class T, class H> class HashSet {
public:
    class ConstIterator {
    public:
		ConstIterator (HashSet &hashset) : ptr( hashset.Hash.constIterator() ){}
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container
         */
        bool hasNext() 
		{
			return ptr.hasNext();
		}

        /**
         * Returns the next element in the iteration.
         * O(n) for iterating through the container.
         * @throw ElementNotExist
         */
        const T& next() 
		{
			if ( !hasNext() ) throw ElementNotExist("next ");
			return ptr.next().key;
		}

	private:
		typename HashMap<T, T, H>::ConstIterator ptr;
    };

    class Iterator {
    public:
		Iterator (HashSet &hashset) : ptr( hashset.Hash.iterator() ){}
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container.
         */
        bool hasNext() 
		{
			return ptr.hasNext();
		}

        /**
         * Returns the next element in the iteration.
         * O(n) for iterating through the container
         * @throw ElementNotExist
         */
        const T& next() 
		{
			if ( !hasNext() ) throw ElementNotExist("next ");
			return ptr.next().key;
		}

        /**
         * Removes from the underlying collection the last element
         * returned by the iterator.
         * O(1)
         * @throw ElementNotExist
         */
        void remove() {}
	private:
		typename HashMap<T, T, H>::Iterator ptr;
    };

    /**
     * Constructs a empty set with your own default capacity
     */
	HashSet() : Hash()
	{

	}

    /**
     * Destructor
     */
    ~HashSet() 
	{ 
	}

    /**
     * Copy constructor
     */
	HashSet(const HashSet &c) : Hash(c.Hash)
	{

	}

    /**
     * Assignment operator
     */
    HashSet& operator=(const HashSet &c) 
	{
		Hash=c.Hash;
	}

    /**
     * Constructs a new set containing the elements in the specified
     * collection.
     */
	template<class C> explicit HashSet(const C& c) :Hash(c)
	{

	}

    /**
     * Constructs a new, empty set; the backing HashMap instance has the
     * specified capacity
     */
	HashSet(int capacity) :Hash(capacity)
	{

	}

    /**
     * Adds the specified element to this set if it is not already present.
     * Returns false if element is previously in the set.
     * O(1) for average
     */
    bool add(const T& elem) 
	{
		if ( Hash.containsKey( elem) ) return false;
		Hash.put( elem, elem);
		return true;
	}

    /**
     * Removes all of the elements from this set.
     */
    void clear() 
	{
		Hash.clear();
	}

    /**
     * Returns true if this set contains the specified element.
     * O(1) for average
     */
    bool contains(const T& elem) const 
	{
		return Hash.containsKey( elem );
	}

    /**
     * Returns true if this set contains no elements.
     * O(1)
     */
    bool isEmpty() const 
	{
		return Hash.isEmpty();
	}

    /**
     * Returns an iterator over the elements in this set.
     */
    Iterator iterator() 
	{
		return Iterator(*this);
	}

    /**
     * Returns an const iterator over the elements in this set.
     */
    ConstIterator constIterator() const 
	{
		return ConstIterator(*this);
	}

    /**
     * Removes the specified element from this set if it is present.
     * O(1) for average
     */
    bool remove(const T& elem) 
	{
		if ( ! Hash.containsKey( elem ) ) return false;
		Hash.remove(elem);
		return true;
	}

    /**
     * Returns the number of elements in this set (its cardinality).
     * O(1)
     */
    int size() const 
	{
		return Hash.size();
	}

private:
	HashMap <T, T, H> Hash;

};

#endif

