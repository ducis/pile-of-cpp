#pragma once




//#include <boost/qvm/all.hpp>




template< class _Scalar, std::size_t _NumDim >
struct PlainVector{
	_Scalar elements[_NumDim];
	//template <class B> PlainVector(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
	//template <class B> const PlainVector &operator=(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
};



template< class _Scalar, std::size_t _NumRows, std::size_t _NumColumns, bool _ColumnMajor >
struct PlainMatrix;

template< class _Scalar, std::size_t _NumRows, std::size_t _NumColumns >
struct PlainMatrix< _Scalar,_NumRows,_NumColumns,false >{
	_Scalar elements[_NumRows][_NumColumns];
	//template <class B> PlainMatrix(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
	//template <class B> const PlainMatrix &operator=(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
};

template< class _Scalar, std::size_t _NumRows, std::size_t _NumColumns >
struct PlainMatrix< _Scalar,_NumRows,_NumColumns,true >{
	_Scalar elements[_NumColumns][_NumRows];
	//template <class B> PlainMatrix(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
	//template <class B> const PlainMatrix &operator=(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
};

template< class _Scalar, std::size_t _Size, bool _ColumnMajor>
struct MakeSquarePlainMatrix{
	typedef PlainMatrix< _Scalar, _Size, _Size, _ColumnMajor > Type;
};



template< class _Scalar >
struct PlainQuaternion{
	_Scalar elements[4];
	//template <class B> PlainQuaternion(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
	//template <class B> const PlainQuaternion &operator=(const B &rhs){
	//	boost::qvm::assign(*this,rhs);
	//}
};



#include <boost/qvm/all.hpp>

namespace boost
{
	namespace qvm
	{
		template< class _Scalar, std::size_t _NumDim >
		struct v_traits< PlainVector< _Scalar, _NumDim > >
		{
			typedef PlainVector<_Scalar, _NumDim> vector_type;
			static int const dim = _NumDim;
			typedef _Scalar scalar_type;

			template <int I> static inline const scalar_type &r( const vector_type &v ) { return v.elements[I]; }
			template <int I> static inline scalar_type &w( vector_type &v ) { return v.elements[I]; }

			static inline const scalar_type &ir( int i, const vector_type &v ) { return v.elements[i]; }
			static inline scalar_type &iw( int i, vector_type &v ){	return v.elements[i]; }
		};


		template< class _Scalar, std::size_t _NumRows, std::size_t _NumColumns >
		struct m_traits< PlainMatrix<_Scalar,_NumRows,_NumColumns,false> >
		{
			typedef PlainMatrix<_Scalar,_NumRows,_NumColumns,false> matrix_type;
			static int const rows = _NumRows;
			static int const cols = _NumColumns;
			typedef _Scalar scalar_type;
			template <int R,int C> static inline const scalar_type &r( const matrix_type &m ){ return m.elements[R][C]; }
			template <int R,int C> static inline scalar_type &w( matrix_type &m ){ return m.elements[R][C]; }

			static inline const scalar_type &ir( int r, int c, const matrix_type &m ){ return m.elements[r][c]; }
			static inline scalar_type &iw( int r, int c, matrix_type &m ){ return m.elements[r][c]; }
		};

		template< class _Scalar, std::size_t _NumRows, std::size_t _NumColumns >
		struct m_traits< PlainMatrix<_Scalar,_NumRows,_NumColumns,true> >
		{
			typedef PlainMatrix<_Scalar,_NumRows,_NumColumns,true> matrix_type;
			static int const rows = _NumRows;
			static int const cols = _NumColumns;
			typedef _Scalar scalar_type;
			template <int R,int C> static inline const scalar_type &r( const matrix_type &m ){ return m.elements[C][R]; }
			template <int R,int C> static inline scalar_type &w( matrix_type &m ){ return m.elements[C][R]; }

			static inline const scalar_type &ir( int r, int c, const matrix_type &m ){ return m.elements[c][r]; }
			static inline scalar_type &iw( int r, int c, matrix_type &m ){ return m.elements[c][r]; }
		};

		
		template< class _Scalar >
		struct q_traits< PlainQuaternion<_Scalar> >{
			typedef PlainQuaternion<_Scalar> quaternion_type;
			typedef _Scalar scalar_type;
			template <int I> static inline const scalar_type &r(const quaternion_type &q){ return q.elements[I]; }
			template <int I> static inline scalar_type &w(quaternion_type &q){ return q.elements[I]; }
		};
	}
}
