#pragma once

#include <cstddef>
#include <algorithm>
#include <cassert>
#include <stdexcept>

#define SHORT_ARRAY_MAX_SIZE 16

template< typename T, std::size_t N >
class short_array{
private:
	char m_raw[N*sizeof(T)];
	T *ptr(){	return reinterpret_cast<T*>(m_raw);	}
	const T *ptr() const{	return reinterpret_cast<const T*>(m_raw);	}
public:
    typedef T              value_type;
    typedef T*             iterator;
    typedef const T*       const_iterator;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
	static const size_type static_size = N;

    iterator begin() { return ptr(); }
    const_iterator begin() const { return ptr(); }
    iterator end() { return ptr()+N; }
    const_iterator end() const { return ptr()+N; }

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

	size_type size(){	return N;		}

    reference operator[](size_type i) 
    { 
        assert( i < N ); 
        return ptr()[i];
    }
    
    const_reference operator[](size_type i) const 
    {     
        assert( i < N ); 
        return ptr()[i]; 
    }

    reference at(size_type i) { range_check(i); return ptr()[i]; }
    const_reference at(size_type i) const { range_check(i); return ptr()[i]; }

    reference front() { return ptr()[0]; }
    
    const_reference front() const { return ptr()[0]; }
    
    reference back() { return ptr()[N-1]; }
    
    const_reference back() const { return ptr()[N-1]; }

	const T *data() const { return ptr(); }
	T *data() { return ptr(); }
	
    template <typename R>
    const short_array<T,N> &operator= ( const short_array<R,N> &rhs ) {
        std::copy(rhs.begin(),rhs.end(), begin());
        return *this;
    }

	template <typename _InputIterator>
	short_array<T,N>& operator= ( _InputIterator rhs ){
		for(iterator i=begin();i!=end();++i){
			*i = *rhs++;
		}
		return *this;
	}

	//template <typename R>
	//short_array<T,N>& operator= ( const R *rhs ){
	//	for(iterator i=begin();i!=end();++i){
	//		*i = *rhs++;
	//	}
	//	return *this;
	//}

    void rangecheck (size_type i) {
        if (i >= size()) {
            std::out_of_range e("short_array<>: index out of range");
            throw(e);
        }
    }

	~short_array(){
		destruct(begin(),end());
	}



	short_array(){
		for(T *pL=ptr(); pL!=ptr()+N; ++pL){
			new (pL) T();
		}
	}

    template <typename R>
    explicit short_array( const short_array<R,N> &rhs ) {
		const R *pR = rhs.ptr();
		for(T *pL=ptr(); pL!=ptr()+N; ++pL){
			new (pL) T(*pR++);
		}
    }


	template <typename _InputIterator>
	explicit short_array( _InputIterator rhs ){
		for(T *pL=ptr(); pL!=ptr()+N; ++pL){
			new (pL) T(*rhs++);
		}
	}


	//template <typename R>
	//explicit short_array( const R * rhs ){
	//	for(T *pL=ptr(); pL!=ptr()+N; ++pL){
	//		new (pL) T(*rhs++);
	//	}
	//}


	//template <typename R>
	//explicit short_array( const R &rhs ){
	//	for(T *pL=ptr(); pL!=ptr()+N; ++pL){
	//		new (pL) T(rhs);
	//	}
	//}


	

		
private:
	static void destruct(T *first, T *last){
		for(T *p(first); p<last; ++p){
			p->~T();
		}
	}
};
