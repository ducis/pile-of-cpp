/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "Utility.h"

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T> class LinkedList {
private:
	struct Node;
public:
    class ConstIterator {
        friend class LinkedList;
		friend class Iterator;
    public:
        /**
         * Returns true if the iteration has more elements.
         * O(1).
         */
        bool hasNext() const{
			return NULL!=m_pNode;
		}

        /**
         * Returns the next element in the iteration.
         * O(1).
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
			assert(hasNext());
			if(!hasNext())	throw ElementNotExist();
			const T &ret(m_pNode->obj);
			m_pNode = m_pNode->next;
			return ret;
		}
		~ConstIterator(){
			--m_cont.m_locks;
		}
		ConstIterator(const ConstIterator &itr):m_pNode(itr.m_pNode),m_cont(itr.m_cont){
			++m_cont.m_locks;
		}
	private:
		ConstIterator(const LinkedList &cont, const Node *pNode):m_cont(cont),m_pNode(pNode){
			++m_cont.m_locks;
		}
		const Node *m_pNode;
		const LinkedList &m_cont;
    };

    class Iterator {
        friend class LinkedList;
    public:
        /**
         * Returns true if the iteration has more elements.
         * O(1).
         */
        bool hasNext() const{
			return m_wait ? NULL!=*m_ppPrevNode : NULL!=(*m_ppPrevNode)->next ;
		}

        /**
         * Returns the next element in the iteration.
         * O(1).
         * @throw ElementNotExist exception when hasNext() == false
         */
        T& next() {
			assert(hasNext());
			if(m_wait){
				if(NULL==*m_ppPrevNode) throw ElementNotExist();
				m_wait=false;
				return (*m_ppPrevNode)->obj;
			}else{
				if(NULL==(*m_ppPrevNode)->next) throw ElementNotExist();
				T &ret((*m_ppPrevNode)->next->obj);
				m_ppPrevNode = &(*m_ppPrevNode)->next;
				return ret;
			}
		}

        /**
         * Removes from the underlying collection the last element
         * returned by the iterator
         * O(1).
         */
        void remove() {
			assert(m_ppPrevNode);
			assert(!m_wait);
			TemporaryUnlocker<unsigned int> unlocker(m_cont.m_locks);
			m_cont.removeInsertionPP(m_ppPrevNode);
			m_wait = true;
		}
		~Iterator(){
			--m_cont.m_locks;
		}
		Iterator(const Iterator &itr):m_ppPrevNode(itr.m_ppPrevNode),m_cont(itr.m_cont),m_wait(itr.m_wait){
			++m_cont.m_locks;
		}
	private:

		Iterator(LinkedList &cont, Node **ppNode):m_cont(cont),m_ppPrevNode(ppNode),m_wait(true){
			++m_cont.m_locks;
		}

		bool m_wait;
		Node **m_ppPrevNode;
		LinkedList &m_cont;
    };

    /**
     * Constructs an empty list
     */
	LinkedList():m_pair(NULL),m_size(0),m_locks(0){}

    /**
     * Copy constructor
     * You may utilize the ``addAll'' function from Utility.h
     */
	LinkedList(const LinkedList<T> &c):m_pair(copy_chain(c.m_pair.pBegin,NULL)),m_size(c.m_size),m_locks(0) {
	}

    /**
     * Assignment operator
     * You may utilize the ``addAll'' function from Utility.h
     */
	LinkedList<T>& operator=(const LinkedList<T> &c){
		destroy_chain(m_pair.pBegin);
		m_pair = copy_chain(c.m_pair.pBegin,NULL);
		m_size = c.m_size;
		return *this;
	}

    /**
     * Constructs a list containing the elements of the specified
     * collection, in the order they are returned by the collection's
     * const iterator.
     * You may utilize the ``addAll'' function from Utility.h
     */
	template <class C> LinkedList(const C& c):m_pair(copy_seq(c.constIterator())),m_size(c.m_size),m_locks(0) {
	}

    /**
     * Desturctor
     */
    ~LinkedList() {
		assert(0==m_locks);
		destroy_chain(m_pair.pBegin);
	}

    /**
     * Inserts the specified element at the specified position in
     * this list.
     * O(n)
     * @throw IndexOutOfBound exception when index is out of bound
     */
    void add(int index, const T& elem) {
		assert(0==m_locks);
		assert(index>=0&&index<=m_size);
		if(index<0||index>m_size) throw IndexOutOfBound();
		insertInsertionPP(get_nth_insertion_pp(&m_pair.pBegin,index),elem);
	}

    /**
     * Appends the specified element to the end of this list.
     * O(1).
     * Always returns true;
     */
    bool add(const T& elem) {
		assert(0==m_locks);
		insert_by_insertion_pp(m_pair.ppEnd,elem);
		m_pair.ppEnd = &(*m_pair.ppEnd)->next;
		++m_size;
		return true;
	}

    /**
     * Inserts the specified element at the beginning of this list.
     * O(1).
     */
    void addFirst(const T& elem) {
		assert(0==m_locks);
		insertInsertionPP(&m_pair.pBegin,elem);
	}

    /**
     * Removes all of the elements from this list.
     * O(n).
     */
    void clear() {
		assert(0==m_locks);
		if(m_size){
			destroy_chain(m_pair.pBegin);
			m_pair.pBegin = NULL;
			m_pair.ppEnd = &m_pair.pBegin;
			m_size = 0;
		}
	}

    /**
     * Returns true if this list contains the specified element.
     * O(n).
     */
    bool contains(const T& elem) const {
		for(Node *p(m_pair.pBegin); p; p=p->next){
			if(elem == p->obj)	return true;
		}
		return false;
	}

    /**
     * Returns a reference to the element at the specified position.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T& get(int index) {
		return const_cast<T&>(const_cast<const LinkedList*>(this)->get(index));
	}

    /**
     * Returns a const reference to the element at the specified position.
     * O(n).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
		assert(index>=0);
		assert(index<m_size);
		if(index<0||index>=m_size) throw IndexOutOfBound();
		return get_nth(m_pair.pBegin,index)->obj;
	}

    /**
     * Returns a reference to the first element.
     * O(1).
     * @throw ElementNotExist
     */
    T& getFirst() {
		return const_cast<T&>(const_cast<const LinkedList*>(this)->getFirst());
	}

    /**
     * Returns a const reference to the first element.
     * O(1).
     * @throw ElementNotExist
     */
    const T& getFirst() const {
		assert(m_pair.pBegin);
		if(!m_pair.pBegin)	throw ElementNotExist();
		return m_pair.pBegin->obj;
	}

    /**
     * Returns a reference to the last element.
     * O(1).
     * @throw ElementNotExist
     */
    T& getLast() {
		return const_cast<T&>(const_cast<const LinkedList*>(this)->getLast());
	}

    /**
     * Returns a const reference to the last element.
     * O(1).
     * @throw ElementNotExist
     */
    const T& getLast() const {
		assert(m_size);
		if(0==m_size){
			throw ElementNotExist();
		}
		return get(m_size-1);
	}

    /**
     * Returns the index of the first occurrence of the specified element
     * O(1).
     * in this list, or -1 if this list does not contain the element.
     */
    int indexOf(const T& elem) const {
		int i(0);
		for(Node *p(m_pair.pBegin); p; p=p->next){
			if(elem==p->obj)	return i;
			++i;
		}
		return -1;
	}

    /**
     * Returns true if this list contains no elements.
     * O(1).
     */
    bool isEmpty() const {	return 0==m_size;	}

    /**
     * Returns an iterator
     * O(1).
     */
    Iterator iterator() {	return Iterator(*this,&m_pair.pBegin);  }

    /**
     * Returns an const iterator
     * O(1).
     */
    ConstIterator constIterator() const {  return ConstIterator(*this,m_pair.pBegin); }

    /**
     * Removes the element at the specified position in this list.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T removeIndex(int index) {
		assert(0==m_locks);
		assert(index>=0&&index<m_size);
		if(index<0||index>=m_size){
			throw IndexOutOfBound();
		}
		Node **pp = get_nth_insertion_pp(&m_pair.pBegin, index);
		T ret((*pp)->obj);
		removeInsertionPP(pp);
		return ret;
	}

    /**
     * Removes the first occurrence of the specified element from this
     * O(n).
     * list, if it is present.
     */
    bool remove(const T& elem) {
		assert(0==m_locks);
		Node **pp( get_insertion_pp_by_value(&m_pair.pBegin, elem) );
		if(*pp){
			removeInsertionPP(pp);
			return true;
		}
		return false;
	}

    /**
     * Removes and returns the first element from this list.
     * O(n).
     * @throw ElementNotExist
     */
    T removeFirst() {
		assert(0==m_locks);
		assert(m_size);
		if(!m_size){
			throw ElementNotExist();
		}
		T ret(m_pair.pBegin->obj);
		removeInsertionPP(&m_pair.pBegin);
		return ret;
	}

    /**
     * Removes and returns the last element from this list.
     * O(n).
     * @throw ElementNotExist
     */
    T removeLast() {
		assert(0==m_locks);
		assert(m_size);
		if(!m_size)		throw ElementNotExist();
		return removeIndex(m_size-1);
	}

    /**
     * Replaces the element at the specified position in this list with
     * the specified element.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T& set(int index, const T& elem) {
		//TODO: Should I use assignment semantic or destruction/copy-construction semantic?
		//Right now destruction/copy-construction is chosen because copy-ctors are needed elsewhere.
		assert(0==m_locks);
		assert(index>=0&&index<m_size);
		if(index<0||index>=m_size) throw IndexOutOfBound();
		Node *p( get_nth(m_pair.pBegin,index) );
		p->obj.~T();
		return *( new (&p->obj) T(elem) );
	}

    /**
     * Returns the number of elements in this list.
     */
    int size() const {	return m_size;  }

    /**
     * Returns a view of the portion of this list between the specified
     * fromIndex, inclusive, and toIndex, exclusive.
     * O(n).
     */
	LinkedList<T> subList(int fromIndex, int toIndex){
		assert(fromIndex>=0);
		assert(toIndex<=m_size);
		assert(toIndex>fromIndex);
		if(fromIndex<0||toIndex>m_size||toIndex<fromIndex){
			throw IndexOutOfBound();
		}
		return LinkedList<T>(copy_chain(*get_nth_insertion_pp(&m_pair.pBegin,fromIndex),*get_nth_insertion_pp(&m_pair.pBegin,toIndex)),toIndex-fromIndex);
	}
private:
	struct Node{
		T obj;
		Node *next;
		explicit Node( const T &_obj ):obj(_obj){}
		Node (const T &_obj, Node *_next):obj(_obj),next(_next){}
	};
	struct Pair{
		Node *pBegin;
		Node **ppEnd;
		explicit Pair( Node *_pBegin ):pBegin(_pBegin),ppEnd(&pBegin){}
		void non_sealing_add( const T &obj ){//Caution: This proc leaves (*ppEnd) undefined. Call seal() after one or a series of non_sealing_add()'s.
			*ppEnd = new Node(obj);
			ppEnd = &(*ppEnd)->next;
		}
		void seal(){
			*ppEnd = NULL;
		}
	};
	Pair m_pair;
	size_t m_size;
	mutable unsigned int m_locks;

	LinkedList(const Pair &pair, size_t sz):m_locks(0),m_pair(pair),m_size(sz){}

	void removeInsertionPP( Node **pp ){
		remove_by_insertion_pp(pp);
		if(NULL==*pp){
			m_pair.ppEnd = pp;
		}
		--m_size;
	}
	void insertInsertionPP( Node **pp ){
		insert_by_insertion_pp(pp);
		if(m_pair.ppEnd == pp){
			m_pair.ppEnd = &(*pp)->next;
		}
		++m_size;
	}

	static void remove_by_insertion_pp(Node **pp){
		Node *t(*pp);
		*pp = (*pp)->next;
		delete t;
	}
	static void insert_by_insertion_pp(Node **pp,const T &val){
		*pp = new Node(val,*pp);
	}

	static Node **get_nth_insertion_pp(Node **pp,size_t n){
		for(;n>0;--n){
			pp=&(*pp)->next;
		}
		return pp;
	}
	static Node **get_insertion_pp_by_value(Node **pp, const T &val){
		while( *pp && val!=(*pp)->obj ){
			pp = &(*pp)->next;
		}
		return pp;
	}
	static const Node *get_nth(const Node *p, size_t n){
		for(;n>0;--n){
			p = p->next;
		}
		return p;
	}
	static Node *get_nth(Node *p, size_t n){
		return const_cast<Node *>(get_nth(const_cast<const Node *>(p),n));
	}

	static void destroy_chain( Node *begin ){
		Node *p(begin);
		while(p){
			Node *t(p);
			p = p->next;
			delete t;
		}
	}

	static void copy_chain( const Node *begin, const Node *end, Pair &ret ){
		for(const Node *pc(begin);pc!=end;pc = pc->next){
			ret.non_sealing_add(pc->obj);
		}
		ret.seal();
	}

	static const Pair copy_chain( const Node *begin, const Node *end ){
		Pair ret(NULL);
		copy_chain(begin,end,ret);
		return ret;
	}

	template<typename _Itr>
	static void copy_seg( _Itr itr, Pair &ret ){
		while(itr.hasNext()){
			ret.non_sealing_add(itr.next());
		}
		ret.seal();
	}
	template<typename _Itr>
	static const Pair copy_seg( const _Itr &itr ){
		Pair ret(NULL);
		copy_seg( itr, ret );
		return ret;
	}
};
#endif

