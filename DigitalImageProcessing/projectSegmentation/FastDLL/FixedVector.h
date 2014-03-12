#pragma once

#include <cstdlib>
#include <algorithm>

template<typename _S,size_t _Len> 
struct FixedVector{
	typedef _S scalar_type;
	typedef FixedVector<_S,_Len> self_type;

	friend const self_type operator*(const scalar_type &lhs, const self_type &rhs){
		return rhs*lhs;
	}
	friend const self_type operator*(const self_type &lhs, const scalar_type &rhs){
		self_type ret(lhs);
		return ret*=rhs;
	}
	friend const self_type operator+(const scalar_type &lhs, const self_type &rhs){
		return rhs+lhs;
	}
	friend const self_type operator+(const self_type &lhs, const scalar_type &rhs){
		self_type ret(lhs);
		return ret+=rhs;
	}
	friend const self_type operator/(const self_type &lhs, const scalar_type &rhs){
		self_type ret(lhs);
		return ret/=rhs;
	}
	friend const self_type operator-(const self_type &lhs, const scalar_type &rhs){
		self_type ret(lhs);
		return ret-=rhs;
	}
	const self_type &operator*=(const scalar_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]*=rhs;
		}
		return *this;
	}
	const self_type &operator*=(const self_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]*=rhs[i];
		}
		return *this;
	}
	const self_type &operator/=(const scalar_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]/=rhs;
		}
		return *this;
	}
	const self_type &operator/=(const self_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]/=rhs[i];
		}
		return *this;
	}
	const self_type &operator+=(const scalar_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]+=rhs;
		}
		return *this;
	}
	const self_type &operator+=(const self_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]+=rhs[i];
		}
		return *this;
	}
	const self_type &operator-=(const scalar_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]-=rhs;
		}
		return *this;
	}
	const self_type &operator-=(const self_type &rhs){
		for(size_t i=0;i<_Len;++i){
			data[i]-=rhs[i];
		}
		return *this;
	}
	static const self_type Zeros(){
		self_type z;
		z.SetToZero();
		return z;
	}
	void SetToZero(){
		for(size_t i(0);i<_Len;++i){
			data[i] = 0;
		}
	}
	const scalar_type NormSq() const{
		scalar_type ns(0);
		for(size_t i(0); i<_Len; ++i){
			ns+=data[i]*data[i];
		}
		return ns;
	}
	const scalar_type &operator[](size_t i) const{
		return data[i];
	}
	scalar_type &operator[](size_t i){
		return data[i];
	}
	void Set(const scalar_type &v){
		std::fill(data,data+_Len,v);
	}
	scalar_type *Begin(){ return data; }
	scalar_type *End(){ return data+_Len; }
	const scalar_type *Begin() const{ return data; }
	const scalar_type *End() const{ return data+_Len; }
	scalar_type data[_Len];
};

template<typename T> 
const T operator+(const T &lhs, const T &rhs){
	T ret(lhs);
	return ret+=rhs;
}

template<typename T>
const T operator-(const T &lhs, const T &rhs){
	T ret(lhs);
	return ret-=rhs;
}

template<typename T>
const T operator*(const T &lhs, const T &rhs){
	T ret(lhs);
	return ret*=rhs;
}

template<typename T>
const T operator/(const T &lhs, const T &rhs){
	T ret(lhs);
	return ret/=rhs;
}



template<typename _S,size_t _Len> inline void SetToZero(FixedVector<_S,_Len> &x){
	x.SetToZero();
}


template< typename _S, size_t _Len > const _S DistSqr(const FixedVector<_S,_Len> &l, const FixedVector<_S,_Len> &r){
	return (l-r).NormSq();
}


template< typename _OS, typename _S, size_t _Len > _OS &operator<<( _OS &os, const FixedVector<_S,_Len> &v ){
	os<<os.widen('[');
	for(size_t i(0);i<_Len;++i){
		os<<v[i]<<os.widen(',');
	}
	os<<os.widen(']');
	return os;
}

