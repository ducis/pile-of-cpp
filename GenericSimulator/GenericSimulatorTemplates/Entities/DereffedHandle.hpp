#pragma once

#include "Meta/handle.hpp"
#include "Meta/entity.hpp"
#include "static_assert.hpp"

namespace GenericSim{ namespace Entities{
	template<typename _HandleToEntity>
	class DereffedHandle{
		typedef GenericSim::Meta::GetHandleElementType<_HandleToEntity>::Type wrapped_entity_type;
		STATIC_ASSERT(( GenericSim::Meta::IsEntity<wrapped_entity_type>::value ));
		_HandleToEntity m_h;
	public:
		typedef typename GenericSim::Meta::GetEntityConcept<wrapped_entity_type>::Type entity_concept_type;

		template< typename entity_concept_type::Labels N >
		struct AtResultType{
			typedef typename GenericSim::Meta::GetEntityAtResultType<wrapped_entity_type>::Type Type;
		};

		template< typename entity_concept_type::Labels N >
		struct ConstAtResultType{
			typedef typename GenericSim::Meta::GetEntityAtResultType<const wrapped_entity_type>::Type Type;
		};

		template< typename entity_concept_type::Labels N >
		struct StateType{
			typedef typename GenericSim::Meta::GetEntityStateType<wrapped_entity_type>::Type Type;
		};

		template< typename entity_concept_type::Labels N >
		typename AtResultType<N>::Type at_e()
		{
			return m_h->at_e<N>();
		}

		template< typename entity_concept_type::Labels N >
		typename ConstAtResultType<N>::Type at_e() const
		{
			return const_cast<const wrapped_entity_type &>(*m_h).at_e<N>();
		}

		//be careful of operator=  !!!!
		HandleDeref(const _HandleToEntity &h):m_h(h){}
	};
}}
