#pragma once

namespace MyUtilities{
	template<typename T> const T Ones();
	template<> const vector2 Ones(){
		return vector2(1,1);
	}
	template<> const vector3 Ones(){
		return vector3(1,1,1);
	}
	template<> const vector4 Ones(){
		return vector4(1,1,1,1);
	}
	template<typename T,typename P> const T Lerp(const T &a, const T &b, const P &param){//œﬂ–‘≤Â÷µ
		return a*(Ones<T>()-param)+b*param;
	}
};