#pragma once

namespace Utility{	
	class Unity{
	public:
		template<typename T>
		const T &operator*(const T &in) const{
			return in;
		}
	};
};