#pragma once

#include <utility>
#include "VectorTraits.h"

namespace per_element{

	//这里面重载的运算符都是将操作推广到逐分量的相应操作

	template<typename _Lhs1st, typename _Lhs2nd, typename _Rhs1st, typename _Rhs2nd>  
	const std::pair< typename OperationTraits<_Lhs1st,_Rhs1st>::ProductType, typename OperationTraits<_Lhs2nd,_Rhs2nd>::ProductType >	
		operator*( const std::pair<_Lhs1st,_Lhs2nd> &lhs, const std::pair<_Rhs1st,_Rhs2nd> &rhs )
	{
		//两个同样的std::pair相乘
		return std::make_pair( lhs.first*rhs.first, lhs.second*rhs.second );
	}
	template<typename V> const V &operator*=( V &lhs, const V &rhs ){
		//适用于任何可以看作是固定维数且用[]索引的向量的类型
		for(size_t i(0); i<VectorTraits<V>::Dimension; ++i){
			lhs[i]*=rhs[i];
		}
		return lhs;
	}
	template<typename V> const V operator*( const V &lhs, const V &rhs ){
		V x(lhs);
		return x*=rhs;
	}
};
