#pragma once

namespace GenericSim{	namespace Meta{

	template<typename T, typename _Enable = void>
	struct GetHandleElementType{
		typedef typename T::element_type Type;
	};

	template<typename T>
	struct GetHandleElementType<T*,void>{
		typedef typename T Type;
	};

} }