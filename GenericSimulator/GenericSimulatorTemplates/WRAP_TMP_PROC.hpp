#pragma once

#include <boost/preprocessor/repetition/enum_params.hpp>

//#define WRAP_TEMP_PROC_MAX_ARITY 16
#define WRAP_TMP_PROC( ARITY, F )											\
	struct F##_PROC_OBJ_##ARITY												\
	{																		\
		typedef void result_type;											\
		template<BOOST_PP_ENUM_PARAMS( ARITY, typename A )>					\
		void operator()(BOOST_PP_ENUM_BINARY_PARAMS( ARITY, A, &a )) const	\
		{	F(BOOST_PP_ENUM_PARAMS( ARITY, a ));	}						\
	};


//#define WRAP_TEMP_PROC_1( F )												\
//	struct F##_PROC_OBJ_1													\
//	{																		\
//		template<typename A0> void operator()( A0 a ) const					\
//		{    F(a);	}														\
//	};
//
