#pragma once

#include "OperationTraits.h"


template<typename V> class UnifiedCoord{ //用于在可变的区域(一维的区间,二维的矩形或三维的长方体)中的定位的量,由scale和offset两个向量组成
	friend const V operator*( const UnifiedCoord &lhs, const V &rhs ){
		return lhs.m_scale*rhs+lhs.m_offset;
	}
public:
	const V &Scale() const{ return m_scale; }
	const V &Offset() const{ return m_offset; }
	void SetScale(const V &scale){ m_scale = scale; }
	void SetOffset(const V &offset){ m_offset = offset; }
	UnifiedCoord(const V &scale, const V &offset): m_scale(scale), m_offset(offset){}
	UnifiedCoord(){}
private:
	V m_scale;
	V m_offset;
};


template<typename V> struct OperationTraits<UnifiedCoord<V>,V>{ //模版trick,不用在意
	typedef V ProductType;
};
