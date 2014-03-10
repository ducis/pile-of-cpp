#pragma once

#include "OperationTraits.h"


template<typename V> class UnifiedCoord{ //�����ڿɱ������(һά������,��ά�ľ��λ���ά�ĳ�����)�еĶ�λ����,��scale��offset�����������
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


template<typename V> struct OperationTraits<UnifiedCoord<V>,V>{ //ģ��trick,��������
	typedef V ProductType;
};
