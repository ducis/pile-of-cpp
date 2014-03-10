#pragma once

#include <boost/range/value_type.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include "Meta/handle.hpp"

namespace GenericSim{	namespace Handles{
	template< class _HandleToRange >
	class HandleToElementOfRandomAccessRange
	{
		typedef typename GenericSim::Meta::GetHandleElementType<_HandleToRange>::Type range_type;
		typedef typename boost::range_difference<range_type>::type id_type;
		_HandleToRange m_hR;
		id_type m_id;
	public:
		typedef typename boost::range_value< range_type >::type element_type;
		HandleToElementOfRandomAccessRange( const _HandleToRange &hR, const id_type &id ):m_hR(hR),m_id(id){}
		element_type &operator*() const{
			assert(boost::size(*m_hR)>m_id);
			return *(boost::begin(*m_hR)+m_id);
		}
		element_type *operator->() const{	return &**this; }
		friend bool operator<( const HandleToElementOfRandomAccessRange &l,const HandleToElementOfRandomAccessRange &r ){
			return l.m_hR<r.m_hR ? true : r.m_hR<l.m_hR ? false : l.m_id<r.m_id;
		}
	};
} }
