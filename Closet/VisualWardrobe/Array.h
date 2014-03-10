#pragma once
#include <stddef.h>

//C式静态数组的Wrapper,因为Boost中有更好的所以不建议继续使用

template<typename T, size_t _Size>
class UninitializedArray{
public:
	typedef T EleType;
	T &operator[](size_t i){
		return reinterpret_cast<T*>(m_mem)[i];
	}
	template<size_t _SubSize> T &As2D(size_t i, size_t j){
		return operator[]( i*_SubSize+j );
	}
	const T &operator[](size_t i) const{
		return reinterpret_cast<const T*>(m_mem)[i];
	}
	T *PtrBegin(){
		return reinterpret_cast<T*>(m_mem);
	}
	const T *PtrBegin() const{
		return reinterpret_cast<const T*>(m_mem);
	}
	T *PtrEnd(){
		return PtrBegin()+_Size;
	}
	const T *PtrEnd() const{
		return PtrBegin()+_Size;
	}
	void DtorAll(){
		T *p(PtrEnd());
		while(p>PtrBegin()){
			--p;
			p->~T();
		}
	}
	inline void Ctor(size_t i){ new (PtrBegin()+i) T; }
	template<typename A0> 
		inline void Ctor(size_t i, A0 a0){ 
			new (PtrBegin()+i) T(a0); }
	template<typename A0,typename A1> 
		inline void Ctor(size_t i, A0 a0, A1 a1){ 
			new(PtrBegin()+i) T(a0,a1); }
	template<typename A0,typename A1,typename A2> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2){ 
			new(PtrBegin()+i) T(a0,a1,a2); }
	template<typename A0,typename A1,typename A2,typename A3> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2, A3 a3){ 
			new(PtrBegin()+i) T(a0,a1,a2,a3); }
	template<typename A0,typename A1,typename A2,typename A3,typename A4> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4){ 
			new(PtrBegin()+i) T(a0,a1,a2,a3,a4); }
	template<typename A0,typename A1,typename A2,typename A3,typename A4,typename A5> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5){ 
			new(PtrBegin()+i) T(a0,a1,a2,a3,a4,a5); }
	template<typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6){ 
			new(PtrBegin()+i) T(a0,a1,a2,a3,a4,a5,a6); }
	template<typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7> 
		inline void Ctor(size_t i, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7){ 
			new(PtrBegin()+i) T(a0,a1,a2,a3,a4,a5,a6,a7); }
protected:
	unsigned char m_mem[sizeof(T)*_Size];
};


template<typename T, size_t _Size> 
class FixedArrayBase:private UninitializedArray<T,_Size>{
public:
	using UninitializedArray<T,_Size>::EleType;
	using UninitializedArray<T,_Size>::operator[];
	using UninitializedArray<T,_Size>::As2D;
	using UninitializedArray<T,_Size>::PtrBegin;
	using UninitializedArray<T,_Size>::PtrEnd;
	~FixedArrayBase(){
		DtorAll();
	}
};

template<typename T, size_t _Size> class FixedArray;

template<typename T>
class FixedArray<T,2>:public FixedArrayBase<T,2>{
public:
	FixedArray( const T &a, const T &b ){
		T *p(PtrBegin());
		new (p++) T(a);
		new (p++) T(b);
	}
	FixedArray(){
		T *p(PtrBegin());
		new (p++) T;
		new (p++) T;
	}
};

template<typename T>
class FixedArray<T,3>:public FixedArrayBase<T,3>{
public:
	FixedArray( const T &a, const T &b, const T &c ){
		T *p(PtrBegin());
		new (p++) T(a);
		new (p++) T(b);
		new (p++) T(c);
	}
	FixedArray(){
		T *p(PtrBegin());
		new (p++) T;
		new (p++) T;
		new (p++) T;
	}
};

template<typename T>
class FixedArray<T,4>:public FixedArrayBase<T,4>{
public:
	FixedArray( const T &a, const T &b, const T &c, const T &d ){
		T *p(PtrBegin());
		new (p++) T(a);
		new (p++) T(b);
		new (p++) T(c);
		new (p++) T(d);
	}
	FixedArray(){
		T *p(PtrBegin());
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
	}
};


template<typename T>
class FixedArray<T,5>:public FixedArrayBase<T,5>{
public:
	FixedArray( const T &a, const T &b, const T &c, const T &d, const T &e ){
		T *p(PtrBegin());
		new (p++) T(a);
		new (p++) T(b);
		new (p++) T(c);
		new (p++) T(d);
		new (p++) T(e);
	}
	FixedArray(){
		T *p(PtrBegin());
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
	}
};


template<typename T>
class FixedArray<T,6>:public FixedArrayBase<T,6>{
public:
	FixedArray( const T &a, const T &b, const T &c, const T &d, const T &e, const T &f ){
		T *p(PtrBegin());
		new (p++) T(a);
		new (p++) T(b);
		new (p++) T(c);
		new (p++) T(d);
		new (p++) T(e);
		new (p++) T(f);
	}
	FixedArray(){
		T *p(PtrBegin());
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
		new (p++) T;
	}
};


template<typename T, size_t _Size1, size_t _Size2>
class FixedArray2:public FixedArray<T,_Size1*_Size2>{};