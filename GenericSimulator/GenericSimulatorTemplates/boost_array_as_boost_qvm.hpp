#pragma once


#include <boost/qvm/all.hpp>
#include <boost/array.hpp>

namespace boost
{
	namespace qvm
	{
		template< class _Scalar, std::size_t _NumDim >
		struct v_traits< boost::array< _Scalar, _NumDim > >
		{
			typedef boost::array< _Scalar, _NumDim > vector_type;
			static int const dim = _NumDim;
			typedef _Scalar scalar_type;

			template <int I> static inline const scalar_type &r( const vector_type &v ) { return v[I]; }
			template <int I> static inline scalar_type &w( vector_type &v ) { return v[I]; }

			static inline const scalar_type &ir( int i, const vector_type &v ) { return v[i]; }
			static inline scalar_type &iw( int i, vector_type &v ){	return v[i]; }
		};

		
		template< class _Scalar >
		struct q_traits< boost::array< _Scalar, 4 > >{
			typedef boost::array< _Scalar, 4 > quaternion_type;
			typedef _Scalar scalar_type;
			template <int I> static inline const scalar_type &r(const quaternion_type &q){ return q[I]; }
			template <int I> static inline scalar_type &w(quaternion_type &q){ return q[I]; }
		};
	}
}
