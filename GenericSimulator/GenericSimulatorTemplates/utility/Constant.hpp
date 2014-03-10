#pragma once

namespace Utility{
	template<typename T, T val>
	class Constant{
	public:
		operator T() const{
			return val;
		}
	};
}
