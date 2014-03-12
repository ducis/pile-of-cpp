/** @file */
//NEGATIVE INT

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "Utility.h"
#include "ArrayList.h"
#include "LinkedList.h"

template <class K, class V, class H>
class HashMap
{
friend class ConstIterator;
friend class Iterator;

public:
    class ConstIterator
    {
    public:
		friend class HashMap;

		ConstIterator (const HashMap & pp) : now(0), ptr(pp.Hash)
		{
			p1 ( ptr.constIterator()); 
			if ( p1.hasNext()) 
			{
				p2 ( p1.next.constIterator() );
			}
		}
        //O(n) for iterating through the container.
        bool hasNext() 
		{
			return now == ptr.size();
		}

        /* O(n) for iterating through the container.
         * @throw ElementNotExist */
        const Entry<K, V>& next() 
		{
			if ( !hasNext() ) throw ElementNotExist(" next ");
			now++;
			if (p2.hasNext()) return p2.next();
			while (p1.hasNext())
			{
				p2 ( p1.next().constIterator() );
				if (p2.hasNext()) return p2.next();
			}
		}
	private:
		int now;
		typename ArrayList < LinkedList < Entry <K, V> > >::ConstIterator p1;
		typename LinkedList < Entry <K, V> >::ConstIterator p2;
		const ArrayList< LinkedList <Entry <K, V> > > & ptr;
    };

    class Iterator
    {
    public:
		friend class HashMap;
	
		Iterator (HashMap & pp) : now(0), ptr(pp.Hash)
		{
			p1 ( ptr.iterator()); 
			if ( p1.hasNext()) 
			{
				p2 ( p1.next().iterator());
			}
		}
        /* O(n) for iterating through the container.*/
        bool hasNext() 
		{
			return now == ptr.size();
		}

        /* O(n) for iterating through the container.
         * @throw ElementNotExist */
        Entry<K, V>& next() 
		{
			if ( !hasNext() ) throw ElementNotExist(" next ");
			now++;
			if (p2.hasNext()) { LastReturn = p2.next(); return LastReturn ; }
			while (p1.hasNext())
			{
				p2 = p1.next().iterator();
				if (p2.hasNext()) { LastReturn = p2.next(); return LastReturn ; }
			}
		}

        /**
         * Removes from the underlying collection the last element returned by the iterator (optional operation).
         * O(1)
         * @throw ElementNotExist
         */
        void remove() 
		{


		}
	private:
		int now;
		typename ArrayList < LinkedList < Entry <K, V> > >::Iterator p1;
		typename LinkedList < Entry <K, V> >::Iterator p2;
		ArrayList< LinkedList <Entry <K, V> > > & ptr;
		Entry <K, V> LastReturn;
   
    };

    /**
     * Constructs an empty list with an initial capacity.
     * You can construct it with your own initial capacity.
     */
	HashMap() : capacity(2651333), sz(0), Hash(capacity)
	{
	}

    /**
     * Copy-constructor
     */
	HashMap(const HashMap& x) : capacity(x.capacity), sz(0), Hash(capacity)
	{
		for (typename HashMap::Iterator i( x.iterator() ); i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			put(tmp.key, tmp.value);
		}

	}

    /**
     * Constructs an empty HashMap with the specified initial capacity
     */
	HashMap(int initialCapacity) : capacity(initialCapacity), sz(0), Hash(capacity)
	{ 
	}

    /**
     * Constructs a new HashMap with the same mappings as the specified Map.
     */
    template <class E2>
    explicit HashMap(const E2 &m)
	{
		for (typename E2::Iterator i( m.iterator() ); i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			put(tmp.key, tmp.value);
		}
	}

    /**
     * assignment operator
     */
    HashMap& operator=(const HashMap& x) 
	{
		clear(); capacity=x.capacity;
		for (typename HashMap::Iterator i( x.iterator() ); i.hasNext(); )
		{
			Entry <K, V> tmp(i.next());
			put(tmp.key, tmp.value);
		}
		return *this;
	}

    /**
     * destructor
     */
    ~HashMap() 
	{
		clear();
	}

    /**
     * Returns an iterator over the elements in this map in proper sequence.
     */
    Iterator iterator() 
	{
		return Iterator(*this);
	}

    /**
     * Returns an CONST iterator over the elements in this map in proper sequence.
     */
    ConstIterator constIterator() const 
	{
		return ConstIterator(*this);
	}

    /**
     * Removes all of the mappings from this map.
     */
    void clear() 
	{
		Hash.clear(); sz=0;
	}

    /**
     * Returns true if this map contains a mapping for the specified key.
     * O(1) for average
     */
    bool containsKey(const K& key) const 
	{
		int hashkey = H::hashcode(key) % capacity ;

		for (typename LinkedList < Entry <K, V> >::ConstIterator i( Hash.get(hashkey).constIterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.key == key) return true; 
		}
		return false;
	}

    /**
     * Returns true if this map maps one or more keys to the specified value.
     * O(n)
     */
    bool containsValue(const V& value) const 
	{
		for (typename HashMap <K, V, H>::ConstIterator i( constIterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.value == value) return true; 
		}
		return false;
	}

    /**
     * Returns a reference to the value to which the specified key is mapped.
     * O(1) for average
     * @throw ElementNotExist
     */
    V& get(const K& key) 
	{
		int hashkey = H::hashcode(key) % capacity ;

		for (typename LinkedList < Entry <K, V> >::ConstIterator i( Hash.get(hashkey).constIterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.key == key) return tmp.value; 
		}
	
		throw ElementNotExist("get ");
	}

    /**
     * Returns a const reference to the value to which the specified key is mapped.
     * O(1) for average
     * @throw ElementNotExist
     */
    const V& get(const K& key) const 
	{ 
		int hashkey = H::hashcode(key) % capacity ;

		for (typename LinkedList < Entry <K, V> >::ConstIterator i( Hash.get(hashkey).constIterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.key == key) return tmp.value; 
		}
	
		throw ElementNotExist("get ");
	}

    /**
     * Returns true if this map contains no key-value mappings.
     * O(1)
     */
    bool isEmpty() const 
	{
		return sz==0;
	}

    /**
     * Associates the specified value with the specified key in this map.
     * Returns the previous value, if not exist, a value returned by the default-constructor.
     * O(1)
     */
    V put(const K& key, const V& value) 
	{
		V val;
		int hashkey = H::hashcode(key) % capacity ;

		for (typename LinkedList < Entry <K, V> >::Iterator i( Hash.get(hashkey).iterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.key == key) 
			{
				val = tmp.value;
				tmp.value = value;
				i.set( tmp );// to set the last element returned by the iterator @LinkedList.iterator TODO
				return val;
			}
		}
	
		Entry <K, V> tmp(key, value);
		Hash.get(key).add( tmp );
		return val;

	}

    /**
     * Removes the mapping for the specified key from this map if present.
     * Returns the previous value.
     * O(1) for average
     * @throw ElementNotExist
     */
    V remove(const K& key) 
	{
		V val;
		int hashkey = H::hashcode(key) % capacity ;

		for (typename LinkedList < Entry <K, V> >::Iterator i( Hash.get(hashkey).iterator() );
			i.hasNext(); )
		{
			Entry <K, V> tmp( i.next() );
			if (tmp.key == key) 
			{
				val = tmp.value;
				i.remove();// to set the last element returned by the iterator @LinkedList.iterator TODO
				return val;
			}
		}

		throw ElementNotExist("remove ");
		return val;

	}

    /**
     * Returns the number of key-value mappings in this map.
     * O(1)
     */
    int size() const 
	{
		return sz;
	}

private:

	int capacity, sz;
	ArrayList< LinkedList< Entry<K, V> > > Hash;

};
#endif
