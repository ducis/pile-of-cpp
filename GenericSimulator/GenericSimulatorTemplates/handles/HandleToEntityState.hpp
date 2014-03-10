#pragma once

#include "Meta/handle.hpp"
#include "Meta/entity.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace GenericSim{ namespace Handles{

	template<
		class _HandleToEntity,
		typename GenericSim::Meta::GetEntityLabels< typename GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type >::Type N
	>
	class HandleToEntityState
	{
		typedef typename GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type entity_type;
		_HandleToEntity m_hE;
	public:
		typedef typename GenericSim::Meta::GetEntityStateType<entity_type,N>::Type element_type;

		explicit HandleToEntityState( const _HandleToEntity &hE ): m_hE(hE){}

		element_type &operator*() const {	return m_hE->at_e<N>();	}
		element_type *operator->() const {	return &**this;	}
		friend bool operator<( const HandleToEntityState &l, const HandleToEntityState &r ){ return l.m_hE<r.m_hE; }
	};

	template< class _EntityConcept, typename _EntityConcept::Labels N, class _HandleToEntity >
		HandleToEntityState< 
			_HandleToEntity, 
			typename GenericSim::Meta::GetEntityLabels< typename GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type >::Type(N) 
		>
	MakeHandleToEntityState(const _HandleToEntity &hE){
		STATIC_ASSERT(( boost::is_same< 
			_EntityConcept, 
			typename GenericSim::Meta::GetEntityConcept< typename GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type >::Type 
		>::value ));
		return HandleToEntityState< 
			_HandleToEntity, 
			typename GenericSim::Meta::GetEntityLabels< typename GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type 
		>::Type(N) >(hE);
	}

} }
