#pragma once


#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include "entities/ConceptedRASeq.hpp"


#include "static_assert.hpp"

#define GENSIM_PLAIN_TUPLE_MAX_SIZE FUSION_MAX_VECTOR_SIZE

namespace GenericSim{
	namespace Meta{
		template< class _EntityConcept, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( GENSIM_PLAIN_TUPLE_MAX_SIZE, class _T, void ) > 
		//template< class _EntityConcept, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( GENSIM_PLAIN_TUPLE_MAX_SIZE, class _T, boost::fusion::void_ ) > 
		struct MakePlainTuple{// Usage:  MakePlainTupe<SomeConcept, Type0, Type1, ...>::Type
			template< class T >	struct VoidCast{
				typedef T Type;
			};
			template<> struct VoidCast<void>{
				typedef boost::fusion::void_ Type;
			};
			
		#define MAKE_PARAM_G522R267YWSGSA( Z, N, _ )  BOOST_PP_COMMA_IF(N) typename VoidCast<_T##N>::Type
		//#define MAKE_PARAM_G522R267YWSGSA( Z, N, _ )  BOOST_PP_COMMA_IF(N) _T##N

			typedef GenericSim::Entities::ConceptedRASeq< _EntityConcept,boost::fusion::vector< BOOST_PP_REPEAT( GENSIM_PLAIN_TUPLE_MAX_SIZE, MAKE_PARAM_G522R267YWSGSA, _ ) > >Type;
		#undef MAKE_PARAM_G522R267YWSGSA
			//STATIC_ASSERT( boost::fusion::result_of::size<Type>::value==_EntityConcept::NDIM );
		};
	}
}
