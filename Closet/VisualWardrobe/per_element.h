#pragma once

#include <utility>
#include "VectorTraits.h"

namespace per_element{

	//���������ص���������ǽ������ƹ㵽���������Ӧ����

	template<typename _Lhs1st, typename _Lhs2nd, typename _Rhs1st, typename _Rhs2nd>  
	const std::pair< typename OperationTraits<_Lhs1st,_Rhs1st>::ProductType, typename OperationTraits<_Lhs2nd,_Rhs2nd>::ProductType >	
		operator*( const std::pair<_Lhs1st,_Lhs2nd> &lhs, const std::pair<_Rhs1st,_Rhs2nd> &rhs )
	{
		//����ͬ����std::pair���
		return std::make_pair( lhs.first*rhs.first, lhs.second*rhs.second );
	}
	template<typename V> const V &operator*=( V &lhs, const V &rhs ){
		//�������κο��Կ����ǹ̶�ά������[]����������������
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
