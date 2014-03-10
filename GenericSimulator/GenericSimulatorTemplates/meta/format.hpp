#pragma once

#include <boost/array.hpp>

namespace GenericSim{
	namespace Meta{
		template < class _EntityConcept, class _StateIdentifier >
		struct MakeFormat{
			typedef boost::array<_StateIdentifier, _EntityConcept::NDIM> Type;
		};
	}
}
