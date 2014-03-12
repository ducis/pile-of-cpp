/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "Utility.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, it means
 * the length of the array of your inner implemention
 * For example, even if the capacity is 10, the method "isEmpty()" may still return true.
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class E>
class ArrayList
{
public:

    class Iterator;
    class ConstIterator
    {
		friend class Iterator;
		friend class ArrayList;
    public:
        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() {
			return m_cur<m_container.endPtr();
		}

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        const E& next() {
			if(!hasNext())	throw ElementNotExist();
			return *m_cur++;
		}
		ConstIterator( const ConstIterator &itr ):m_container(itr.m_container), m_cur(itr.m_cur){
		    ++m_container.m_locks;
		}
		~ConstIterator(){
			--m_container.m_locks;
		}
	private:
		ConstIterator( const ArrayList &cont, size_t ind ):m_container(cont), m_cur(cont.m_storage.ptr()+ind){
			++m_container.m_locks;
		}
		const ArrayList &m_container;
		const E *m_cur;
    };

    class Iterator
    {
		friend class ArrayList;
    public:
        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() { return m_impl.hasNext(); }

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        E& next() { return const_cast<E&>(m_impl.next()); }

        /**
         * Removes from the underlying collection the last element returned by the iterator (optional operation).
         * O(1)
         * @throw ElementNotExist
         */
        void remove() {
			if(
				m_impl.m_cur<=m_impl.m_container.m_storage.ptr() ||
				m_impl.m_cur>m_impl.m_container.endPtr()
				)
			{
				throw ElementNotExist();
			}
			TemporaryUnlocker<unsigned int> unlocker(m_impl.m_container.m_locks);
			const_cast<ArrayList &>(m_impl.m_container).removeIndex( (--m_impl.m_cur)-m_impl.m_container.m_storage.ptr() );
		}

		Iterator( const Iterator &itr ):m_impl(itr.m_impl){}

	private:
		Iterator( ArrayList &cont, size_t ind ):m_impl( cont, ind ){
		}
		ConstIterator m_impl;
    };

    /**
     * Constructs an empty list with an initial capacity of ten.
     */
	ArrayList():m_locks(0),m_storage(10),m_size(0) { }////////////////////////

    /**
     * Constructs a list containing the elements of the specified collection, in
     * the order they are returned by the collection's iterator.
     */
    template <class E2>
	explicit ArrayList(const E2& x):m_locks(0),m_storage(x.size()),m_size(0) { //////////////////////
		for( typename E2::ConstIterator i( x.constIterator() );	i.hasNext();	new (m_storage.ptr()+(m_size++)) E(i.next()) )
			;
	}

    /**
     * Constructs an empty list with the specified initial capacity.
     */
	explicit ArrayList(int initialCapacity):m_locks(0),m_storage(initialCapacity),m_size(0) { }/////////////

    /**
     * Destructor
     */
    ~ArrayList() {
		assert(0==m_locks);
		destruct(m_storage.ptr(),m_size);
	}////////////

    /**
     * Assignment operator
     */
    ArrayList& operator=(const ArrayList& x) {//////////////
		assert(0==m_locks);
		destruct(m_storage.ptr(),m_size);
		ensureCapacity(x.size());
		m_size = x.size();
		deep_copy_to_raw(m_storage.ptr(),x.m_storage.ptr(),m_size);
		return *this;
	}

    /**
     * Copy-constructor
     */
	ArrayList(const ArrayList& x):m_locks(0),m_storage(x.size()),m_size(x.size()) {
		deep_copy_to_raw(m_storage.ptr(),x.m_storage.ptr(),m_size);
	}//////////////////

    /**
     * Returns an iterator over the elements in this list in proper sequence.
     */
    Iterator iterator() { return Iterator( *this,0 ); }////////////

    /**
     * Returns an CONST iterator over the elements in this list in proper sequence.
     */
    ConstIterator constIterator() const { return ConstIterator( *this,0 ); }///////////////

    /**
     * Appends the specified element to the end of this list.
     * O(1)
     */
    bool add(const E& e) {
		assert(0==m_locks);
		ensureCapacity(m_size+1);
		new (m_storage.ptr()+(m_size++)) E(e);
	}

    /**
     * Inserts the specified element at the specified position in this list.
     * The range of index is [0, size].
     * O(n)
     * @throw IndexOutOfBound
     */
    void add(int index, const E& element) {
		assert(0==m_locks);
		assert(index>=0);
		assert(index<=m_size);
		if(index<0||index>m_size){
			throw IndexOutOfBound();
		}
		ensureCapacity(m_size+1);
		E *pToInsert( m_storage.ptr()+index );
		for(E *p( m_storage.ptr()+m_size ); p>pToInsert; --p){
			//Avoid assignment operators.
			//Rationale:
			//To make the container compatible with elements without default ctors, copy-ctors must be used.
			//Since copy-ctors must be used anyway, usage of assignment operators is avoided.
			new (p) E(*(p-1));
			(p-1)->~E();
		}
		++m_size;
		new (pToInsert) E(element);
	}

    /**
     * Removes all of the elements from this list.
     */
    void clear() {
		assert(0==m_locks);
		destruct(m_storage.ptr(),m_size);
		m_size = 0;
	}

    /**
     * Returns true if this list contains the specified element.
     * O(n)
     */
    bool contains(const E& e) const {
		for(const E *p(m_storage.ptr()); p<m_storage.ptr()+m_size; ++p){
			if(e==*p){
				return true;
			}
		}
		return false;
	}

    /**
     * Increases the capacity of this ArrayList instance, if necessary, to ensure that it can hold at least the number of elements specified by the minimum capacity argument.
     */
    void ensureCapacity(int minCapacity) {
		assert(minCapacity>=0);
		assert(0==m_locks);
		if(m_storage.length()<minCapacity){
			size_t s(m_storage.length());
			while( s<(unsigned(minCapacity)) )	s<<=1;
			E *prev(m_storage.ptr());//char *prevRaw(m_storage.RawPtr());//Because delete[] accepts void* , it should be safe to delete type-casted pointers.
			new (&m_storage) Storage(s);//Not destructing the previous Storage to delay the freeing of the allocated memory.
			deep_copy_to_raw(m_storage.ptr(),prev,m_size);
			destruct(prev,m_size);
			delete[] prev;
		}
	}

    /**
     * Returns a reference to the element at the specified position in this list.
     * O(1)
     * @throw IndexOutOfBound
     */
    E& get(int index) {
		return const_cast<E&>(const_cast<const ArrayList *>(this)->get(index));
	}

    /**
     * Returns a const reference to the element at the specified position in this list.
     * O(1)
     * @throw IndexOutOfBound
     */
    const E& get(int index) const {
		assert(index>=0);
		assert(index<m_size);
		if(index<0||index>=m_size){
			throw IndexOutOfBound();
		}
		return *(m_storage.ptr()+index);
	}

    /**
     * Returns the index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element.
     * O(n)
     */
    int indexOf(const E& e) const {
		for(const E *p(m_storage.ptr()); p<m_storage.ptr()+m_size; ++p){
			if(e==*p){
				return p-m_storage.ptr();
			}
		}
		return -1;
	}

    /**
     * Returns true if this list contains no elements.
     * O(1)
     */
    bool isEmpty() const {
		return 0==m_size;
	}

    /**
     * Returns the index of the last occurrence of the specified element in this list, or -1 if this list does not contain the element.
     * O(n)
     */
    int lastIndexOf(const E& e) const {
		for(const E *p(m_storage.ptr()+m_size);p>m_storage.ptr();){
			if(e==*(--p)){
				return p-m_storage.ptr();
			}
		}
		return -1;
	}

    /**
     * Removes the element at the specified position in this list.
     * Returns the element that was removed from the list.
     * O(n)
     * @throw IndexOutOfBound
     */
    E removeIndex(int index) {
		assert(0==m_locks);
		assert(index>=0);
		assert(index<m_size);
		if(index<0||index>=m_size){
			throw IndexOutOfBound();
		}
		--m_size;
		E *p(m_storage.ptr()+index);
		p->~E();
		for(; p<m_storage.ptr()+m_size; ++p){
			//Avoid assignment operators.
			//Rationale:	See implementation of add().
			new (p) E(*(p+1));
			(p+1)->~E();
		}
	}

    /**
     * Removes the first occurrence of the specified element from this list, if it is present.
     * O(n)
     */
    bool remove(const E& e) {
		int ind(indexOf(e));
		if(ind>=0){
			removeIndex(ind);
			return true;
		}else{
			return false;
		}
	}

    /**
     * Removes from this list all of the elements whose index is between fromIndex, inclusive, and toIndex, exclusive.
     * O(n)
     * @throw IndexOutOfBound
     */
    void removeRange(int fromIndex, int toIndex) {
		assert(fromIndex>=0);
		assert(toIndex<=m_size);
		if(fromIndex<0||toIndex>m_size){
			throw IndexOutOfBound();
		}
		if(toIndex<=fromIndex)	return;
		assert(0==m_locks);
		destruct(m_storage.ptr()+fromIndex, m_storage.ptr()+toIndex);
		for(E *pl(m_storage.ptr()+fromIndex),pr(m_storage.ptr()+toIndex);pr<m_storage.ptr()+m_size;++pl,++pr){
			new	(pl) E(*pr);
			pr->~E();
		}
		m_size -= toIndex-fromIndex;
	}

    /**
     * Replaces the element at the specified position in this list with the specified element.
     * Returns the element previously at the specified position.
     * O(1)
     * @throw IndexOutOfBound
     */
    E set(int index, const E& element) {
		assert(index>=0);
		assert(index<m_size);
		if(index<0||index>=m_size)	throw IndexOutOfBound();
		E *p(m_storage.ptr()+index);
		E ret(*p);
		p->~E();
		new (p) E(element);
		return ret;
	}

    /**
     * Returns the number of elements in this list.
     * O(1)
     */
    int size() const {	return m_size; }

    /**
     * Returns a view of the portion of this list between the specified fromIndex, inclusive, and toIndex, exclusive.
     * O(n)
     * @throw IndexOutOfBound
     */
    ArrayList subList(int fromIndex, int toIndex) const {
		assert(fromIndex>=0);
		assert(toIndex<=m_size);
		if(fromIndex<0||toIndex>m_size){
			throw IndexOutOfBound();
		}
		if(toIndex<=fromIndex)	return;
		ArrayList ret(toIndex-fromIndex);
		ret.m_size = toIndex-fromIndex;
		deep_copy_to_raw( ret.m_storage.ptr()+fromIndex, m_storage.ptr()+fromIndex, toIndex-fromIndex );
		return ret;
	}
private:
    struct Storage{//To account for classes without default ctors.
	private:
		char *m_pBytes;
		size_t m_length;
		static char *alloc(size_t length){
			return new char[get_size(length)];
		}
		static size_t get_size(size_t length){
			return length*sizeof(E);
		}
		//No copy ctor or assignment since how many E's are constructed is unknown within Storage.
		Storage(const Storage &st);
		const Storage &operator = (const Storage &st);
	public:
		explicit Storage(size_t length):m_length(length),m_pBytes(alloc(length)){}
		~Storage(){		delete[] m_pBytes;		}
		size_t length() const{		return m_length;	}
		E *ptr(){	return reinterpret_cast<E*>(m_pBytes);	}
		const E *ptr() const{	return reinterpret_cast<const E*>(m_pBytes);	}
		//char *RawPtr(){		return m_pBytes;	}
    };
	Storage m_storage;
	mutable unsigned int m_locks;
	size_t m_size;

	const E *endPtr() const{
		return m_storage.ptr()+m_size;
	}
	E *endPtr(){
		return const_cast<E*>(const_cast<const ArrayList *>(this)->endPtr);
	}

	static void deep_copy_to_raw(E *dest, const E *src, size_t count){
		for(E *d(dest); d<dest+count; ++d){
			new (d) E(*src);
			++src;
		}
	}
	static void destruct(E *first, E *last){
		for(E *p(first); p<last; ++p){
			p->~E();
		}
	}
	static void destruct(E *first, size_t n){
		destruct(first,first+n);
	}
};
#endif

