#pragma once

#include <boost/utility.hpp>
#include "utility/StackOnly.hpp"

namespace Utility{
	template< void _OnConstruction(), void _OnDestruction() >
	class PairedCall:boost::noncopyable,StackOnly{
	public:
		PairedCall(){ _OnConstruction(); }
		~PairedCall(){ _OnDestruction(); }
	};
}
